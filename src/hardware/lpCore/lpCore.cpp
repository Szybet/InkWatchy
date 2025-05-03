#include "lpCore.h"
#include "rtcMem.h"

#if LP_CORE

bool screenForceNextFullTimeWrite = false; // Make the watchface update the whole time bar
bool screenTimeChanged = false;            // If the watchface has written time (minutes / hours) to the screen

#define LP_CORE_SCREEN_X 8
#define LP_CORE_SCREEN_Y 2
#define LP_CORE_SCREEN_W 185
#define LP_CORE_SCREEN_H 56

void lpCoreScreenPrepare(bool now, bool setDuChange)
{
    debugLog("Clearing screen space for lp core");
    dis->fillRect(LP_CORE_SCREEN_X, LP_CORE_SCREEN_Y, LP_CORE_SCREEN_W, LP_CORE_SCREEN_H, GxEPD_WHITE);
    if (now == true)
    {
        updateDisplay(PARTIAL_UPDATE);
    }
    else if (setDuChange == true)
    {
        dUChange = true;
    }
}

void stopLpCore()
{
    if(bootStatus.reason == ulp) {
        delayTask(100);
    }
    ulp_lp_core_stop();
    delayTask(10);
    if(bootStatus.reason == ulp) {
        delayTask(100);
    }
    deInitRtcGpio();
    delayTask(10);
    if(bootStatus.reason == ulp) {
        delayTask(100);
    }
}

void setAlarmForLpCore()
{
#if INK_ALARMS
    rtc_retain_mem_t *rtc_mem = bootloader_common_get_rtc_retain_mem();
    uint32_t nextAlarmToRtc = (uint32_t)rM.nextAlarm;
    memcpy(&rtc_mem->custom[4], &nextAlarmToRtc, 4);
#endif
}

void clearLpCoreRtcMem()
{
    debugLog("Clearing lp core rtc mem");
    rtc_retain_mem_t *rtc_mem = bootloader_common_get_rtc_retain_mem();
#ifdef CONFIG_BOOTLOADER_CUSTOM_RESERVE_RTC
    memset(rtc_mem->custom, 0, CONFIG_BOOTLOADER_CUSTOM_RESERVE_RTC_SIZE);
#endif
    setAlarmForLpCore();

    // Set timezone offset
    int16_t lpCoreTimeZoneOff = timeZoneOffset / 60;
    memcpy(&rtc_mem->custom[8], &lpCoreTimeZoneOff, 2);
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

bool runLpCore()
{
#if LP_CORE_TEST_ENABLED
    ulp_lp_core_cfg_t cfg = {
        .wakeup_source = ULP_LP_CORE_WAKEUP_SOURCE_LP_TIMER,
        .lp_timer_sleep_duration_us = uint32_t(2 * 1000000),
    };
#else
    ulp_lp_core_cfg_t cfg = {
        .wakeup_source = ULP_LP_CORE_WAKEUP_SOURCE_LP_TIMER,
        .lp_timer_sleep_duration_us = uint32_t((1 + (60 - getCurrentSeconds())) * 1000000),
    };
#endif

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
    if (bootStatus.fromWakeup == true && willAlarmTrigger() == false)
    {
        screenForceNextFullTimeWrite = true;
        // We want it to update on it's own
        if (bootStatus.reason != wakeUpReason::ulp)
        {
            rtc_retain_mem_t *rtc_mem = bootloader_common_get_rtc_retain_mem();
            rM.wFTime.Hour = rtc_mem->custom[1];
            rM.wFTime.Minute = rtc_mem->custom[2];
            debugLog("Updated from lp core hour and minute: " + getHourMinute(rM.wFTime));
        }
        if (bootStatus.reason == wakeUpReason::ulp)
        {
            // Force a rewrite, be sure about it, the read was done previously
            readRTC();
            rM.wFTime.Minute = 255;
            rM.wFTime.Hour = 255;
        }
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
    dis->fillRect(0, 0, 200, 200, GxEPD_BLACK);
    disUp(true);
    initRTC();
    debugLog("Current unix time: " + String(getUnixTime(timeRTCUTC0)));

    clearLpCoreRtcMem();
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
        return LPOG_ERROR_8_STR;
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