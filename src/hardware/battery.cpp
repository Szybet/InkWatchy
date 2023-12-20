#include "hardware.h"
#include "battery.h"
#include "../functions.h"

batteryInfo RTC_DATA_ATTR bat;

float BatteryRead() { return analogReadMilliVolts(SRTC.getADCPin()) / 500.0f; } // Battery voltage goes through a 1/2 divider.

double getBatteryVoltage()
{
    double sum = 0;

    for (int i = 0; i < VOLTAGE_AVG_COUNT; i++)
    {
        sum += BatteryRead() - 0.0125;
        delay(VOLTAGE_AVG_DELAY);
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
    if (bat.curV > bat.maxV)
    {
        bat.isCharging = true;
    }
    else
    {
        bat.isCharging = false;
    }
}

void loopBattery()
{
    bat.curV = getBatteryVoltage();
    bat.percentage = ((bat.curV - bat.minV) / (bat.maxV - bat.minV)) * 100.0;
    if(bat.percentage > 100) {
        // Charging
        bat.percentage = 100;
    }
    if (bat.curV > bat.maxV)
    {
        bat.isCharging = true;
    }
    else
    {
        bat.isCharging = false;
    }
}

#if DEBUG
void dumpBattery() {
    loopBattery();
    debugLog("Battery voltage: " + String(bat.curV));
}
#endif
