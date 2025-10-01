/*

    Copyright 2024, <https://github.com/BitsForPeople>

    This program is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by the
    Free Software Foundation, either version 3 of the License, or (at your
    option) any later version.

    This program is distributed in the hope that it will be useful, but
    WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
    or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License along
    with this program. If not, see <https://www.gnu.org/licenses/>.
*/
#pragma once
#include <esp_log.h>

#include <cstdint>

#if __has_include(<span>) && __cplusplus > 201703L
#include <span>
#endif
#include <algorithm>
#include <type_traits>

#include "tamp_arch.hpp"

#if __cplusplus >= 202002L
#define TAMP_CPP_LIKELY [[likely]]
#define TAMP_CPP_UNLIKELY [[unlikely]]
#else
#define TAMP_CPP_LIKELY
#define TAMP_CPP_UNLIKELY
#endif

namespace tamp {

// const char* const TAG = "TS";

#if __cpp_lib_span >= 202002L
using byte_span = std::span<const uint8_t>;
#else
class byte_span {
    const uint8_t* m_data{nullptr};
    std::size_t m_len{0};

   public:
    constexpr byte_span() noexcept = default;
    constexpr byte_span(const byte_span&) noexcept = default;
    constexpr byte_span(const uint8_t* const data, const std::size_t len) : m_data{data}, m_len{len} {}

    constexpr byte_span& operator=(const byte_span&) noexcept = default;

    constexpr const uint8_t* data() const noexcept { return m_data; }

    constexpr bool empty() const noexcept { return m_len == 0; }

    constexpr std::size_t size() const noexcept { return m_len; }

    constexpr std::size_t size_bytes() const noexcept { return m_len; }
};
#endif

/**
 * @brief Optimized functions for locating substring ("pattern") matches.
 *
 */
class Locator {
   public:
    // static constexpr bool STATS_ENABLED = false;
    // class Stats {
    //     private:
    //         static constexpr const char* TAG = "Tamp Cstats";
    //     public:
    //     uint32_t searchCnt {0};
    //     uint32_t cumPotSearchLen {0};
    //     uint32_t cumPatLen {0};
    //     uint32_t potMatchCnt {0};
    //     uint32_t matchCnt {0};
    //     uint32_t cumSearchLen {0};
    //     uint32_t cumMatchPatLen {0};

    //     // Needed as work-around for gcc bug
    //     // "error: default member initializer for ... required before the end of its enclosing class"
    //     constexpr Stats() noexcept :
    //         searchCnt {0},
    //         cumPotSearchLen {0},
    //         cumPatLen {0},
    //         potMatchCnt {0},
    //         matchCnt {0},
    //         cumSearchLen {0},
    //         cumMatchPatLen {0}
    //         {

    //         }

    //     void log() const noexcept {
    //         if constexpr (Locator::STATS_ENABLED) {
    //             ESP_LOGI(TAG,
    //             "\n\tSearches: %" PRIu32
    //             "\n\tdata len: %" PRIu32
    //             "\n\tcum patlen: %" PRIu32
    //             "\n\tpot matches: %" PRIu32
    //             "\n\tmatches: %" PRIu32
    //             "\n\tsearched: %" PRIu32
    //             "\n\tmatched: %" PRIu32,
    //             this->searchCnt,
    //             this->cumPotSearchLen,
    //             this->cumPatLen,
    //             this->potMatchCnt,
    //             this->matchCnt,
    //             this->cumSearchLen,
    //             this->cumMatchPatLen
    //             );
    //         }
    //     }

    //     constexpr void reset() noexcept {
    //         if constexpr (Locator::STATS_ENABLED) {
    //             searchCnt = 0;
    //             cumPotSearchLen = 0;
    //             cumPatLen = 0;
    //             potMatchCnt = 0;
    //             matchCnt = 0;
    //             cumSearchLen = 0;
    //             cumMatchPatLen = 0;
    //         }
    //     }

    //     constexpr void enterFind(const uint32_t dataLen, const uint32_t patLen) noexcept {
    //         if constexpr (Locator::STATS_ENABLED) {
    //             this->searchCnt += 1;
    //             this->cumPatLen += patLen;
    //             this->cumPotSearchLen += dataLen;
    //         }
    //     }

    //     constexpr void potMatchesFound(const uint32_t cnt) noexcept {
    //         if constexpr (Locator::STATS_ENABLED) {
    //             this->potMatchCnt += 1;
    //         }
    //     }

    //     constexpr void matchFound(const uint32_t patLen) noexcept {
    //         if constexpr (Locator::STATS_ENABLED) {
    //             this->matchCnt += 1;
    //             this->cumMatchPatLen += patLen;
    //         }
    //     }

    //     constexpr void matchFound(const uint32_t patLen, const uint32_t searchLen) noexcept {
    //         if constexpr (Locator::STATS_ENABLED) {
    //             this->matchFound(patLen);
    //             this->cumSearchLen += searchLen;
    //         }
    //     }

    //     constexpr void noMatchFound(const uint32_t dataLen) noexcept {
    //         if constexpr (Locator::STATS_ENABLED) {
    //             this->cumSearchLen += dataLen;
    //         }
    //     }

    // };

    // inline static Stats stats {};

   private:
    /*
      Challenge: Given a vector of 16 8-bit booleans (0x00/0xff), find the
      position (0...15) of every 0xff in that vector.

      We do this by mapping each position i to the 16-bit power-of-two 1<<i,
      performing a bit-wise AND of each 16-bit value with the corresponding
      8-bit boolean, and summing up all 16 16-bit values into one 16-bit
      value. On this value, we then use Count Leading Zeros (CLZ) to iterate
      over all '1' bits.

      Mapping the 8-bit values to 16-bit values isn't straight forward though,
      because we have only basic arithmetic operations on either 8- *or* 16-bit
      values at our disposal. So to go from 16x8 to 16x16 to 1x16 bits (fast),
      some trickery is needed.

      The ACCX register is 39+1 bits wide and lets us accumulate unsigned 8-bit
      values into a single result of up to 39 bits, i.e. enough for the 16 bits
      we need.


      After masking with the 8-bit booleans, we multiply (mac) this vector with
      itself, then mac the values at all even positions again which makes the
      even positions 'worth' 2x as much, so we effectively get an 8x16 mac with
      {2*(128*128),(128*128),2*(64*64),(64*64),...,2*(1*1),(1*1)}, i.e.
      {(1<<15),(1<<14),(1<<13),(1<<12),...,(1<<1),(1<<0)} as desired.

      Positions in the vector are mapped to bits in descending order, the first
      u8 to 1<<15, the last u8 to 1<<0. This is because the Xtensas have a 1-cycle
      "count leading zeroes" (CLZ) instruction ("NSAU"), whereas a "count trailing
      zeroes" (CTZ) or "find first set" (FFS) requires ~4 instructions.
     */
    /**
     * @brief Vector of \c uint8_t used for extracting match positions when
     * using ESP32-S3 SIMD.
     */
    alignas(16) static constexpr uint8_t POS_U8[16]{
        128, 128, 64, 64, 32, 32, 16, 16, 8, 8, 4, 4, 2, 2, 1, 1,
    };

    /**
     * @brief Under the precondition that *(uint32_t*)a != *(uint32_t*)b, returns the
     * common prefix length of *a and *b in bytes (0...3)
     *
     * @param a
     * @param b
     * @return common prefix length of *a and *b in bytes (0...3)
     */
    static uint32_t subword_match_len(const void* a, const void* b) noexcept {
        if constexpr (Arch::HW_CTZ) {
            // CTZ(a^b) / 8:
            const uint32_t x = as<uint32_t>(a) ^ as<uint32_t>(b);
            return __builtin_ctz(x) / 8;
        } else if constexpr (false && Arch::HW_CLZ) {
            // Emulate CTZ via CLZ:
            uint32_t x = as<uint32_t>(a) ^ as<uint32_t>(b);
            x = x & -x;
            return (uint32_t)(31 - __builtin_clz(x)) / 8;
        } else {
            const uint32_t x = as<uint32_t>(a) ^ as<uint32_t>(b);
            if ((x << 16) == 0) {
                if ((x << 8) == 0) {
                    return 3;
                } else {
                    return 2;
                }
            } else {
                if ((x << 24) == 0) {
                    return 1;
                } else {
                    return 0;
                }
            }
        }
    }

    template <int32_t INC, typename T>
    static void __attribute__((always_inline)) incptr(T*& ptr) noexcept {
        ptr = (T*)(p<uint8_t>(ptr) + INC);
    }

    template <typename T>
    static const T* p(const void* const ptr) noexcept {
        return (const T*)ptr;
    }

    template <typename T>
    static T __attribute__((always_inline)) as(const void* const ptr) noexcept {
        return *(const T*)(ptr);
    }

    /**
     * @brief Returns \p x rounded down to the next lower multiple of \p M.
     *
     * @tparam M
     * @param x
     * @return greatest integer multiple of \p M <= \p x
     */
    template <uint32_t M>
    static uint32_t multof(const uint32_t x) noexcept {
        static_assert(M != 0);
        if constexpr (M == 1) {
            return x;
        } else if constexpr ((M & (M - 1)) == 0) {
            return x & ~(M - 1);
        } else {
            constexpr uint64_t OOM = (0x100000000llu / M);
            // return (x / M) * M;
            return (uint32_t)((x * OOM) >> 32) * M;
        }
    }

    template <typename T, uint32_t M, uint32_t N = 0>
    static bool __attribute__((always_inline)) unrolled_find(const uint8_t*& data, const uint32_t v) noexcept {
        if (as<T>(data + N) != v) TAMP_CPP_LIKELY {
            if constexpr (N + 1 < M) {
                return unrolled_find<T, M, N + 1>(data, v);
            } else {
                return false;
            }
        } else {
            incptr<N>(data);
            return true;
        }
    }

    template <uint32_t M, uint32_t N = 0>
    static bool __attribute__((always_inline))
    unrolled_find_3(const uint8_t*& data, const uint32_t vl, const uint32_t vh) noexcept {
        /* Loading one uint32_t from data, we compare
         * (*(uint32_t*)data << 8) against vl and
         * (*(uint32_t*)data >> 8) against vh
         * This turns out to be slightly faster than loading (*(uint32_t*)data) and (*(uint32_t*)(data+1)),
         * and reduces register pressure on Xtensas because L32I requires the offset to be a multiple of 4.
         */
        static_assert(N < M);
        if constexpr (N % 2 == 0) {
            if ((as<uint32_t>(data + ((N / 2) * 2)) << 8) != vl) TAMP_CPP_LIKELY {
                if constexpr (N + 1 < M) {
                    return unrolled_find_3<M, N + 1>(data, vl, vh);
                } else {
                    return false;
                }
            } else {
                incptr<N>(data);
                return true;
            }
        } else {
            if ((as<uint32_t>(data + ((N / 2) * 2)) >> 8) != vh) TAMP_CPP_LIKELY {
                if constexpr (N + 1 < M) {
                    return unrolled_find_3<M, N + 1>(data, vl, vh);
                } else {
                    return false;
                }
            } else {
                incptr<N>(data);
                return true;
            }
        }
    }
    /**
     * @brief Scalar pattern search for patterns <= sizeof(uint32_t) bytes in length.
     *
     * @param pattern
     * @param patLen must be <= sizeof(uint32_t)
     * @param data
     * @param dataLen
     * @return start of the first match found in \p data, or \c nullptr if none found.
     */
    // Attention: For some reason, when gcc inlines this, sometimes this DOUBLES the total processing time. Needs
    // investigation!
    static const uint8_t* /* __attribute__((noinline)) */ find_pattern_short_scalar(const uint8_t* const pattern,
                                                                                    const uint32_t patLen,
                                                                                    const uint8_t* data,
                                                                                    uint32_t dataLen) noexcept {
        const uint8_t* const end = data + dataLen - (patLen - 1);

        if (patLen > sizeof(uint16_t)) {
            if (patLen == sizeof(uint32_t)) TAMP_CPP_UNLIKELY {
                const uint32_t v = as<uint32_t>(pattern);

                if constexpr (Arch::XTENSA && Arch::XT_LOOP) {
                    uint32_t tmp;

                    asm("LOOPNEZ %[len], end_%="
                        "\n"
                        "L32I %[tmp], %[data], 0"
                        "\n"
                        "ADDI %[data], %[data], 1"
                        "\n"  // Pipelining.
                        "BEQ %[tmp], %[v], found_%="
                        "\n"
                        "end_%=:"
                        "\n"
                        "MOVI %[data], 1"
                        "\n"  // Make result = 0 w/o a jump
                        "found_%=:"
                        "\n"
                        "ADDI %[data], %[data], -1"
                        "\n"
                        "exit_%=:"
                        : [tmp] "=&r"(tmp), [data] "+&r"(data)
                        : [v] "r"(v), [len] "r"(dataLen - (sizeof(uint32_t) - 1)),
                          "m"(*(const uint8_t(*)[dataLen])data));

                    return data;

                } else {
                    constexpr uint32_t LOOP_UNROLL_FACTOR = 8;

                    const uint8_t* const end_unrolled = data + multof<LOOP_UNROLL_FACTOR>(dataLen - (patLen - 1));
                    while (data < end_unrolled && !unrolled_find<uint32_t, LOOP_UNROLL_FACTOR>(data, v)) TAMP_CPP_LIKELY {
                        incptr<LOOP_UNROLL_FACTOR>(data);
                    }

                    if (data >= end_unrolled) {
                        // Nothing found so far.
                        while (data < end && as<uint32_t>(data) != v) {
                            incptr<1>(data);
                        }
                    }
                }
            } else {
                // assert patLen == 3

                // Given a 24-bit value in a 32-bit variable V and 4 bytes (32 bits) loaded from memory
                // in a 32-bit variable D,
                // if ((V << 8) == (D << 8)) then the lower 3 bytes (24 bits) of D are equal to V,
                // if (V == (D >> 8)) then the higher 3 bytes of D are equal to V.

                const uint32_t vl = as<uint32_t>(pattern) << 8;
                const uint32_t vh = vl >> 8;

                if constexpr (Arch::XTENSA && Arch::XT_LOOP) {
                    // In this case, loading + incrementing + left-shifting takes 3 instructions/cycles,
                    // while right-shifting takes only 1. So instead of 4 instructions per byte we use
                    // (4+2) instructions per 2 bytes, i.e. 3 per byte, i.e. 25% less.
                    uint32_t tmp_32;
                    uint32_t tmp_24;
                    const uint32_t len = end - data;

                    asm("LOOPNEZ %[len], end_%="
                        "\n"

                        "L32I %[tmp_32], %[data], 0"
                        "\n"
                        "ADDI %[data], %[data], 2"
                        "\n"  // Pipelining.

                        "SLLI %[tmp_24], %[tmp_32], 8"
                        "\n"
                        "BEQ %[tmp_24], %[vl], found_lo_%="
                        "\n"

                        "SRLI %[tmp_24], %[tmp_32], 8"
                        "\n"
                        "BEQ %[tmp_24], %[vh], found_hi_%="
                        "\n"

                        "end_%=:"
                        "\n"
                        "ADDI %[data], %[data], 2"
                        "\n"  // Make sure data points to end or end+1. (We need the check at the end anyway in case we
                              // overread and found_hi a match one byte too late.)
                        "found_lo_%=:"
                        "\n"
                        "ADDI %[data], %[data], -1"
                        "\n"
                        "found_hi_%=:"
                        "\n"
                        "ADDI %[data], %[data], -1"
                        "\n"
                        "exit_%=:"
                        "\n"
                        : [tmp_32] "=&r"(tmp_32), [tmp_24] "=&r"(tmp_24), [data] "+&r"(data)
                        : [vl] "r"(vl), [vh] "r"(vh), [len] "r"((len + 1) / 2), "m"(*(const uint8_t(*)[dataLen])data));

                } else {
                    constexpr uint32_t LOOP_UNROLL_FACTOR = 8;

                    // Loop unrolled 8x.
                    const uint8_t* const end_unrolled = data + multof<LOOP_UNROLL_FACTOR>(dataLen - (patLen - 1));

                    while (data < end_unrolled && !unrolled_find_3<LOOP_UNROLL_FACTOR>(data, vl, vh)) TAMP_CPP_LIKELY {
                        incptr<LOOP_UNROLL_FACTOR>(data);
                    }

                    if (data >= end_unrolled) {
                        // Nothing found so far.
                        while (data < end && (as<uint32_t>(data) << 8) != vl) {
                            incptr<1>(data);
                        }
                    }
                }
            }

        } else {
            // assert patLen <= sizeof(uint16_t)

            if (patLen == sizeof(uint16_t)) {
                // This is where ~50% of the total CPU time is spent!

                const uint32_t v = as<uint16_t>(pattern);

                if constexpr (Arch::XTENSA && Arch::XT_LOOP) {
                    // Extracting two uint16_t's from a single uint32_t appears to be slightly slower overall, so we go
                    // with single uin16_t's.
                    uint32_t tmp;

                    asm("LOOPNEZ %[len], end_%="
                        "\n"
                        "L16UI %[tmp], %[data], 0"
                        "\n"
                        "ADDI %[data], %[data], 1"
                        "\n"  // Pipelining.
                        "BEQ %[tmp], %[v], found_%="
                        "\n"
                        "end_%=:"
                        "\n"
                        "MOVI %[data], 1"
                        "\n"  // Make result = 0 w/o a jump
                        "found_%=:"
                        "\n"
                        "ADDI %[data], %[data], -1"
                        "\n"
                        "exit_%=:"
                        : [tmp] "=&r"(tmp), [data] "+&r"(data)
                        : [v] "r"(v), [len] "r"(dataLen - (sizeof(uint16_t) - 1)),
                          "m"(*(const uint8_t(*)[dataLen])data));

                    return data;

                } else {
                    constexpr uint32_t LOOP_UNROLL_FACTOR = 8;

                    // Loop unrolled 8x.
                    const uint8_t* const end_unrolled = data + multof<LOOP_UNROLL_FACTOR>(dataLen - (patLen - 1));

                    while (data < end_unrolled && !unrolled_find<uint16_t, LOOP_UNROLL_FACTOR>(data, v)) TAMP_CPP_LIKELY {
                        incptr<LOOP_UNROLL_FACTOR>(data);
                    }

                    if (data >= end_unrolled) {
                        // Nothing found so far.
                        while (data < end && as<uint16_t>(data) != v) {
                            incptr<1>(data);
                        }
                    }
                }

            } else TAMP_CPP_UNLIKELY {
                // assert patLen == 1
                const uint32_t v = as<uint8_t>(pattern);
                if constexpr (Arch::XTENSA && Arch::XT_LOOP) {
                    uint32_t tmp = dataLen;
                    asm volatile(
                        "LOOPNEZ %[tmp], end_%="
                        "\n"
                        "L8UI %[tmp], %[data], 0"
                        "\n"
                        "BEQ %[tmp], %[v], end_%="
                        "\n"
                        "ADDI %[data], %[data], 1"
                        "\n"
                        "end_%=:"
                        : [tmp] "+r"(tmp), [data] "+r"(data)
                        : [v] "r"(v));
                } else {
                    while (data < end && as<uint8_t>(data) != v) {
                        incptr<1>(data);
                    }
                }
            }
        }
        return (data < end) ? data : nullptr;
    }

    // Unrolled loop generator for find_pattern_long_scalar.
    template <uint32_t M, uint32_t N = 0>
    static bool __attribute__((always_inline))
    unrolled_find_f_l(const uint8_t*& first, const uint8_t*& last, const uint32_t f, const uint32_t l) noexcept {
        if (f != as<uint32_t>(first + N) || l != as<uint32_t>(last + N)) TAMP_CPP_LIKELY {
            if constexpr (N + 1 < M) {
                return unrolled_find_f_l<M, N + 1>(first, last, f, l);
            } else {
                return false;
            }
        } else {
            incptr<N>(first);
            incptr<N>(last);
            return true;
        }
    }

    template <uint32_t M, uint32_t N = 0>
    static bool __attribute__((always_inline))
    _unrolled_find_f_l(const uint8_t*& first, const uint8_t*& last, const uint32_t f, const uint32_t l) noexcept {
        const uint8_t* fp = first;
        const uint8_t* const end = fp + M;
        const uint32_t len = &last - &first;
        do {
            const uint8_t* ff = find_pattern_short_scalar((const uint8_t*)&f, sizeof(uint32_t), fp, end - fp);
            if (ff) {
                if (*(const uint32_t*)(ff + len) != l) TAMP_CPP_LIKELY {
                    fp = ff + 1;
                } else {
                    first = ff;
                    last = ff + len;
                    return true;
                }
            } else {
                return false;
            }
        } while (fp < end);
        return false;
        // do {
        //     uint32_t ftmp1 = *(const uint32_t*)(fp++);
        //     uint32_t ftmp2 = *(const uint32_t*)(lp++);
        //     if(f == ftmp1 && l == ftmp2) TAMP_CPP_UNLIKELY {
        //         first = fp-1;
        //         last = lp-1;
        //         break;
        //     }
        // } while(fp < end);
        // if(fp < end) {
        //     return true;
        // } else {
        //     return false;
        // }
    }

    /**
     * @brief Scalar pattern search for patterns > sizeof(uint32_t) bytes in length.
     *
     * @param pattern
     * @param patLen must be >= sizeof(uint32_t)
     * @param data
     * @param dataLen
     * @return start of the first match found in \p data, or \c nullptr if none found.
     */
    // Attention: For some reason, when gcc inlines this, sometimes this DOUBLES the total processing time. Needs
    // investigation!
    static const uint8_t* /* __attribute__((noinline)) */ find_pattern_long_scalar(const uint8_t* pattern,
                                                                                   const uint32_t patLen,
                                                                                   const uint8_t* data,
                                                                                   uint32_t dataLen) noexcept {
        using T = uint32_t;
        constexpr uint32_t sw = sizeof(T);

        const uint8_t* first = data;
        const uint8_t* last = first + patLen - sw;
        const uint32_t f = as<T>(pattern);
        const uint32_t l = as<T>(pattern + patLen - sw);

        const uint8_t* const end = first + dataLen - (patLen - 1);

        const uint32_t cmpLen = patLen - std::min(patLen, 2 * sw);

        do {
            {
                constexpr uint32_t LOOP_UNROLL_FACTOR = 8;
                const uint8_t* const end_unrolled = first + multof<LOOP_UNROLL_FACTOR>(end - first);
                while (first < end_unrolled && !unrolled_find_f_l<LOOP_UNROLL_FACTOR>(first, last, f, l)) TAMP_CPP_LIKELY {
                    incptr<LOOP_UNROLL_FACTOR>(first);
                    incptr<LOOP_UNROLL_FACTOR>(last);
                }

                if (first >= end_unrolled) {
                    // Nothing found so far.
                    while (first < end) {
                        if (f == as<T>(first) && l == as<T>(last)) {
                            break;
                        } else {
                            incptr<1>(first);
                            incptr<1>(last);
                        }
                    };
                }
            }

            if (first < end) {
                if (cmpLen == 0 || cmp8(first + sw, pattern + sw, cmpLen) >= cmpLen) {
                    return first;
                } else {
                    incptr<1>(first);
                    incptr<1>(last);
                }
            }
        } while (first < end);

        return nullptr;
    }

    template <uint32_t M, uint32_t N = 0, typename P>
    static bool __attribute__((always_inline)) unrolled_cmp8(const P*& d1, const P*& d2) noexcept {
        if (*(p<uint8_t>(d1) + N) == *(p<uint8_t>(d2) + N)) TAMP_CPP_LIKELY {
            if constexpr (N + 1 < M) {
                return unrolled_cmp8<M, N + 1>(d1, d2);
            } else {
                return true;
            }
        } else {
            incptr<N>(d1);
            incptr<N>(d2);
            return false;
        }
    }

   public:
    /**
     * @brief Memory compare, like ::cmp(), but only compares single bytes in a loop.
     * This is faster on average if a mismatch within the first few (~4-6 or so) bytes is likely.
     *
     * @param d1
     * @param d2
     * @param len must be >= 1
     * @return uint32_t
     */
    static uint32_t cmp8(const void* d1, const void* d2, const uint32_t len) noexcept {
        const uint8_t* const start = p<uint8_t>(d1);

        if constexpr (Arch::XTENSA && Arch::XT_LOOP) {
            // Not really needed. GCC 12.2.0 generates the (ZOL) code for this too.

            // Memory barrier for the compiler
            asm volatile("" ::"m"(*(const uint8_t(*)[len])d1));
            asm volatile("" ::"m"(*(const uint8_t(*)[len])d2));

            uint32_t tmp1, tmp2;
            asm volatile(
                "LOOPNEZ %[cnt], end_%="
                "\n"

                "L8UI %[tmp2], %[d2], 0"
                "\n"
                "L8UI %[tmp1], %[d1], 0"
                "\n"

                "ADDI %[d2], %[d2], 1"
                "\n"  // Pipelining.

                "BNE %[tmp1], %[tmp2], end_%="
                "\n"

                "ADDI %[d1], %[d1], 1"
                "\n"

                "end_%=:"
                : [tmp1] "=r"(tmp1), [tmp2] "=r"(tmp2), [d1] "+r"(d1), [d2] "+r"(d2)
                : [cnt] "r"(len));

            return p<uint8_t>(d1) - start;

        } else {
            const uint8_t* const end = p<uint8_t>(d1) + len;

            while (d1 < end) {
                if (as<uint8_t>(d1) == as<uint8_t>(d2)) TAMP_CPP_LIKELY {
                    incptr<1>(d1);
                    incptr<1>(d2);
                } else {
                    break;
                }
            }
            return p<uint8_t>(d1) - start;
        }
    }

    /**
     * @brief Compare up to \p len bytes from \p d1 and \p d2 and return the length of
     * the common prefix of both memory regions.
     *
     * @param d1 pointer to memory to compare against \p d2
     * @param d2 pointer to memory to compare against \p d1
     * @param len maximum number of bytes to compare
     * @return common prefix length of \p *d1 and \p *d2, i.e. the number of equal bytes
     * from the start of both memory regions; returns \p len if all bytes are equal.
     */
    static uint32_t cmp(const void* d1, const void* d2, const uint32_t len) noexcept {
        const uint8_t* const end = (const uint8_t*)d1 + len;

        if constexpr (Arch::XTENSA && Arch::XT_LOOP) {
            // Memory barrier for the compiler
            asm volatile("" ::"m"(*(const uint8_t(*)[len])d1));
            asm volatile("" ::"m"(*(const uint8_t(*)[len])d2));

            {
                uint32_t tmp1, tmp2;

                // Start comparing in 4-byte chunks (32bit) until a mismatch is found.
                asm volatile(

                    "LOOPNEZ %[cnt], end_%="
                    "\n"

                    "L32I %[tmp2], %[d2], 0"
                    "\n"
                    "L32I %[tmp1], %[d1], 0"
                    "\n"

                    "ADDI %[d2], %[d2], 4"
                    "\n"  // Pipelining

                    "BNE %[tmp1], %[tmp2], exit_%="
                    "\n"

                    "ADDI %[d1], %[d1], 4"
                    "\n"

                    "end_%=:"
                    "\n"
                    "ADDI %[d2], %[d2], 4"
                    "\n"
                    "exit_%=:"
                    "\n"
                    "ADDI %[d2], %[d2], -4"
                    "\n"  // Undo preincrement of d2
                    : [tmp1] "=r"(tmp1), [tmp2] "=r"(tmp2), [d1] "+r"(d1), [d2] "+r"(d2)
                    : [cnt] "r"(len / 4));
            }

            {
                // Compare remaining data (0..3 bytes) byte-wise
                uint32_t tmp1, tmp2;
                asm volatile(

                    "LOOPNEZ %[cnt], end_%="
                    "\n"

                    "L8UI %[tmp2], %[d2], 0"
                    "\n"
                    "L8UI %[tmp1], %[d1], 0"
                    "\n"

                    "ADDI %[d2], %[d2], 1"
                    "\n"  // Pipelining.

                    "BNE %[tmp1], %[tmp2], end_%="
                    "\n"

                    "ADDI %[d1], %[d1], 1"
                    "\n"

                    "end_%=:"
                    : [tmp1] "=r"(tmp1), [tmp2] "=r"(tmp2), [d1] "+r"(d1), [d2] "+r"(d2)
                    : [cnt] "r"(end - (const uint8_t*)d1));
            }

            return (const uint8_t*)d1 - (end - len);

        } else {
            {
                const void* const end32 = p<uint8_t>(d1) + multof<4>(len);
                while (d1 < end32 && as<uint32_t>(d1) == as<uint32_t>(d2)) {
                    incptr<4>(d1);
                    incptr<4>(d2);
                }
            }

            if (d1 < end) {
                // Find any common prefix in (d1+0)...(d1+sizeof(uint32_t)-1)
                const uint32_t sml = subword_match_len(d1, d2);
                return std::min(len, (uint32_t)(((p<uint8_t>(d1) + len) - end) + sml));
            } else {
                return len;
            }
        }
    }

    /**
     * @brief Scalar, i.e. non-SIMD, pattern search.
     *
     * @param pattern start of pattern to search for
     * @param patLen length of pattern to search for
     * @param data start of data to search
     * @param dataLen length of data to search
     * @return first start of pattern in data, or \c nullptr if not found
     */
    static const uint8_t* find_pattern_scalar(const uint8_t* const pattern, const uint32_t patLen, const uint8_t* data,
                                              const uint32_t dataLen) noexcept {
        if (patLen > sizeof(uint32_t)) {
            return find_pattern_long_scalar(pattern, patLen, data, dataLen);
        } else {
            return find_pattern_short_scalar(pattern, patLen, data, dataLen);
        }
    }

    /**
     * @brief Searches \p data for the first occurrence of a \p pattern.
     * On ESP32-S3 targets, this uses SIMD instructions; delegates to ::find_pattern_scalar() on other targets.
     *
     * @param pattern start of pattern to search for
     * @param patLen length of pattern to search for
     * @param data start of data to search
     * @param dataLen length of data to search
     * @return first start of pattern in data, or \c nullptr if not found
     */
    static const uint8_t* __attribute__((noinline))
    find_pattern(const uint8_t* const pattern, const uint32_t patLen, const uint8_t* const data,
                 const uint32_t dataLen) noexcept {
        // stats.enterFind(dataLen, patLen);

        if constexpr (Arch::ESP32S3) {
            // Memory barrier for the compiler
            asm volatile("" ::"m"(*(const uint8_t(*)[dataLen])data));
            asm volatile("" ::"m"(*(const uint8_t(*)[patLen])pattern));

            // q7[n] := POS_U8[n]
            asm volatile(
                "LD.QR q7, %[bytes]"
                "\n"
                :
                : [bytes] "m"(POS_U8));

            // q4[n] := pattern[0]
            asm volatile("EE.VLDBC.8 q4, %[first]" : : [first] "r"(pattern));

            // q5[n] := pattern[patLen-1]
            asm volatile("EE.VLDBC.8 q5, %[last]" : : [last] "r"(pattern + patLen - 1));

            // Pointer to potential match on first byte of pattern:
            const uint8_t* first = data;
            // Pointer to potential match on last byte of pattern:
            const uint8_t* last = first + patLen - 1;

            const uint8_t* const end = first + dataLen - patLen;

            // Using (q1:q0) to load+align 'first' data,
            // (q3:q2) to load+align 'last' data
            asm volatile(
                "EE.VLD.128.IP q0, %[first], 16"
                "\n"
                "EE.LD.128.USAR.IP q1, %[first], 16"
                "\n"

                "EE.VLD.128.IP q2, %[last], 16"
                "\n"
                : [first] "+r"(first), [last] "+r"(last));

            const uint8_t* const pat1 = pattern + 1;
            // We won't need to compare the first and the last byte of pattern a second time.
            const uint32_t cmpLen = patLen - std::min(patLen, (uint32_t)2);

            // 'first' is ahead by 32 bytes of where we are actually searching.
            const uint8_t* const flimit = end + 32;

            do {
                // Want at least one iteration while there is any data left.
                uint32_t tmp = ((flimit - first) + 15) / 16;
                asm volatile(

                    "EE.ZERO.ACCX"
                    "\n"

                    "LOOPNEZ %[tmp], end_%="
                    "\n"

                    /* Align & compare data from 'first' */
                    "EE.SRC.Q.QUP q1, q0, q1"
                    "\n"

                    // Pipelining: Load next data from 'last'
                    "EE.LD.128.USAR.IP q3, %[last], 16"
                    "\n"

                    // q6[n] := first[n] == pattern[0]
                    "EE.VCMP.EQ.S8 q6, q1, q4"
                    "\n"

                    /* Align & compare data from 'last' */
                    "EE.SRC.Q.QUP q3, q2, q3"
                    "\n"
                    // q3[n] := last[n] == pattern[patLen-1]
                    "EE.VCMP.EQ.S8 q3, q3, q5"
                    "\n"

                    /* Fold: */
                    // ACCX += SUM( (int8_t)q6[n] * (int8_t)q3[n] )
                    // The comparisons above yield either 0 (false) or 0xff (true) for every lane.
                    // 0xff interpreted as signed 8-bit is -1.
                    // Thus, signed multiplication of the two comparison results is equivalent to a
                    // boolean AND, yielding either 0 or 1 (=(-1)*(-1)).
                    // Then accumulating these 16 multiplication results into ACCX yields the number
                    // of 1's (0...16); ACCX := COUNT( q6[n] && q3[n] )
                    "EE.VMULAS.S8.ACCX q6, q3"
                    "\n"

                    // Pipelining: Load next data from 'first'
                    "EE.LD.128.USAR.IP q1, %[first], 16"
                    "\n"

                    // Get result, exit loop if any matches:
                    "RUR.ACCX_0 %[tmp]"
                    "\n"
                    "BNEZ %[tmp], end_%="
                    "\n"

                    "end_%=:"
                    : [first] "+r"(first), [last] "+r"(last), [tmp] "+r"(tmp)
                    :);

                if (tmp) {
                    // stats.potMatchesFound(tmp);

                    // The result of the comparison is still in q6.
                    // Note: We must not disturb q0, q1, or q2 here, as their current contents are used in the next
                    // iteration.

                    /* Yes, it takes no more than 7 instructions, plus the 'lookup table' from POS_U8 (in q7),
                     * to extract 16 bits from the 16 8-bit boolean results of a vector comparison.
                     *
                     * Seriously. This is the fastest way I could come up with.
                     * The S3's PIE has only ONE horizontal SIMD instruction (VMULAS.ACCX).
                     */
                    asm volatile(

                        "EE.ZERO.ACCX"
                        "\n"

                        // AND both comparison results
                        // q6[n] := q6[n] && q3[n]
                        "EE.ANDQ q6, q6, q3"
                        "\n"

                        // And position factor with comparison mask:
                        // q6[n] := POS_U8[n] & q6[n]
                        "EE.ANDQ q6, q7, q6"
                        "\n"

                        // MAC 1: MAC all (non-masked) elements:
                        "EE.VMULAS.U8.ACCX q6, q6"
                        "\n"

                        // Extract only the even elements from q6, set the rest to 0.
                        "EE.ZERO.Q q3"
                        "\n"  // Get zeroes to fill the gaps.
                        "EE.VUNZIP.8 q6, q3"
                        "\n"

                        // MAC the even elements one more time:
                        "EE.VMULAS.U8.ACCX q6, q6"
                        "\n"

                        // Stall the pipeline for 1 cycle.
                        // Then continue:

                        // Extract:
                        "RUR.ACCX_0 %[tmp]"
                        "\n"

                        : [tmp] "=r"(tmp)
                        :);

                    // What we want is now in bits 15...0 of tmp, where bit i denotes if
                    // (first[15-i] == pattern[0] && last[15-i] == pattern[patLen-1])

                    tmp = tmp << 16;
                    const uint8_t* s1 = first - 48;

                    do {
                        {
                            const uint32_t bits = __builtin_clz(tmp) + 1;
                            s1 += bits;
                            tmp = tmp << bits;  // Remove the bit we're handling now.
                        }
                        if (s1 <= end) TAMP_CPP_LIKELY {
                            if (cmpLen == 0 || cmp8(s1, pat1, cmpLen) >= cmpLen) {
                                // stats.matchFound(patLen, (s1-1)-data + patLen);
                                return s1 - 1;
                            }
                        } else {
                            // Found match would extend beyond the end of data.
                            return nullptr;
                        }

                    } while (tmp != 0);
                }
            } while (first < flimit);

            // stats.noMatchFound(dataLen);
            return nullptr;

        } else {
            return find_pattern_scalar(pattern, patLen, data, dataLen);
        }
    }

    /**
     * @brief Searches \p data for the longest prefix of \p pattern that can be found.
     *
     * @param pattern
     * @param patLen length of the pattern; must be >= 2
     * @param data
     * @param dataLen
     * @return a std::span of the match found in data, or an empty std::span if no prefix was found.
     */
    static byte_span find_longest_match(const uint8_t* const pattern, const uint32_t patLen, const uint8_t* data,
                                        uint32_t dataLen) noexcept {
        const uint8_t* bestMatch{nullptr};
        uint32_t matchLen{0};

        {
            const uint8_t* match;
            uint32_t searchLen = 2;
            do {
                match = find_pattern(pattern, searchLen, data, dataLen);
                if (match) {
                    bestMatch = match;
                    matchLen = searchLen;
                    if (searchLen < patLen) TAMP_CPP_LIKELY {
                        // If the current match happens to extend beyond what we searched for,
                        // we'll take that too.
                        matchLen += cmp8(pattern + searchLen, match + searchLen, patLen - searchLen);
                    }
                    dataLen -= match + 1 - data;
                    data = match + 1;

                    searchLen = matchLen + 1;  // Next match must beat the current one.
                }
            } while (match && searchLen <= patLen && searchLen <= dataLen);
        }

        return byte_span{bestMatch, matchLen};
    }
};

}  // namespace tamp
