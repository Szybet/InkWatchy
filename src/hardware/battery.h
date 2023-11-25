#ifndef BATTERY_H
#define BATTERY_H

#include "defines/defines.h"
#include <SmallRTC.h>

struct batteryInfo {
    float curV;
    float minV;
    float critV;
    float maxV;
    bool isCharging;
};

extern batteryInfo bat;

double getBatteryVoltage();

void loopBattery();
void initBattery();

uint32_t analogReadMilliVolts(uint8_t pin);
#endif