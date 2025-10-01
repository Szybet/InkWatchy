#ifndef TAMP_COMPRESSOR_H
#define TAMP_COMPRESSOR_H

#ifdef __cplusplus
extern "C" {
#endif

#include "common.h"

/* Externally, do not directly edit ANY of these attributes */
typedef struct TampCompressor {
    /* nicely aligned attributes */

#if TAMP_ESP32  // Avoid bitfields for speed.
    uint32_t window_pos;
    uint32_t bit_buffer_pos;

    uint32_t input_size;
    uint32_t input_pos;

    /* Conf attributes */
    uint8_t conf_window;                 // number of window bits
    uint8_t conf_literal;                // number of literal bits
    uint8_t conf_use_custom_dictionary;  // Use a custom initialized dictionary.
#if TAMP_LAZY_MATCHING
    uint8_t conf_lazy_matching;  // Use lazy matching for better compression
#endif
    uint8_t min_pattern_size;

#if TAMP_LAZY_MATCHING
    /* Lazy matching cache */
    int16_t cached_match_index;
    uint8_t cached_match_size;
#endif
#else  // Use bitfields for reduced memory-usage
    /* Conf attributes */
    uint32_t conf_window : 4;                 // number of window bits
    uint32_t conf_literal : 4;                // number of literal bits
    uint32_t conf_use_custom_dictionary : 1;  // Use a custom initialized dictionary.
#if TAMP_LAZY_MATCHING
    uint32_t conf_lazy_matching : 1;  // Use lazy matching for better compression
#endif

    /* Other small attributes */
    uint32_t window_pos : 15;
    uint32_t bit_buffer_pos : 6;
    uint32_t min_pattern_size : 2;

    uint32_t input_size : 5;
    uint32_t input_pos : 4;

#if TAMP_LAZY_MATCHING
    /* Lazy matching cache */
    int16_t cached_match_index;
    uint8_t cached_match_size;
#endif
#endif  // TAMP_ESP32
    unsigned char input[16] /* __attribute__ ((aligned (16)))*/;
    uint32_t bit_buffer;

    unsigned char *window;
} TampCompressor;

/**
 * @brief Initialize Tamp Compressor object.
 *
 * @param[out] compressor Object to initialize.
 * @param[in] conf Compressor configuration. Set to NULL for default (window=10, literal=8).
 * @param[in] window Pre-allocated window buffer. Size must agree with conf->window.
 *                   If conf.use_custom_dictionary is true, then the window must be
 *                   externally initialized.
 *
 * @return Tamp Status Code. Returns TAMP_INVALID_CONF if an invalid conf state is provided.
 */
tamp_res tamp_compressor_init(TampCompressor *compressor, const TampConf *conf, unsigned char *window);

/**
 * @brief Sink data into input buffer.
 *
 * Copies bytes from `input` to the internal input buffer until the internal
 * input buffer is full, or the supplied input is exhausted.
 *
 * Somewhere between 0 and 16 bytes will be copied from the input.
 *
 * This is a computationally cheap/fast function.
 *
 * @param[in,out] compressor TampCompressor object to perform compression with.
 * @param[in] input Pointer to the input data to be sinked into compressor.
 * @param[in] input_size Size of input.
 * @param[out] consumed_size Number of bytes of input consumed. May be NULL.
 */
void tamp_compressor_sink(TampCompressor *compressor, const unsigned char *input, size_t input_size,
                          size_t *consumed_size);

/**
 * @brief Run a single compression iteration on the internal input buffer.
 *
 * This is a computationally intensive function.
 *
 * The most that will ever be written to output in a single invocation is:
 *
 *     (1 + 8 + WINDOW_BITS + 7) // 8
 *
 * or more simply:
 *
 *     (16 + WINDOW_BITS) // 8
 *
 * where // represents floor-division.  Explanation:
 *      * 1 - is_literal bit
 *      * 8 - maximum huffman code length
 *      * WINDOW_BITS - The number of bits to represent the match index. By default, 10.
 *      * 7 - The internal bit buffer may have up to 7 bits from a previous invocation. See NOTE
 * below.
 *      * // 8 - Floor divide by 8 to get bytes; the upto remaining 7 bits remain in the internal
 * output bit buffer.
 *
 * NOTE: Unintuitively, tamp_compressor_poll partially flushes (flushing multiples of 8-bits) the
 * internal output bit buffer at the **beginning** of the function call (not the end). This means
 * that a **previous** tamp_compressor_poll call may have placed up to (16 + WINDOW_BITS) bits in
 * the internal output bit buffer. The partial flush at the beginning of tamp_compressor_poll clears
 * as many whole-bytes as possible from this buffer. After this flush, there remains up to 7 bits,
 * to which the current call's compressed token/literal is added to.
 *
 * A 3-byte output buffer should be able to handle any compressor configuration.
 *
 * @param[in,out] compressor TampCompressor object to perform compression with.
 * @param[out] output Pointer to a pre-allocated buffer to hold the output compressed data.
 * @param[in] output_size Size of the pre-allocated output buffer.
 * @param[out] output_written_size Number of bytes written to output. May be NULL.
 *
 * @return Tamp Status Code. Can return TAMP_OK, TAMP_OUTPUT_FULL, or TAMP_EXCESS_BITS.
 */
tamp_res tamp_compressor_poll(TampCompressor *compressor, unsigned char *output, size_t output_size,
                              size_t *output_written_size);
// backwards compatibility for old naming
#define tamp_compressor_compress_poll tamp_compressor_poll

/**
 * @brief Check if the compressor's input buffer is full.
 *
 * @param[in] compressor TampCompressor object to check.
 *
 * @return true if the compressor is full, false otherwise.
 */
bool tamp_compressor_full(const TampCompressor *compressor);

/**
 * @brief Completely flush the internal bit buffer. Makes output "complete".
 *
 * The following table contains the most number of bytes that could be flushed in a worst-case
 * scenario:
 *
 * +---------------------+--------------------+-------------------------------------------+------------------------------------------+
 * | Literal Size (Bits) | Window Size (Bits) | Max Output Size write_token=false (Bytes) | Max
 * Output Size write_token=true (Bytes) |
 * +=====================+====================+===========================================+==========================================+
 * | 5                   | 8                  | 15                                        | 16 |
 * +---------------------+--------------------+-------------------------------------------+------------------------------------------+
 * | 5                   | 9-15               | 16                                        | 17 |
 * +---------------------+--------------------+-------------------------------------------+------------------------------------------+
 * | 6                   | 8                  | 17                                        | 18 |
 * +---------------------+--------------------+-------------------------------------------+------------------------------------------+
 * | 6                   | 9-15               | 18                                        | 19 |
 * +---------------------+--------------------+-------------------------------------------+------------------------------------------+
 * | 7                   | 8                  | 19                                        | 20 |
 * +---------------------+--------------------+-------------------------------------------+------------------------------------------+
 * | 7                   | 9-15               | 20                                        | 21 |
 * +---------------------+--------------------+-------------------------------------------+------------------------------------------+
 * | 8                   | 8                  | 21                                        | 22 |
 * +---------------------+--------------------+-------------------------------------------+------------------------------------------+
 * | 8                   | 9-15               | 22                                        | 23 |
 * +---------------------+--------------------+-------------------------------------------+------------------------------------------+
 *
 * @param[in,out] compressor TampCompressor object to flush.
 * @param[out] output Pointer to a pre-allocated buffer to hold the output compressed data.
 * @param[in] output_size Size of the pre-allocated output buffer.
 * @param[out] output_written_size Number of bytes written to output. May be NULL.
 * @param[in] write_token Write the FLUSH token, if appropriate. Set to true if you want to continue
 * using the compressor. Set to false if you are done with the compressor, usually at the end of a
 * stream.
 *
 * @return Tamp Status Code. Can return TAMP_OK, or TAMP_OUTPUT_FULL.
 */
tamp_res tamp_compressor_flush(TampCompressor *compressor, unsigned char *output, size_t output_size,
                               size_t *output_written_size, bool write_token);

/**
 * Callback-variant of tamp_compressor_compress.
 *
 * @param[in] callback User-provided function to be called every compression-cycle.
 * @param[in,out] user_data Passed along to callback.
 */
tamp_res tamp_compressor_compress_cb(TampCompressor *compressor, unsigned char *output, size_t output_size,
                                     size_t *output_written_size, const unsigned char *input, size_t input_size,
                                     size_t *input_consumed_size, tamp_callback_t callback, void *user_data);

/**
 * @brief Compress a chunk of data until input or output buffer is exhausted.
 *
 * @param[in,out] compressor TampCompressor object to perform compression with.
 * @param[out] output Pointer to a pre-allocated buffer to hold the output compressed data.
 * @param[in] output_size Size of the pre-allocated output buffer.
 * @param[out] output_written_size Number of bytes written to output. May be NULL.
 * @param[in] input Pointer to the input data to be compressed.
 * @param[in] input_size Number of bytes in input data.
 * @param[out] input_consumed_size Number of bytes of input data consumed. May be NULL.
 *
 * @return Tamp Status Code. Can return TAMP_OK, TAMP_OUTPUT_FULL, or TAMP_EXCESS_BITS.
 */
TAMP_ALWAYS_INLINE tamp_res tamp_compressor_compress(TampCompressor *compressor, unsigned char *output,
                                                     size_t output_size, size_t *output_written_size,
                                                     const unsigned char *input, size_t input_size,
                                                     size_t *input_consumed_size) {
    return tamp_compressor_compress_cb(compressor, output, output_size, output_written_size, input, input_size,
                                       input_consumed_size, NULL, NULL);
}

/**
 * Callback-variant of tamp_compressor_compress_and_flush.
 *
 * @param[in] callback User-provided function to be called every compression-cycle.
 * @param[in,out] user_data Passed along to callback.
 */
tamp_res tamp_compressor_compress_and_flush_cb(TampCompressor *compressor, unsigned char *output, size_t output_size,
                                               size_t *output_written_size, const unsigned char *input,
                                               size_t input_size, size_t *input_consumed_size, bool write_token,
                                               tamp_callback_t callback, void *user_data);

/**
 * @brief Compress a chunk of data until input or output buffer is exhausted.
 *
 * If the output buffer is full, buffer flushing will not be performed and TAMP_OUTPUT_FULL will be
 * returned. May be called again with an appropriately updated pointers and sizes.
 *
 * @param[in,out] compressor TampCompressor object to perform compression with.
 * @param[out] output Pointer to a pre-allocated buffer to hold the output compressed data.
 * @param[in] output_size Size of the pre-allocated output buffer.
 * @param[out] output_written_size Number of bytes written to output. May be NULL.
 * @param[in] input Pointer to the input data to be compressed.
 * @param[in] input_size Number of bytes in input data.
 * @param[out] input_consumed_size Number of bytes of input data consumed. May be NULL.
 *
 * @return Tamp Status Code. Can return TAMP_OK, TAMP_OUTPUT_FULL, or TAMP_EXCESS_BITS.
 */
TAMP_ALWAYS_INLINE tamp_res tamp_compressor_compress_and_flush(TampCompressor *compressor, unsigned char *output,
                                                               size_t output_size, size_t *output_written_size,
                                                               const unsigned char *input, size_t input_size,
                                                               size_t *input_consumed_size, bool write_token) {
    return tamp_compressor_compress_and_flush_cb(compressor, output, output_size, output_written_size, input,
                                                 input_size, input_consumed_size, write_token, NULL, NULL);
}

#ifdef __cplusplus
}
#endif

#endif
