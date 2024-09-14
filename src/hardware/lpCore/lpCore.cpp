#include "lpCore.h"

#if LP_CORE

bool loadLpCore() {
    // Load it from littlefs first
    bufSize bin = fsGetBlob("yatchy-lp-program.bin", "/other/");

    if(bin.size <= 0) {
        debugLog("Failed to load lp core");
        return false;
    } else {
        debugLog("Lp core size: " + String(bin.size));
    }

    esp_err_t err = ulp_lp_core_load_binary(bin.buf, bin.size);
    if (err != ESP_OK)
    {
        debugLog("Failed to load lp core: " + String(esp_err_to_name(err)));
        return false;
    }

    debugLog("shared_mem->sleep_duration_ticks" + String(ulp_lp_core_memory_shared_cfg_get()->sleep_duration_ticks));
    
    return true;
}

bool runLpCore() {
    ulp_lp_core_cfg_t cfg = {
        .wakeup_source = ULP_LP_CORE_WAKEUP_SOURCE_LP_TIMER,
        .lp_timer_sleep_duration_us = 10 * 5,
    };

    debugLog("shared_mem->sleep_duration_ticks" + String(ulp_lp_core_memory_shared_cfg_get()->sleep_duration_ticks));

    esp_err_t err = ulp_lp_core_run(&cfg);
    
    debugLog("shared_mem->sleep_duration_ticks" + String(ulp_lp_core_memory_shared_cfg_get()->sleep_duration_ticks));

    return true;
}

#if DEBUG
#define LP_DEBUG_ADDRESS 0x50002000
void monitorLpCore() {
    uint32_t previous_read = 1000;

    while (true) {
        // Read the volatile data from the specified address
        uint32_t read = *((volatile uint32_t *)LP_DEBUG_ADDRESS);

        if (read != previous_read) {
            debugLog("Current: " + String(read));
            previous_read = read;
        } else {
            debugLog(":(");
        }

        delayTask(900);
    }
}
#endif
#endif