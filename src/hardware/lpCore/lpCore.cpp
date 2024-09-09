#include "lpCore.h"

#if LP_CORE

/*
https://github.com/esp-rs/esp-hal/blob/main/esp-lp-hal/src/lib.rs
https://github.com/espressif/esp-idf/blob/master/examples/system/ulp/lp_core/build_system/main/lp_core_build_system_example_main.c
https://github.com/espressif/esp-idf/tree/master/examples/system/ulp/lp_core
https://github.com/espressif/esp-idf/blob/59e18382702b6986be3d3f55e9ac7763c1397cf7/components/ulp/lp_core/lp_core.c#L141
https://github.com/espressif/esp-idf/blob/master/examples/system/ulp/lp_core/gpio/main/lp_core_gpio_example_main.c
*/

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