#pragma once

#include "defines.h"

struct rtcMem {
    // Battery
    batteryInfo bat;
    bool isBatterySaving : 1;
    // Accelerometer
    StableBMA SBMA; // Class
    bool initedAxc : 1;
    bool stepsInited : 1;
    uint8_t stepDay; // TODO: too low
    // Display
    GxEPD2_BW<GxEPD2_154_D67, GxEPD2_154_D67::HEIGHT> display;
};

extern rtcMem rM;