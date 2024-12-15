#pragma once

#include "defines.h"

#define PREV_VOLTAGE_SIZE 3
struct batteryInfo {
    float curV;
    float minV;
    float critV;
    float maxV;
    float charV;
    bool isCharging;
    bool isFullyCharged; // Only on the Yatchy really, v3 people are free to implement it
    uint8_t percentage;
    float prevVOne;
    bool oneCheck; // Force check once even if no voltage changed
};

extern batteryInfo bat;
extern bool isBatterySaving;

float BatteryRead();
float getBatteryVoltage();

void loopBattery();
void isChargingCheck();
void initBattery();
void loopPowerSavings();
bool reasonForVoltageSpikes();

uint32_t analogReadMilliVolts(uint8_t pin);


#if DEBUG
void dumpBattery();
void dumpBatteryScreen(void *parameter);
#endif

