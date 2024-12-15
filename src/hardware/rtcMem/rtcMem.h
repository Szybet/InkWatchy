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
    uint8_t stepDay;
};

extern rtcMem rM;