#include "battery.h"

RTC_DATA_ATTR batteryInfo bat;
RTC_DATA_ATTR bool isBatterySaving = false;

float BatteryRead() { return analogReadMilliVolts(BATT_ADC_PIN) / ADC_VOLTAGE_DIVIDER; }

#if ATCHY_VER == YATCHY
#define FALLBACK_BATTERY_VOLTAGE 4.12345
float previousGoodRead = FALLBACK_BATTERY_VOLTAGE;
#endif
float getBatteryVoltage()
{
#if NO_CHARGING == true && DEBUG == true
    return 4.0;
#endif
    uint8_t readedTimes = 0;
#if ATCHY_VER != YATCHY
    float sum = 0.0;
    for (int i = 0; i < VOLTAGE_AVG_COUNT; i++)
    {
        sum += BatteryRead();
        readedTimes = readedTimes + 1;
        delay(VOLTAGE_AVG_DELAY);
    }
#else
    float sum = 0.0;
    float previousRead = 0.0;
    for (int i = 0; i < VOLTAGE_AVG_COUNT; i++)
    {
        float readed = BatteryRead();
        if ((previousRead != 0.0 && abs(previousRead - readed) > 0.6) || readed == 0.0)
        {
            if (previousRead > 3.3)
            {
                break;
            }
            else
            {
                if (previousGoodRead == FALLBACK_BATTERY_VOLTAGE)
                {
                    debugLog("Returning fallback battery voltage");
                }
                return previousGoodRead;
            }
        }
        sum += readed;
        readedTimes = readedTimes + 1;
        delayTask(VOLTAGE_AVG_DELAY);
    }

#endif
    float batVoltFinish = sum / readedTimes;
    debugLog("So battery voltage: " + String(batVoltFinish));
    return batVoltFinish;
}

void initBattery()
{
    if (bootStatus.fromWakeup == false)
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
        bat.prevVOne = 0.0;
    }

    if (bootStatus.fromWakeup == false || bootStatus.reason == wakeUpReason::rtc || bootStatus.reason == wakeUpReason::ulp)
    {
        bat.oneCheck = true;
        loopBattery();
        bat.oneCheck = false;
    }
}

#if DEBUG
RTC_DATA_ATTR bool previousCharging = true;
#endif

#if ATCHY_VER == YATCHY
RTC_DATA_ATTR bool previousStatInStateBefore = false;
RTC_DATA_ATTR bool previousStatInStateAfter = true; // There is no such configuration, so it will always trigger at least once
RTC_DATA_ATTR bool previousFiveVolt = false;        // false because it will be true after flashing
bool isSomethingWrong = false;
#endif
void isChargingCheck()
{
#if NO_CHARGING == 1 && DEBUG == 1
    bat.isCharging = false;
    return;
#endif
#if ATCHY_VER == WATCHY_2 || ATCHY_VER == WATCHY_1 || ATCHY_VER == WATCHY_1_5
    if (bat.curV >= bat.charV)
    {
        // debugLog("It's charging because of above voltage");
        bat.isCharging = true;
    }
    else
    {
        // debugLog("It's charging because of below voltage");
        bat.isCharging = false;
    }
    // This did not worked well, sadly
    /*
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
    */
#elif ATCHY_VER == WATCHY_3
    // Looks like bad code? go to definition of the pin
    if (analogRead(CHRG_STATUS_PIN) > 4000)
    {
        bat.isCharging = true;
    }
    else
    {
        bat.isCharging = false;
    }
#elif ATCHY_VER == YATCHY
    /*
    In main.cpp for testing:
    loopBattery();
    isChargingCheck();
    debugLog("Yatchy gpio: " + uint16ToBinaryString(gpioExpander.readRegister(GPIO)));
    delayTask(2000);
    return;
    Sadly not anymore, outdated:
    Charging: Before is 0 and after is 0 (so is_falling on this pin for interrupt so we can detect charging)
    Discharging: Before is 1 and after is 0
    Fully charged: Before is 1 and after is 1

    Basically now we can't detect if its between Hi-Z and L
    */
    gpioExpander.setInterrupt(MCP_STAT_IN, false); // Turn off interrupt
    bool fiveVolt = gpioExpander.digitalRead(MCP_5V);
    gpioExpander.setPinState(MCP_STAT_OUT, false);
    delayTask(5);
    bool statInStateBefore = gpioExpander.digitalRead(MCP_STAT_IN);
    gpioExpander.setPinState(MCP_STAT_OUT, true);
    delayTask(5);
    bool statInStateAfter = gpioExpander.digitalRead(MCP_STAT_IN);
#if DEBUG && true == false
    debugLog("Executed isCharging");
    debugLog("fiveVolt: " + String(fiveVolt));
    debugLog("statInStateBefore: " + String(statInStateBefore));
    debugLog("statInStateAfter: " + String(statInStateAfter));
#endif
    if (previousStatInStateBefore != statInStateBefore || previousStatInStateAfter != statInStateAfter || previousFiveVolt != fiveVolt)
    {
        debugLog("Charging state changed for gpio expander");
        bool isFine = true;
        // Only charging
        if (statInStateBefore == 0 && statInStateAfter == 0 && fiveVolt == true)
        {
            bat.isCharging = true;
            bat.isFullyCharged = false;
        } // Fully charged
        else if (statInStateBefore == 1 && statInStateAfter == 1 && fiveVolt == true)
        {
            bat.isCharging = false;
            bat.isFullyCharged = true;
        } // Not charging
        else if (statInStateBefore == 0 && statInStateAfter == 0 && fiveVolt == false)
        {
            bat.isCharging = false;
            bat.isFullyCharged = false;
        }
        else
        {
            if(isSomethingWrong == false) {
                debugLog("Something is messed up with charging detection, trying again soon");
                isSomethingWrong = true;
                delayTask(100);
                isChargingCheck();
                return;
            }
            debugLog("SOMETHING IS WRONG WITH CHARGING");
            isSomethingWrong = false;
            isFine = false;
        }
        if (isFine == true)
        {
            isSomethingWrong = false;
#if DEBUG && true == true
            debugLog("bat.isCharging: " + BOOL_STR(bat.isCharging));
            debugLog("bat.isFullyCharged: " + BOOL_STR(bat.isCharging));
#endif
#if BATTERY_RGB_DIODE
            if (bat.isFullyCharged == true)
            {
                debugLog("Setting color to fully charged!");
                setRgb(BATTERY_CHARGED_COLOR);
            }
            else if (bat.isCharging == true)
            {
                debugLog("Setting color to charging!");
                setRgb(BATTERY_CHARGING_COLOR);
            }
            else
            {
                debugLog("Setting color to discharging!");
                setRgb(BATTERY_DISCHARGING_COLOR, true, 1000);
            }
#endif
        }
        else
        {
#if BATTERY_RGB_DIODE
            setRgb(IwNone);
#endif
        }
        previousFiveVolt = fiveVolt;
        previousStatInStateBefore = statInStateBefore;
        previousStatInStateAfter = statInStateAfter;
    }
    // debugLog("Turning on interrupt back on");
    delayTask(10);
    gpioExpander.setInterrupt(MCP_STAT_IN, true); // Turn on interrupt
#endif
#if DEBUG && true == false
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
#if BATTERY_TIME_DROP
        if (bat.curV < BATTERY_TIME_DROP_VOLTAGE)
        {
            if (fsFileExists("/bat_dump.txt") == false)
            {
                fsSetString("bat_dump.txt", String(getUnixTime(timeRTCUTC0)) + " at voltage " + String(bat.curV), "/");
            }
        }
#endif
        debugLog("prevOne: " + String(bat.prevVOne) + " curV: " + String(bat.curV));
        bat.prevVOne = bat.curV;
        // debugLog("bat.curV: " + String(bat.curV));
        // debugLog("bat.charV: " + String(bat.charV));
        int batPercTemp = ((bat.curV - bat.minV) / (bat.maxV - bat.minV)) * 100.0;
        debugLog("Calculated battery percentage, raw: " + String(batPercTemp));
        if (batPercTemp > 100)
        {
            // Charging for sure
            batPercTemp = 100;
        }
        if (batPercTemp < 0)
        {
            batPercTemp = 0;
        }
        bat.percentage = batPercTemp;
        debugLog("Current voltage percentage: " + String(bat.percentage));

#if DEBUG && true == 0
        debugLog("Dumping previous voltages:");
        for (int i = 0; i < PREV_VOLTAGE_SIZE; i++)
        {
            debugLog(String(i) + " - " + String(bat.prevV[i]));
        }
#endif

        isChargingCheck();
        loopPowerSavings();
    }
#if ATCHY_VER == WATCHY_3
    isChargingCheck();
#endif
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
        display.setFont(getFont("dogicalpixel4"));
        display.setTextSize(1);
        display.fillRect(80, 80, 40, 40, GxEPD_WHITE);
        display.setCursor(85, 100);
        display.print(String(getBatteryVoltage()));
        resetSleepDelay();
    }
}
#endif