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
#include <cstdint>
#include <type_traits>

namespace mem {
namespace {
template <int32_t INC, typename T>
static inline void __attribute__((always_inline)) incptr(T*& ptr) noexcept {
    if constexpr (std::is_const_v<T>) {
        ptr = (T*)((const uint8_t*)ptr + INC);
    } else {
        ptr = (T*)((uint8_t*)ptr + INC);
    }
}

template <typename T>
static inline T* pplus(T* const ptr, const int32_t inc) {
    if constexpr (std::is_const_v<T>) {
        return (T*)((const uint8_t*)ptr + inc);
    } else {
        return (T*)((uint8_t*)ptr + inc);
    }
}

template <int32_t INC, typename T>
static inline T* pplus(T* const ptr) {
    if constexpr (std::is_const_v<T>) {
        return (T*)((const uint8_t*)ptr + INC);
    } else {
        return (T*)((uint8_t*)ptr + INC);
    }
}

template <uint32_t CNT>
static inline void __attribute__((always_inline)) _cpy(void* dst, const void* src) noexcept {
    if constexpr (CNT > 0) {
        using T =
            // std::conditional_t<(CNT >= sizeof(uint64_t)), uint64_t, // On architectures < 64 bit,
            // uint32_t is slightly preferable (less register pressure)
            std::conditional_t<(CNT >= sizeof(uint32_t)), uint32_t,
                               std::conditional_t<(CNT >= sizeof(uint16_t)), uint16_t, uint8_t>>;
        // >;
        constexpr size_t S = sizeof(T);
        *(T*)dst = *(const T*)src;
        if constexpr (CNT > S) {
            _cpy<CNT - S>(pplus<S>(dst), pplus<S>(src));
        }
    }
}

template <uint32_t N>
static constexpr uint32_t log2() {
    static_assert(N != 0);
    for (uint32_t i = 1; i < 32; ++i) {
        if ((N >> i) == 0) {
            return i - 1;
        }
    }
    return 32;
}

template <uint32_t N>
static constexpr uint32_t MSB = (1 << log2<N>());

/**
 * @brief Copies a number of bytes from \p src to \p dst.
 * The number of bytes copied is \c max( \p len \c , \p CEIL-1 \c ) , i.e.
 * 0 to \p CEIL-1 bytes can be copied.
 *
 * @tparam CEIL maximum number of bytes that can by copied + 1
 * @param dst
 * @param src
 * @param len
 */
template <uint32_t CEIL = 16>
static inline void __attribute__((always_inline)) unrolled_cpy(void* dst, const void* src, std::size_t len) noexcept {
    static_assert(CEIL != 0 /* && (CEIL & (CEIL-1)) == 0 */);
    if constexpr (CEIL > 32) {
        // You should probably be using std::memcpy...
        {
            void* const dend = pplus(dst, (len / 16) * 16);
            while (dst < dend) {
                _cpy<16>(dst, src);
                incptr<16>(dst);
                incptr<16>(src);
            }
        }
        unrolled_cpy<16>(dst, src, len);
    } else if constexpr ((CEIL & (CEIL - 1)) != 0) {
        static_assert(MSB<CEIL> <= 16);
        static_assert((CEIL - MSB<CEIL>) < 16);
        if (len >= MSB<CEIL>) {
            _cpy<CEIL - MSB<CEIL>>(dst, src);
            incptr<CEIL - MSB<CEIL>>(dst);
            incptr<CEIL - MSB<CEIL>>(src);
            len -= CEIL - MSB<CEIL>;
        }
        unrolled_cpy<MSB<CEIL>>(dst, src, len);
    } else if constexpr (CEIL >= 2) {
        if (len & (CEIL / 2)) {
            _cpy<CEIL / 2>(dst, src);
            incptr<CEIL / 2>(dst);
            incptr<CEIL / 2>(src);
        }
        unrolled_cpy<CEIL / 2>(dst, src, len);
    } else {
        (void)dst;
        (void)src;
        (void)len;
    }
}
}  // namespace

/**
 * @brief Copies a number of bytes from \p src to \p dst.
 * The number of bytes copied is \c max( \p len \c , \p CEIL-1 \c ) , i.e.
 * 0 to \p CEIL-1 bytes can be copied.
 *
 * @tparam CEIL maximum number of bytes that can by copied + 1
 * @param dst
 * @param src
 * @param len
 */
template <uint32_t CEIL = 16>
static inline void /* __attribute__((always_inline)) */ __attribute__((noinline))
cpy_short(void* dst, const void* src, const std::size_t len) noexcept {
    unrolled_cpy<CEIL>(dst, src, len);
}

}  // namespace mem
