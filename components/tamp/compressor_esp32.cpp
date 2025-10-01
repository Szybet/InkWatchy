#include "common.h"

/* Modification of the original tamp compressor.c, 2024 <https://github.com/BitsForPeople> */

#if TAMP_ESP32

#include <stdbool.h>
#include <stdlib.h>

#include <cstring>

#include "compressor.h"

#if TAMP_ESP32_AUTO_RESET_TASK_WDT
#include "esp_task_wdt.h"
#endif

#include "private/copyutil.hpp"
#include "private/tamp_search.hpp"

extern "C" {

typedef uint32_t u8;
typedef uint32_t u16;

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

#define MAX_PATTERN_SIZE (compressor->min_pattern_size + 13)
#define WINDOW_SIZE (1 << compressor->conf_window)

static inline void write_to_bit_buffer(TampCompressor *compressor, uint32_t bits, u8 n_bits) {
    compressor->bit_buffer_pos += n_bits;
    compressor->bit_buffer |= bits << (32 - compressor->bit_buffer_pos);
}

static inline tamp_res partial_flush(TampCompressor *compressor, unsigned char *output, size_t output_size,
                                     size_t *output_written_size) {
    for (*output_written_size = output_size; compressor->bit_buffer_pos >= 8 && output_size;
         output_size--, compressor->bit_buffer_pos -= 8, compressor->bit_buffer <<= 8)
        *output++ = compressor->bit_buffer >> 24;
    *output_written_size -= output_size;
    return (compressor->bit_buffer_pos >= 8) ? TAMP_OUTPUT_FULL : TAMP_OK;
}

// Object to hold a temporary copy of input data (also enforcing alignment)
class InputCopy {
   public:
    InputCopy(const TampCompressor &compressor) noexcept : compressor{compressor} { (void)input; }

    /**
     * @brief Returns a pointer to a sequence of at least \p minBytes bytes from the compressor's
     * current input buffer. Makes a copy of the bytes and returns a pointer into \c this->input if
     * necessary.
     *
     * @param minBytes minimum amount of sequential bytes wanted
     * @return pointer to a sequence of input bytes, either in \c compressor.input or to the copy in
     * \c this->input.
     */
    const uint8_t *getInput(const uint32_t minBytes) noexcept {
        constexpr uint32_t INBUF_SIZE = sizeof(TampCompressor::input);

        const uint32_t ipos = compressor.input_pos;
        if (ipos == 0) {
            // Nothing to be done.
            return compressor.input;
        }

        if constexpr (tamp::Arch::ESP32S3 && INBUF_SIZE == 16) {
            // Rotate 16 bytes from compressor.input 'right' (down) by ipos bytes and
            // store in this->input.

            asm volatile(
                // Load
                "EE.LD.128.USAR.IP q0, %[input], 16"
                "\n"
                "EE.VLD.128.IP q1, %[input], -16"
                "\n"
                // Align
                "EE.SRC.Q q0, q0, q1"
                "\n"
                // Rotate
                "WUR.SAR_BYTE %[shift]"
                "\n"
                "EE.SRC.Q q0, q0, q0"
                "\n"
                // Store
                "EE.VST.128.IP q0, %[out], 0"
                "\n"
                : "=m"(this->input)
                : [input] "r"(compressor.input), [shift] "r"(ipos), [out] "r"(this->input), "m"(compressor.input));

            return this->input;

        } else {
            const uint32_t l1 = INBUF_SIZE - ipos;
            if (minBytes <= l1) {
                // Nothing to be done.
                return compressor.input + ipos;
            } else {
                mem::cpy_short<INBUF_SIZE>(this->input, compressor.input + ipos, l1);
                mem::cpy_short<INBUF_SIZE>(this->input + l1, compressor.input, minBytes - l1);
                return this->input;
            }
        }
    }

   private:
    alignas(16) uint8_t input[sizeof(TampCompressor::input)];
    const TampCompressor &compressor;
};

void find_best_match(TampCompressor *compressor, uint16_t *match_index, uint8_t *match_size) {
    tamp::byte_span out{};

#if TAMP_ESP32_AUTO_RESET_TASK_WDT
    esp_task_wdt_reset();
#endif

    if (TAMP_LIKELY(compressor->input_size >= compressor->min_pattern_size)) {
        const uint32_t patLen = MIN(compressor->input_size, MAX_PATTERN_SIZE);

        /* We need the pattern to match to be sequential in memory.
           If the current pattern wraps around in the input buffer, we make a
           straightened-out copy to use for the search. If not, we use the
           input data directly.
        */

        InputCopy input{*compressor};

        out = tamp::Locator::find_longest_match(input.getInput(patLen), patLen, compressor->window, WINDOW_SIZE);
    }
    *match_size = out.size();
    *match_index = out.data() - compressor->window;
}

}  // extern "C"

#endif  // TAMP_ESP32
