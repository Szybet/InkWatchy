#include "lpCore.h"

#if LP_CORE

#include "export/lp_logs.h"
// Maybe don't call those functions from there
#include "export/lp_rust.h"
#include <bootloader_common.h>

bool loadLpCore()
{
    debugLog("Loading lp core");
    // Load it from littlefs first
    bufSize bin = fsGetBlob("yatchy-lp-program.bin", "/other/");

    if (bin.size <= 0)
    {
        debugLog("Failed to load lp core");
        return false;
    }
    else
    {
        debugLog("Lp core size: " + String(bin.size));
    }

    esp_err_t err = ulp_lp_core_load_binary(bin.buf, bin.size);
    if (err != ESP_OK)
    {
        debugLog("Failed to load lp core: " + String(esp_err_to_name(err)));
        return false;
    }
    return true;
}

bool runLpCore()
{
    ulp_lp_core_cfg_t cfg = {
        .wakeup_source = ULP_LP_CORE_WAKEUP_SOURCE_LP_TIMER,
        .lp_timer_sleep_duration_us = 10 * 5,
    };

    debugLog("shared_mem->sleep_duration_ticks" + String(ulp_lp_core_memory_shared_cfg_get()->sleep_duration_ticks));
    debugLog("Running lp core");
    // It will fail to run if there was lp core already running
    esp_err_t err = ulp_lp_core_run(&cfg);
    debugLog("shared_mem->sleep_duration_ticks" + String(ulp_lp_core_memory_shared_cfg_get()->sleep_duration_ticks));
    return true;
}

#if DEBUG
#define LP_DEBUG_ADDRESS 0x50002000 // The address of bootloader rtc memory
void monitorLpCore()
{
    uint8_t *rtc_retain_mem = bootloader_common_get_rtc_retain_mem()->custom;
    while (true)
    {
        uint8_t read = *((volatile uint8_t *)rtc_retain_mem);
        if (read != LPOG_NOTHING_0)
        {
            debugLog("Lp log update: " + String(getLpLog(read)));
            *((volatile uint8_t *)rtc_retain_mem) = 0;
        }
        delayTask(100);
    }
}

const char *getLpLog(uint8_t id)
{
    switch (id)
    {
    case 0:
        return LPOG_NOTHING_0_STR;
    case 1:
        return LPOG_START_1_STR;
    case 2:
        return LPOG_UNKNOWN_255_STR;
    case 3:
        return LPOG_UNKNOWN_255_STR;
    case 4:
        return LPOG_UNKNOWN_255_STR;
    case 5:
        return LPOG_UNKNOWN_255_STR;
    case 6:
        return LPOG_UNKNOWN_255_STR;
    case 7:
        return LPOG_UNKNOWN_255_STR;
    case 8:
        return LPOG_UNKNOWN_255_STR;
    case 9:
        return LPOG_UNKNOWN_255_STR;
    case 10:
        return LPOG_UNKNOWN_255_STR;
    case 11:
        return LPOG_UNKNOWN_255_STR;
    case 12:
        return LPOG_UNKNOWN_255_STR;
    case 13:
        return LPOG_UNKNOWN_255_STR;
    case 14:
        return LPOG_UNKNOWN_255_STR;
    case 15:
        return LPOG_UNKNOWN_255_STR;
    case 16:
        return LPOG_UNKNOWN_255_STR;
    case 17:
        return LPOG_UNKNOWN_255_STR;
    case 18:
        return LPOG_UNKNOWN_255_STR;
    case 19:
        return LPOG_UNKNOWN_255_STR;
    case 20:
        return LPOG_UNKNOWN_255_STR;
    case 21:
        return LPOG_UNKNOWN_255_STR;
    case 22:
        return LPOG_UNKNOWN_255_STR;
    case 23:
        return LPOG_UNKNOWN_255_STR;
    case 24:
        return LPOG_UNKNOWN_255_STR;
    case 25:
        return LPOG_UNKNOWN_255_STR;
    case 26:
        return LPOG_UNKNOWN_255_STR;
    case 27:
        return LPOG_UNKNOWN_255_STR;
    case 28:
        return LPOG_UNKNOWN_255_STR;
    case 29:
        return LPOG_UNKNOWN_255_STR;
    case 30:
        return LPOG_UNKNOWN_255_STR;
    case 31:
        return LPOG_UNKNOWN_255_STR;
    case 32:
        return LPOG_UNKNOWN_255_STR;
    case 33:
        return LPOG_UNKNOWN_255_STR;
    case 34:
        return LPOG_UNKNOWN_255_STR;
    case 35:
        return LPOG_UNKNOWN_255_STR;
    case 36:
        return LPOG_UNKNOWN_255_STR;
    case 37:
        return LPOG_UNKNOWN_255_STR;
    case 38:
        return LPOG_UNKNOWN_255_STR;
    case 39:
        return LPOG_UNKNOWN_255_STR;
    case 40:
        return LPOG_UNKNOWN_255_STR;
    case 41:
        return LPOG_UNKNOWN_255_STR;
    case 42:
        return LPOG_UNKNOWN_255_STR;
    case 43:
        return LPOG_UNKNOWN_255_STR;
    case 44:
        return LPOG_UNKNOWN_255_STR;
    case 45:
        return LPOG_UNKNOWN_255_STR;
    case 46:
        return LPOG_UNKNOWN_255_STR;
    case 47:
        return LPOG_UNKNOWN_255_STR;
    case 48:
        return LPOG_UNKNOWN_255_STR;
    case 49:
        return LPOG_UNKNOWN_255_STR;
    case 50:
        return LPOG_UNKNOWN_255_STR;
    case 51:
        return LPOG_UNKNOWN_255_STR;
    case 52:
        return LPOG_UNKNOWN_255_STR;
    case 53:
        return LPOG_UNKNOWN_255_STR;
    case 54:
        return LPOG_UNKNOWN_255_STR;
    case 55:
        return LPOG_UNKNOWN_255_STR;
    case 56:
        return LPOG_UNKNOWN_255_STR;
    case 57:
        return LPOG_UNKNOWN_255_STR;
    case 58:
        return LPOG_UNKNOWN_255_STR;
    case 59:
        return LPOG_UNKNOWN_255_STR;
    case 60:
        return LPOG_UNKNOWN_255_STR;
    case 61:
        return LPOG_UNKNOWN_255_STR;
    case 62:
        return LPOG_UNKNOWN_255_STR;
    case 63:
        return LPOG_UNKNOWN_255_STR;
    default:
        return LPOG_UNKNOWN_255_STR;
    }
}
#endif
#endif