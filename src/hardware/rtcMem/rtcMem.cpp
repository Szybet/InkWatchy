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
    .SBMA = StableBMA {}, // Class
    .initedAxc = false,
    .stepsInited = false,
    .stepDay = 0,
    // Display
    .display = GxEPD2_154_D67(EPD_CS, EPD_DC, EPD_RESET, EPD_BUSY),
};