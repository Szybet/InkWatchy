#include "hardware.h"
#include "battery.h"
#include "../functions.h"

batteryInfo bat;

float BatteryRead() { return analogReadMilliVolts(SRTC.getADCPin()) / 500.0f; } // Battery voltage goes through a 1/2 divider.

double getBatteryVoltage()
{
    double sum = 0;

    for (int i = 0; i < VOLTAGE_AVG_COUNT; i++)
    {
        sum += BatteryRead() - 0.0125;
    }

    return sum / VOLTAGE_AVG_COUNT;
}

void initBattery()
{
#if GSR_MINIMUM_BATTERY_VOLTAGE
    bat.minV = SRTC.getRTCBattery(false);
    bat.maxV = BATTERY_MAX_VOLTAGE;
    bat.critV = SRTC.getRTCBattery(true);
#else
    bat.minV = BATTERY_MIN_VOLTAGE;
    bat.maxV = BATTERY_MAX_VOLTAGE;
    bat.critV = BATTERY_CRIT_VOLTAGE;
#endif
    bat.curV = getBatteryVoltage();
}

void loopBattery()
{
    bat.curV = getBatteryVoltage();
}
