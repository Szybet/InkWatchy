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
    int percentage;
};

extern RTC_DATA_ATTR batteryInfo bat;
extern RTC_DATA_ATTR bool isBatterySaving;

double getBatteryVoltage();

void loopBattery();
void initBattery();
void loopPowerSavings();

uint32_t analogReadMilliVolts(uint8_t pin);


#if DEBUG
void dumpBattery();
#endif

#endif
