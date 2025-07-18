#include "lpCore.h"
#include "rtcMem.h"

#if LP_CORE

bool screenForceNextFullTimeWrite = false; // Make the watchface update the whole time bar
bool screenTimeChanged = false;            // If the watchface has written time (minutes / hours) to the screen

#define LP_CORE_SCREEN_X 8
#define LP_CORE_SCREEN_Y 2
#define LP_CORE_SCREEN_W 185
#define LP_CORE_SCREEN_H 56

struct customRtcData
{
    uint8_t debug_message;
    uint8_t hour;
    uint8_t minute;
    bool dot;
    uint32_t alarm_unix;
    int16_t timezone_offset;
    uint32_t stupid_unix;
};

customRtcData *customRtcDataGet()
{
    rtc_retain_mem_t *rtc_mem = bootloader_common_get_rtc_retain_mem();
    return (customRtcData *)(rtc_mem->custom);
}

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
    debugLog("Stop lp core called");
    if (bootStatus.reason == ulp)
    {
        delayTask(100);
    }
    ulp_lp_core_stop();
    delayTask(10);
    if (bootStatus.reason == ulp)
    {
        delayTask(100);
    }
    deInitRtcGpio();
    delayTask(10);
    if (bootStatus.reason == ulp)
    {
        delayTask(300);
    }
    // Die
    ulp_lp_core_stop();
}

void setAlarmForLpCore()
{
#if INK_ALARMS
    customRtcData *customRtcData = customRtcDataGet();
    customRtcData->alarm_unix = (uint32_t)rM.nextAlarm;
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
    customRtcData *customRtcData = customRtcDataGet();
    customRtcData->timezone_offset = int16_t(timeZoneOffset / 60);
}

void loadLpCore(String lp_core_program)
{
    debugLog("Loading lp core");
    // Load it from littlefs first
    bufSize bin = fsGetBlob(lp_core_program, "/other/");

    if (bin.size <= 0)
    {
        debugLog("Failed to load lp core");
        assert(false);
    }
    else
    {
        debugLog("Lp core size: " + String(bin.size));
    }

    esp_err_t err = ulp_lp_core_load_binary(bin.buf, bin.size);
    if (err != ESP_OK)
    {
        debugLog("Failed to load lp core: " + String(esp_err_to_name(err)));
        assert(false);
    }
}

uint64_t microsTillNextMin()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    uint64_t seconds = tv.tv_sec % 60;
    uint64_t micros = tv.tv_usec;
    return (59 - seconds) * 1000000 + (1000000 - micros);
}

void runLpCore()
{
    if (screenTimeChanged == true)
    {
        clearLpCoreRtcMem();
    }

    readRTC();
#if LP_CORE_TEST_ENABLED
    ulp_lp_core_cfg_t cfg = {
        .wakeup_source = ULP_LP_CORE_WAKEUP_SOURCE_LP_TIMER,
        .lp_timer_sleep_duration_us = uint32_t(2 * 1000000),
    };
#else
    uint64_t micros = microsTillNextMin();
    debugLog("Micros till next minute: " + String(micros) + " while seconds is: " + String(getCurrentSeconds()) + " so it will wake up in: " + String(micros / 1000000));
#if DEBUG
    if (micros > UINT32_MAX)
    {
        debugLog("BIG PROBLEM UINT32_MAX");
        assert(0);
    }
#endif
    ulp_lp_core_cfg_t cfg = {
        .wakeup_source = ULP_LP_CORE_WAKEUP_SOURCE_LP_TIMER,
        .lp_timer_sleep_duration_us = uint32_t(micros),
    };
#endif

    uint32_t stupidUnix = getUnixTime(timeRTCUTC0) + ceil(float(micros) / 1000000.0);
    /*
    struct tm *tmInfo = gmtime((time_t *)&stupidUnix);
    debugLog("tmInfo->tm_min: " + String(tmInfo->tm_min));
    if (tmInfo->tm_min > 55)
    {
        uint8_t c = 0;
        while (tmInfo->tm_min > 57)
        {
            tmInfo->tm_min = tmInfo->tm_min + 1;
            c = c + 1;
            if (tmInfo->tm_min >= 60)
            {
                tmInfo->tm_min = tmInfo->tm_min - 60;
            }
        }
        stupidUnix = stupidUnix + c;
        debugLog("Stupid unix adjustment: " + String(c) + " THIS SHOULD NOT HAPPEN");
    }
    */

    debugLog("Time given to lp core is: " + String(stupidUnix));
    customRtcData *customRtcData = customRtcDataGet();
    customRtcData->stupid_unix = stupidUnix;

    debugLog("shared_mem->sleep_duration_ticks: " + String(ulp_lp_core_memory_shared_cfg_get()->sleep_duration_ticks));
    debugLog("shared_mem->sleep_duration_us: " + String(ulp_lp_core_memory_shared_cfg_get()->sleep_duration_us));

    debugLog("Running lp core");
    // It will fail to run if there was lp core already running
    esp_err_t err = ulp_lp_core_run(&cfg);
    if (err != ESP_OK)
    {
        debugLog("Failed to run lp core: " + String(esp_err_to_name(err)));
        assert(false);
    }

    debugLog("shared_mem->sleep_duration_ticks: " + String(ulp_lp_core_memory_shared_cfg_get()->sleep_duration_ticks));
    debugLog("shared_mem->sleep_duration_us: " + String(ulp_lp_core_memory_shared_cfg_get()->sleep_duration_us));

    // Debugging a case when lp core wakes up too fast?
    // https://github.com/espressif/esp-idf/blob/bfe5caf58f742fd35c023335f475114a5b88761e/examples/system/ulp/lp_core/lp_timer_interrupt/main/lp_interrupts_main.c#L32
    delayTask(100);
}

/*
void runLpCoreNow()
{
    ulp_lp_core_cfg_t cfg = {
        .wakeup_source = ULP_LP_CORE_WAKEUP_SOURCE_HP_CPU,
    };
    ulp_lp_core_run(&cfg);
    debugLog("runLpCoreNow done");
}

#include <macros.h>
#include <rtc.h>

#define DR_REG_LP_AON_BASE                      0x600B1000
#define LP_AON_STORE1_REG (DR_REG_LP_AON_BASE + 0x4)
#define RTC_SLOW_CLK_CAL_REG                LP_AON_STORE1_REG

#define UNIX_OFFSET_CALL_LOOP 0
#define UNIX_OFFSET_SMALL_ENABLE 0
void unix_offset_call()
{
    loadLpCore("yatchy-lp-unix-cal-program.bin");
    rtc_retain_mem_t *rtc_mem = bootloader_common_get_rtc_retain_mem();
    memset(&rtc_mem->custom[4], 0, 4);
    runLpCoreNow();
    int64_t diff = 0;
#if UNIX_OFFSET_CALL_LOOP
    while (true)
    {
#endif
        uint32_t nextAlarmFromRtc = 0;
        while (nextAlarmFromRtc == 0)
        {
            memcpy(&nextAlarmFromRtc, &rtc_mem->custom[4], 4);
            readRTC();
        }
        struct timeval tv_now;
        gettimeofday(&tv_now, NULL);
        settimeofday(&tv_now, NULL);

        diff = getUnixTime(timeRTCUTC0) - nextAlarmFromRtc;
        int64_t diff2 = tv_now.tv_sec - nextAlarmFromRtc;
        debugLog("Lp core diff is: " + String(diff));
        debugLog("Lp core diff2222 is: " + String(diff2));
        int64_t fuck = esp_rtc_get_time_us() / 1000000;
        debugLog("Fuck: " + String(fuck));
        debugLog("Fuck pure pure: " + String(nextAlarmFromRtc));
        debugLog("Fuck diff is: " + String(fuck - int64_t(nextAlarmFromRtc)));
        debugLog("esp_clk_slowclk_cal_get: " + String(uint32_t(REG_READ(RTC_SLOW_CLK_CAL_REG))));
#if UNIX_OFFSET_CALL_LOOP
        delayTask(500);
    }
#endif
    stopLpCore();
#if UNIX_OFFSET_SMALL_ENABLE
    rtc_mem->custom[10] = diff;
#endif
}
*/

void initManageLpCore()
{
    if (bootStatus.fromWakeup == true && willAlarmTrigger() == false)
    {
        screenForceNextFullTimeWrite = true;
        // We want it to update on it's own
        if (bootStatus.reason != wakeUpReason::ulp)
        {
            customRtcData *customRtcData = customRtcDataGet();
            rM.wFTime.Hour = customRtcData->hour;
            rM.wFTime.Minute = customRtcData->minute;
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
    /*
    else if (bootStatus.fromWakeup == false)
    {
        // if ulp wakeup + minutes
        {
            unix_offset_call();
        }
    }
    */
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
    /*
    rtc_retain_mem_t *rtc_mem = bootloader_common_get_rtc_retain_mem();
    uint32_t nextAlarmFromRtc;
    memcpy(&nextAlarmFromRtc, &rtc_mem->custom[4], 4);
    readRTC();
    debugLog("Time from lp core is: " + String(nextAlarmFromRtc));
    int64_t t = getUnixTime(timeRTCUTC0);
    debugLog("Time in inkwatchy is: " + String(t));
    debugLog("Diff is: " + String(t - nextAlarmFromRtc));
    delayTask(100);
    */
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