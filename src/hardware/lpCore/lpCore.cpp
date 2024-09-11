#include "lpCore.h"

#if LP_CORE

bool loadLpCore() {
    // Load it from littlefs first
    bufSize bin = fsGetBlob("yatchy-lp-program.bin", "/other/");

    if(bin.size <= 0) {
        debugLog("Failed to load lp core");
        return false;
    }

    esp_err_t err = ulp_lp_core_load_binary(bin.buf, bin.size);
    if (err != ESP_OK)
    {
        debugLog("Failed to load lp core: " + String(esp_err_to_name(err)));
        return false;
    }
    return true;
}

bool runLpCore() {
    ulp_lp_core_cfg_t cfg = {
        .wakeup_source = ULP_LP_CORE_WAKEUP_SOURCE_LP_TIMER,
        .lp_timer_sleep_duration_us = 60 * 1000000,
    };

    esp_err_t err = ulp_lp_core_run(&cfg);
    
    return true;
}

#endif