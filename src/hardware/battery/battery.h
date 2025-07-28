#pragma once

#include "defines.h"

struct batteryInfo
{
    float curV;
    bool isCharging : 1;
    bool isFullyCharged : 1; // Only on the Yatchy really, v3 people are free to implement it
    uint8_t percentage;
    float prevVOne;
    bool oneCheck : 1; // Force check once even if no voltage changed
};

float BatteryRead();
float getBatteryVoltage();

void loopBattery(bool forceRead = true);
void isChargingCheck();
void initBattery();
void loopPowerSavings();
bool reasonForVoltageSpikes();

uint32_t analogReadMilliVolts(uint8_t pin);

#if DEBUG
void dumpBattery();
void dumpBatteryScreen(void *parameter);
#endif
