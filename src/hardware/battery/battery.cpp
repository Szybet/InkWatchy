#include "battery.h"
#include "rtcMem.h"

float BatteryRead() { return analogReadMilliVolts(BATT_ADC_PIN) / ADC_VOLTAGE_DIVIDER; }

#if ATCHY_VER == YATCHY
#define FALLBACK_BATTERY_VOLTAGE 4.12345
float previousGoodRead = FALLBACK_BATTERY_VOLTAGE;
#endif
float getBattVoltage()
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
        delayTask(VOLTAGE_AVG_DELAY);
    }
#else
    float sum = 0.0;
    float previousRead = 0.0;
    for (int i = 0; i < VOLTAGE_AVG_COUNT; i++)
    {
        float readed = BatteryRead();
        if ((previousRead != 0.0 && abs(previousRead - readed) > 0.4) || readed == 0.0)
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
    // debugLog("So battery voltage: " + String(batVoltFinish));
    return batVoltFinish;
}

void initBattery()
{
    if (bootStatus.fromWakeup == false || isFullMode() == false)
    {
        rM.bat.oneCheck = true;
        loopBattery();
        rM.bat.oneCheck = false;
    }
}

#if DEBUG
bool previousCharging = true;
#endif

#if ATCHY_VER == YATCHY
bool isSomethingWrong = false;
#endif
void isChargingCheck()
{
    bool previousChargingState = rM.bat.isCharging;

#if NO_CHARGING == 1 && DEBUG == 1
    rM.bat.isCharging = false;
    return;
#endif
#if ATCHY_VER == WATCHY_2 || ATCHY_VER == WATCHY_1 || ATCHY_VER == WATCHY_1_5
    if (rM.bat.curV >= BATTERY_CHARGE_VOLTAGE)
    {
        // debugLog("It's charging because of above voltage");
        rM.bat.isCharging = true;
    }
    else
    {
        // debugLog("It's charging because of below voltage");
        rM.bat.isCharging = false;
    }
    // This did not worked well, sadly
    /*
    else
    {
        float average = 0;
        int averageDivision = PREV_VOLTAGE_SIZE;
        for (int i = 0; i < PREV_VOLTAGE_SIZE; i++)
        {
            if (rM.bat.prevV[i] != 0.0 && i != rM.bat.prevVPos)
            {
                average = average + rM.bat.prevV[i];
            }
            else
            {
                averageDivision = averageDivision - 1;
            }
        }
        average = average / averageDivision;
        if (average == 0.0)
        {
            rM.bat.isCharging = false;
            return;
        }
        debugLog("The average previous voltage battery is: " + String(average));
        if (rM.bat.prevV[rM.bat.prevVPos] - average >= BATTERY_CHARGE_DETECTION_DIFFERENCE || average >= rM.bat.charV)
        {
            rM.bat.isCharging = true;
        }
        else
        {
            rM.bat.isCharging = false;
        }
    }
    */
#elif ATCHY_VER == WATCHY_3
    // Looks like bad code? go to definition of the pin
    if (analogRead(CHRG_STATUS_PIN) > 4000)
    {
        rM.bat.isCharging = true;
    }
    else
    {
        rM.bat.isCharging = false;
    }
#elif ATCHY_VER == YATCHY
    /*
    In main.cpp for testing:
    loopBattery();
    isChargingCheck();
    debugLog("Yatchy gpio: " + uint16ToBinaryString(rM.gpioExpander.readRegister(GPIO)));
    delayTask(2000);
    return;
    Sadly not anymore, outdated:
    Charging: Before is 0 and after is 0 (so is_falling on this pin for interrupt so we can detect charging)
    Discharging: Before is 1 and after is 0
    Fully charged: Before is 1 and after is 1

    Basically now we can't detect if its between Hi-Z and L
    */

    if (xTaskGetCurrentTaskHandle() == buttonTask)
    {
        // It was caused by an interrupt, so we wait for 5V to go down
        if(rM.previousFiveVolt == true) {
            debugLog("Delaying because of button task interrupt call in charging check");
            delayTask(400);
        }
    }

    rM.gpioExpander.setInterrupt(MCP_STAT_IN, false); // Turn off interrupt
    bool fiveVolt = rM.gpioExpander.digitalRead(MCP_5V);
    rM.gpioExpander.setPinState(MCP_STAT_OUT, false);
    delayTask(5);
    bool statInStateBefore = rM.gpioExpander.digitalRead(MCP_STAT_IN);
    rM.gpioExpander.setPinState(MCP_STAT_OUT, true);
    delayTask(5);
    bool statInStateAfter = rM.gpioExpander.digitalRead(MCP_STAT_IN);
#if DEBUG && true == true
    debugLog("Executed isCharging");
    debugLog("fiveVolt: " + String(fiveVolt));
    debugLog("statInStateBefore: " + String(statInStateBefore));
    debugLog("statInStateAfter: " + String(statInStateAfter));
#endif
    if (rM.previousStatInStateBefore != statInStateBefore || rM.previousStatInStateAfter != statInStateAfter || rM.previousFiveVolt != fiveVolt)
    {
        debugLog("Charging state changed for gpio expander");
        bool isFine = true;
        // Only charging
        if (statInStateBefore == 0 && statInStateAfter == 0 && fiveVolt == true)
        {
            rM.bat.isCharging = true;
            rM.bat.isFullyCharged = false;
        } // Fully charged
        else if (statInStateBefore == 1 && statInStateAfter == 1 && fiveVolt == true)
        {
            rM.bat.isCharging = true;
            if (rM.bat.curV > 4.10)
            {
                debugLog("Its charged but voltage doesn't sum up");
                rM.bat.isFullyCharged = true;
                statInStateBefore = 0;
                statInStateAfter = 0;
            }
        } // Not charging
        else if (fiveVolt == false)
        {
            rM.bat.isCharging = false;
            rM.bat.isFullyCharged = false;
#if DEBUG
            if (!(statInStateBefore == 0 && statInStateAfter == 0))
            {
                debugLog("ERROR: Something is wrong with your charging IC, maybe, probably");
            }
#endif
        }
        else
        {
            if (isSomethingWrong == false)
            {
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
            debugLog("rM.bat.isCharging: " + BOOL_STR(rM.bat.isCharging));
            debugLog("rM.bat.isFullyCharged: " + BOOL_STR(rM.bat.isCharging));
#endif
#if BATTERY_RGB_DIODE
            if (rM.bat.isFullyCharged == true)
            {
                debugLog("Setting color to fully charged!");
                setRgb(BATTERY_CHARGED_COLOR);
            }
            else if (rM.bat.isCharging == true)
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
            debugLog("It's still not fine. Something is wrong. Let's say it's not charging at all then");
            rM.bat.isCharging = false;
            rM.bat.isFullyCharged = false;
#if BATTERY_RGB_DIODE
            setRgb(BATTERY_DISCHARGING_COLOR, true, 500);
#endif
        }
        rM.previousFiveVolt = fiveVolt;
        rM.previousStatInStateBefore = statInStateBefore;
        rM.previousStatInStateAfter = statInStateAfter;
    }

    // debugLog("Turning on interrupt back on");
    delayTask(10);
    rM.gpioExpander.setInterrupt(MCP_STAT_IN, true); // Turn on interrupt
#endif
#if DEBUG && true == false
    if (rM.bat.isCharging != previousCharging)
    {
        previousCharging = rM.bat.isCharging;
        debugLog("Charging is now: " + BOOL_STR(rM.bat.isCharging));
    }
#endif

#if DEBUG_MENUS
    if (rM.bat.isCharging == false && previousChargingState == true)
    {
        if (fsGetString(CONF_UNIX_LAST_SYNC, "") != "")
        {
            readRTC();
            fsSetString(CONF_UNIX_LAST_CHARGE, String(getUnixTime(timeRTCUTC0)));
        }
    }
#endif
}

uint64_t batteryReadMillis = 0;
void loopBattery(bool forceRead)
{
    // debugLog("aaa: " + String(batteryReadMillis - millisBetter()));
    if (forceRead == false && millisBetter() - batteryReadMillis < 2000)
    {
        return;
    }
    batteryReadMillis = millisBetter();
    /*
    else if(forceRead == false) {
        debugLog("Time passed, force read false, reading battery");
    }
    */

    rM.bat.curV = getBattVoltage();
    if (abs(rM.bat.prevVOne - rM.bat.curV) > BAT_MINIMAL_DIFFERENCE || rM.bat.oneCheck == true)
    {
        debugLog("Voltage changed changed, doing things...");
#if BATTERY_TIME_DROP
        if (rM.bat.curV < BATTERY_TIME_DROP_VOLTAGE)
        {
            if (fsFileExists("/bat_dump.txt") == false)
            {
                fsSetString("bat_dump.txt", String(getUnixTime(timeRTCUTC0)) + " at voltage " + String(rM.bat.curV), "/");
            }
        }
#endif
        debugLog("prevOne: " + String(rM.bat.prevVOne) + " curV: " + String(rM.bat.curV));
        rM.bat.prevVOne = rM.bat.curV;
        // debugLog("rM.bat.curV: " + String(rM.bat.curV));
        int batPercTemp = ((rM.bat.curV - BATTERY_MIN_VOLTAGE) / (BATTERY_MAX_VOLTAGE - BATTERY_MIN_VOLTAGE)) * 100.0;
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
        rM.bat.percentage = batPercTemp;
        debugLog("Current voltage percentage: " + String(rM.bat.percentage));

#if DEBUG && true == 0
        debugLog("Dumping previous voltages:");
        for (int i = 0; i < PREV_VOLTAGE_SIZE; i++)
        {
            debugLog(String(i) + " - " + String(rM.bat.prevV[i]));
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
    if (reasonForVoltageSpikes() == false)
    {
        if (rM.isBatterySaving == false && rM.bat.percentage < POWER_SAVING_AFTER)
        {
            debugLog("Turning on power settings");
            rM.isBatterySaving = true;
            rM.disableAllVibration = true;
            rM.disableWakeUp = true;
        }
        else if (rM.isBatterySaving == true && rM.bat.percentage + POWER_SAVING_OFF_AFTER > POWER_SAVING_AFTER && rM.bat.isCharging == false)
        {
            debugLog("Turning off power settings");
            rM.isBatterySaving = false;
            loadAllStorage();
        }
    }
}

bool reasonForVoltageSpikes()
{
    if (isWifiTaskCheck() == true)
    {
        return true;
    }
    wifiStatusSimple wifi = wifiStatusWrap();
    if (wifi != WifiOff)
    {
        return true;
    }
    if (motorTaskRunning == true)
    {
        return true;
    }
    return false;
}

#if DEBUG
void dumpBattery()
{
    loopBattery();
    debugLog("Battery voltage: " + String(rM.bat.curV));
}

void dumpBatteryScreen(void *parameter)
{
    while (true)
    {
        dis->setFont(getFont("dogicalpixel4"));
        dis->setTextSize(1);
        dis->fillRect(80, 80, 40, 40, GxEPD_WHITE);
        dis->setCursor(85, 100);
        dis->print(String(getBatteryVoltage()));
        resetSleepDelay();
    }
}
#endif