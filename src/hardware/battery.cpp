#include "hardware.h"
#include "battery.h"
#include "../functions.h"

RTC_DATA_ATTR batteryInfo bat;
RTC_DATA_ATTR bool isBatterySaving = false;

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
    bat.maxV = BATTERY_CHARGE_VOLTAGE;
    bat.critV = SRTC.getRTCBattery(true);
    bat.charV = BATTERY_CHARGE_VOLTAGE;
#else
    bat.minV = BATTERY_MIN_VOLTAGE;
    bat.critV = BATTERY_CRIT_VOLTAGE;
    if (BAD_BATTERY == false)
    {
        bat.maxV = BATTERY_MAX_VOLTAGE;
        bat.charV = BATTERY_CHARGE_VOLTAGE;
    }
    else
    {
        bat.maxV = BAD_BATTERY_MAX_VOLTAGE;
        bat.charV = BAD_BATTERY_CHARGE_VOLTAGE;
    }
#endif
    bat.prevVPos = 0;
    for (int i = 0; i < PREV_VOLTAGE_SIZE; i++)
    {
        bat.prevV[i] = 0.0;
    }

    bat.oneCheck = true;
    loopBattery();
    bat.oneCheck = false;
}

#if DEBUG
RTC_DATA_ATTR bool previousCharging = true;
#endif
void isChargingCheck()
{
    if (bat.curV >= bat.charV)
    {
        // debugLog("It's charging because of above voltage");
        bat.isCharging = true;
    }
    else
    {
        float average = 0;
        int averageDivision = PREV_VOLTAGE_SIZE;
        for (int i = 0; i < PREV_VOLTAGE_SIZE; i++)
        {
            if (bat.prevV[i] != 0.0 && i != bat.prevVPos)
            {
                average = average + bat.prevV[i];
            }
            else
            {
                averageDivision = averageDivision - 1;
            }
        }
        average = average / averageDivision;
        if (average == 0.0)
        {
            bat.isCharging = false;
            return;
        }
        debugLog("The average previous voltage battery is: " + String(average));
        if (bat.prevV[bat.prevVPos] - average >= BATTERY_CHARGE_DETECTION_DIFFERENCE || average >= bat.charV)
        {
            bat.isCharging = true;
        }
        else
        {
            bat.isCharging = false;
        }
    }
#if DEBUG
    if (bat.isCharging != previousCharging)
    {
        previousCharging = bat.isCharging;
        debugLog("Charging is now: " + BOOL_STR(bat.isCharging));
    }
#endif
}

void loopBattery()
{
    bat.curV = getBatteryVoltage();
    if (abs(bat.prevVOne - bat.curV) > BAT_MINIMAL_DIFFERENCE || bat.oneCheck == true)
    {
        debugLog("Voltage changed changed, doing things...");
        debugLog("prevOne: " + String(bat.prevVOne) + " curV: " + String(bat.curV));
        bat.prevVOne = bat.curV;
        bat.prevV[bat.prevVPos] = bat.curV;
        // debugLog("bat.curV: " + String(bat.curV));
        // debugLog("bat.charV: " + String(bat.charV));
        bat.percentage = ((bat.curV - bat.minV) / (bat.maxV - bat.minV)) * 100.0;
        if (bat.percentage > 100)
        {
            // Charging
            bat.percentage = 100;
        }

#if DEBUG && true == 0
        debugLog("Dumping previous voltages:");
        for (int i = 0; i < PREV_VOLTAGE_SIZE; i++)
        {
            debugLog(String(i) + " - " + String(bat.prevV[i]));
        }
#endif

        isChargingCheck();
        bat.prevVPos = bat.prevVPos + 1;
        if (bat.prevVPos >= PREV_VOLTAGE_SIZE)
        {
            bat.prevVPos = 0;
        }

        loopPowerSavings();
    }
}

void loopPowerSavings()
{
    if (isBatterySaving == false && bat.percentage < POWER_SAVING_AFTER && reasonForVoltageSpikes() == false)
    {
        debugLog("Turning on power settings");
        isBatterySaving = true;
        disableAllVibration = true;
        disableWakeUp = true;
    }
    else if (isBatterySaving == true && bat.percentage + POWER_SAVING_OFF_AFTER > POWER_SAVING_AFTER && bat.isCharging == false)
    {
        debugLog("Turning off power settings");
        isBatterySaving = false;
        loadAllStorage();
    }
}

bool reasonForVoltageSpikes()
{
    // use OR here with other functions
    return isWifiTaskCheck();
}

#if DEBUG
void dumpBattery()
{
    loopBattery();
    debugLog("Battery voltage: " + String(bat.curV));
}

void dumpBatteryScreen(void *parameter)
{
    while (true)
    {
        display.setFont(&dogicapixel4pt7b);
        display.setTextSize(1);
        display.fillRect(80, 80, 40, 40, GxEPD_WHITE);
        display.setCursor(85, 100);
        display.print(String(getBatteryVoltage()));
        display.display(true);
        resetSleepDelay();
    }
}
#endif
