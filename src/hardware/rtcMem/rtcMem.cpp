#include "rtcMem.h"

RTC_DATA_ATTR rtcMem rM = {
    // Battery
    .bat = {
        .curV = 0.0,
        .charV = 0.0,
        .isCharging = false,
        .isFullyCharged = false,
        .percentage = 0,
        .prevVOne = 0.0,
        .oneCheck = false},
    .isBatterySaving = false,
// Acc
#if AXC_ENABLED
    .SBMA = StableBMA{}, // Class
    .initedAxc = false,
    .stepsInited = false,
    .stepDay = 0,
#endif
    // Display
    .display = GxEPD2_154_D67(EPD_CS, EPD_DC, EPD_RESET, EPD_BUSY),
    .updateCounter = 0,
// Mcp23018
#if ATCHY_VER == YATCHY
    .gpioExpander = mcp23018{},
#endif
    // RTC
    .SRTC = SmallRTC{}, // Class
    .posixTimeZone = TIMEZONE_POSIX,
// Temp
#if TEMP_CHECKS_ENABLED
    .fixCounts = 0,
    .initialTemp = 0.0,
    .previousTemp = 0.0,
#endif
    // Wifi logic
    .lastSyncUnix = 0,
    .lastTryUnix = 0,
// Ntp
// Time drift correction
#if TIME_DRIFT_CORRECTION
    .driftStartUnix = 0,
#endif
// Watchdog
#if WATCHDOG_TASK
    .everythingIsFine = true,
#endif
    // Power settings
    .disableAllVibration = false,
    .disableWakeUp = false,
};