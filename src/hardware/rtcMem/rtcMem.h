#pragma once

#include "defines.h"

struct rtcMem
{
    // Battery
    batteryInfo bat;
    bool isBatterySaving : 1;
    // Accelerometer
#if AXC_ENABLED
    StableBMA SBMA; // Class
    bool initedAxc : 1;
    bool stepsInited : 1;
    uint8_t stepDay; // TODO: too low
#endif
    // Display
    GxEPD2_BW<GxEPD2_154_D67, GxEPD2_154_D67::HEIGHT> display;
    uint8_t updateCounter;
// Mcp23018
#if ATCHY_VER == YATCHY
    mcp23018 gpioExpander;
#endif
    // RTC
    SmallRTC SRTC;
    char posixTimeZone[POSIX_TIMEZONE_MAX_LENGTH];
    // Temp
#if TEMP_CHECKS_ENABLED
    uint8_t fixCounts;
    float initialTemp;
    float previousTemp;
#endif
    // Wifi logic
    uint64_t lastSyncUnix;
    uint64_t lastTryUnix;
// Ntp
// Time drift correction
#if TIME_DRIFT_CORRECTION
    uint64_t driftStartUnix;
#endif
// Watchdog
#if WATCHDOG_TASK
    bool everythingIsFine : 1;
#endif
    // Power settings
    bool disableAllVibration : 1;
    bool disableWakeUp : 1;
};

extern rtcMem rM;