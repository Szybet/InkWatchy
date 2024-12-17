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
    // Temp
#if TEMP_CHECKS_ENABLED
    uint8_t fixCounts;
    float initialTemp;
    float previousTemp;
#endif
};

extern rtcMem rM;