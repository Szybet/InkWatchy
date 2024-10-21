#include "lpCore.h"

#if LP_CORE

bool screenForceNextFullTimeWrite = false; // Make the watchface update the whole time bar
bool screenTimeChanged = false;            // If the watchface has written time (minutes / hours) to the screen

#define LP_CORE_SCREEN_X 8
#define LP_CORE_SCREEN_Y 2
#define LP_CORE_SCREEN_W 185
#define LP_CORE_SCREEN_H 56

void lpCoreScreenPrepare(bool now)
{
    debugLog("Clearing screen space for lp core");
    display.fillRect(LP_CORE_SCREEN_X, LP_CORE_SCREEN_Y, LP_CORE_SCREEN_W, LP_CORE_SCREEN_H, GxEPD_WHITE);
    if (now == true)
    {
        updateDisplay(PARTIAL_UPDATE);
    }
    else
    {
        dUChange = true;
    }
}

void stopLpCore()
{
    ulp_lp_core_stop();
    delayTask(10);
    deInitRtcGpio();
    delayTask(10);
}

void clearLpCoreRtcMem()
{
    debugLog("Clearing lp core rtc mem");
    rtc_retain_mem_t *rtc_mem = bootloader_common_get_rtc_retain_mem();
#ifdef CONFIG_BOOTLOADER_CUSTOM_RESERVE_RTC
    memset(rtc_mem->custom, 0, CONFIG_BOOTLOADER_CUSTOM_RESERVE_RTC_SIZE);
#endif
}

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

/*
const RES_PIN: u8 = 2; // Output
const DC_PIN: u8 = 3; // Output
const BUSY_PIN: u8 = 4; // Input
pub const MOSI_PIN: u8 = 5; // Output
pub const SCK_PIN: u8 = 7; // Output
*/

void initRtcInvidualGpio(int pin, rtc_gpio_mode_t direction)
{
    // https://github.com/espressif/esp-idf/blob/3c99557eeea4e0945e77aabac672fbef52294d54/examples/system/ulp/ulp_riscv/gpio/main/ulp_riscv_example_main.c#L42
    gpio_num_t gpio_pin = gpio_num_t(pin);
    debugLog("Configuring RTC pin: " + String(gpio_pin));
    ESP_ERROR_CHECK(rtc_gpio_init(gpio_pin));
    ESP_ERROR_CHECK(rtc_gpio_set_direction(gpio_pin, direction));
    // ESP_ERROR_CHECK(rtc_gpio_isolate(gpio_pin)); // This fixes current draw but doesn't allow the lp core to use it
    ESP_ERROR_CHECK(rtc_gpio_pulldown_dis(gpio_pin));
    ESP_ERROR_CHECK(rtc_gpio_pullup_dis(gpio_pin));

    // debugLog("Got level for this gpio rtc: " + String(rtc_gpio_get_level(gpio_pin))); // Reports 0 for reset

    // This messes up gpio when the hp core is not going to sleep, probably
    // Or when it's not cleared up properly? - Nope, this is fine to call even without this, but this still messes things up, maybe its not needed at all
    // https://github.com/search?q=repo%3Aespressif%2Fesp-idf+rtc_gpio_hold_en&type=code
    // it's used only in risv and fsm, not lp core?
#if LP_CORE_TEST_ENABLED == false && true == false
    debugLog("Setting rtc_gpio_hold_en for pin: " + String(pin));
    ESP_ERROR_CHECK(rtc_gpio_hold_en(gpio_pin));
#endif
}

void initRtcGpio()
{
    // De init the screen before this!
#if ATCHY_VER == YATCHY
    // Also make sure about this
    // Set screen cs to low
    gpioExpander.setPinMode(YATCHY_DISPLAY_CS, MCP_OUTPUT);
    // setPinPullUp(YATCHY_DISPLAY_CS, false); // Not needed, it's false at default
    gpioExpander.setPinState(YATCHY_DISPLAY_CS, LOW);
#endif

    initRtcInvidualGpio(EPD_RESET, RTC_GPIO_MODE_OUTPUT_ONLY); // This funny guy increases power consumption up to 600 uA :(
    ESP_ERROR_CHECK(rtc_gpio_set_level(gpio_num_t(EPD_RESET), true)); // Fixes high fucking power consumption
    initRtcInvidualGpio(EPD_DC, RTC_GPIO_MODE_OUTPUT_ONLY);
    ESP_ERROR_CHECK(rtc_gpio_set_level(gpio_num_t(EPD_DC), true)); // Fixes high fucking power consumption
    initRtcInvidualGpio(EPD_BUSY, RTC_GPIO_MODE_INPUT_ONLY);
    initRtcInvidualGpio(EPD_SPI_MOSI, RTC_GPIO_MODE_OUTPUT_ONLY);
    initRtcInvidualGpio(EPD_SPI_SCK, RTC_GPIO_MODE_OUTPUT_ONLY);
}

void deInitRtcInvidualGpio(int pin)
{
    gpio_num_t gpio_pin = gpio_num_t(pin);
    debugLog("Deconfiguring RTC pin: " + String(gpio_pin));
    ESP_ERROR_CHECK(rtc_gpio_deinit(gpio_pin));
    ESP_ERROR_CHECK(rtc_gpio_hold_dis(gpio_pin));
}

void deInitRtcGpio()
{
    deInitRtcInvidualGpio(EPD_RESET);
    deInitRtcInvidualGpio(EPD_DC);
    deInitRtcInvidualGpio(EPD_BUSY);
    deInitRtcInvidualGpio(EPD_SPI_MOSI);
    deInitRtcInvidualGpio(EPD_SPI_SCK);
}

bool runLpCore()
{
    ulp_lp_core_cfg_t cfg = {
        .wakeup_source = ULP_LP_CORE_WAKEUP_SOURCE_LP_TIMER,
        .lp_timer_sleep_duration_us = uint32_t(0.5 * 1000000),
    };

    if (screenTimeChanged == true)
    {
        clearLpCoreRtcMem();
    }
    debugLog("shared_mem->sleep_duration_ticks" + String(ulp_lp_core_memory_shared_cfg_get()->sleep_duration_ticks));
    debugLog("Running lp core");
    // It will fail to run if there was lp core already running
    esp_err_t err = ulp_lp_core_run(&cfg);
    if (err != ESP_OK)
    {
        debugLog("Failed to run lp core: " + String(esp_err_to_name(err)));
        return false;
    }
    debugLog("shared_mem->sleep_duration_ticks" + String(ulp_lp_core_memory_shared_cfg_get()->sleep_duration_ticks));
    return true;
}

void initManageLpCore()
{
    if (bootStatus.fromWakeup == true)
    {
        screenForceNextFullTimeWrite = true;
        rtc_retain_mem_t *rtc_mem = bootloader_common_get_rtc_retain_mem();
        wFTime.Hour = rtc_mem->custom[1];
        wFTime.Minute = rtc_mem->custom[2];
        debugLog("Updated from lp core hour and minute: " + getHourMinute(wFTime));
    }
    else
    {
        // First boot
        clearLpCoreRtcMem();
    }
}

#if DEBUG
void startLpCoreTest()
{
    bootStatus.fromWakeup = false; // To be sure
    initDisplay();
    initRTC();
    debugLog("Current unix time: " + String(getUnixTime(timeRTCUTC0)));

    stopLpCore();
    initRtcGpio();
    loadLpCore();
    runLpCore();
#if LP_CORE_SERIOUS_TEST == false
    monitorLpCore();
#else
    ESP_ERROR_CHECK(esp_sleep_enable_ulp_wakeup());
    esp_deep_sleep_start();
#endif
}

void monitorLpCore()
{
    uint8_t *rtc_custom = bootloader_common_get_rtc_retain_mem()->custom;
    while (true)
    {
        uint8_t read = *((volatile uint8_t *)rtc_custom);
        // debugLog("Read is: " + String(read));
        if (read != LPOG_NOTHING_0)
        {
            debugLog("Lp log update: " + String(getLpLog(read)));
            *((volatile uint8_t *)rtc_custom) = 0;
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
        return LPOG_SCREEN_WRITE_START_2_STR;
    case 3:
        return LPOG_INITIALIZED_PINS_3_STR;
    case 4:
        return LPOG_SCREEN_INIT_4_STR;
    case 5:
        return LPOG_SCREEN_FIRST_ACTION_5_STR;
    case 6:
        return LPOG_SCREEN_END_6_STR;
    case 7:
        return LPOG_SCREEN_POWEROFF_7_STR;
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