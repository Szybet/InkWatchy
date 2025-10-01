#pragma once

#ifdef __XTENSA__
#include <xtensa/config/core-isa.h>
#endif

#ifdef ESP_PLATFORM
#include <sdkconfig.h>
#endif

namespace tamp {

/**
 * @brief Provides feature flags of the architecture we're building for.
 * (Mostly for Xtensa because the RISC-V's don't have many useful features for our use case.)
 *
 */
struct Arch {
    /**
     * @brief Are we running on an Xtensa architecture?
     *
     */
    static constexpr bool XTENSA =
#ifdef __XTENSA__
        true;
#else
        false;
#endif

    /**
     * @brief Do we have the ESP32-S3's ISA extensions?
     *
     */
    static constexpr bool ESP32S3 =
#if CONFIG_IDF_TARGET_ESP32S3
        true;
#else
        false;
#endif

    /**
     * @brief Do we have Xtensa's zero-overhead loops?
     *
     */
    static constexpr bool XT_LOOP =
#if XCHAL_HAVE_LOOPS
        true;
#else
        false;
#endif

    /**
     * @brief Does __builtin_clz() map to a hardware instruction?
     *
     */
    static constexpr bool HW_CLZ =
#if XCHAL_HAVE_NSA || defined(__riscv_zbb)
        true;
#else
        false;
#endif

    /**
     * @brief Does __builtin_ctz() map to a hardware instruction?
     *
     */
    static constexpr bool HW_CTZ =
#ifdef __riscv_zbb
        true;
#else
        false;
#endif

    /**
     * @brief Do we have hardware min/max operations?
     *
     */
    static constexpr bool HW_MINMAX =
#if XCHAL_HAVE_MINMAX
        true;
#else
        false;
#endif
};
}  // namespace tamp
