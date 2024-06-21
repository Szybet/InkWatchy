#ifndef BATTERY_H
#define BATTERY_H

#include "defines/defines.h"
#include <SmallRTC.h>

#define PREV_VOLTAGE_SIZE 3
struct batteryInfo {
    float curV;
    float minV;
    float critV;
    float maxV;
    float charV;
    float prevV[PREV_VOLTAGE_SIZE];
    uint8_t prevVPos;
    bool isCharging;
    uint8_t percentage;
    float prevVOne;
    bool oneCheck; // Force check once even if no voltage changed
};

extern batteryInfo bat;
extern bool isBatterySaving;

double getBatteryVoltage();

void loopBattery();
void initBattery();
void loopPowerSavings();
bool reasonForVoltageSpikes();

uint32_t analogReadMilliVolts(uint8_t pin);


#if DEBUG
void dumpBattery();
void dumpBatteryScreen(void *parameter);
#endif

#endif
