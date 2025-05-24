#include "batteryDebug.h"
#include "rtcMem.h"
#include "localization.h"

#if DEBUG == 1 || DEBUG_MENUS == 1

batteryInfo bDdata;

#define cursorX 0
int currentVoltageHeight;
int ChargingHeight;
int fullyChargedHeight;
int PercentageHeight;
#define batteryTextSize 1

void initBatteryDebugDisplay()
{
    debugLog("initBatteryDebugDisplay called");
    uint16_t h;
    bDdata = rM.bat;
    setFont(&FreeSansBold9pt7b);
    setTextSize(batteryTextSize);
    dis->setCursor(cursorX, 1);
    String menuName = DEBUG_MENU_BATTERY;
    getTextBounds(menuName, NULL, NULL, NULL, &h);
    maxHeight = h;
    uint16_t currentHeight = maxHeight;
    dis->setCursor(cursorX, currentHeight - 3);
    dis->print(menuName);

    dis->fillRect(0, currentHeight, dis->width(), 3, GxEPD_BLACK);
    currentHeight = currentHeight + maxHeight;

    writeLine(DEBUG_BATTERY_CURRENT_V + String(bDdata.curV), cursorX, &currentHeight);
    currentVoltageHeight = currentHeight - maxHeight;

    writeLine(DEBUG_BATTERY_MINIMUM_V + String(BATTERY_MIN_VOLTAGE), cursorX, &currentHeight);

    writeLine(DEBUG_BATTERY_MAXIMUM_V + String(BATTERY_MAX_VOLTAGE), cursorX, &currentHeight);

    writeLine(DEBUG_BATTERY_CRITICAL_V + String(BATTERY_CRIT_VOLTAGE), cursorX, &currentHeight);

    writeLine(DEBUG_BATTERY_LEVEL + String(bDdata.percentage), cursorX, &currentHeight);
    PercentageHeight = currentHeight - maxHeight;

    writeLine(DEBUG_BATTERY_CHARGING + BOOL_STR(bDdata.isCharging), cursorX, &currentHeight);
    ChargingHeight = currentHeight - maxHeight;

#if ATCHY_VER == YATCHY
    writeLine(DEBUG_BATTERY_FULLY + BOOL_STR(bDdata.isFullyCharged), cursorX, &currentHeight);
    fullyChargedHeight = currentHeight - maxHeight;
#endif

    resetSleepDelay();
    disUp(true);
}

void loopBatteryDebugDisplay()
{
    loopBattery();
    isChargingCheck();
    if (bDdata.curV > rM.bat.curV + 0.01 || bDdata.curV < rM.bat.curV - 0.01)
    {
        bDdata.curV = rM.bat.curV;
        dis->setCursor(cursorX, currentVoltageHeight);
        setTextSize(batteryTextSize);

        String battVoltageStr = String(rM.bat.curV);
        while (battVoltageStr.length() < 5)
        {
            battVoltageStr = battVoltageStr + " ";
        }

        writeTextReplaceBack(DEBUG_BATTERY_CURRENT_V + battVoltageStr, cursorX, currentVoltageHeight);
        dUChange = true;
    }
    if (bDdata.isCharging != rM.bat.isCharging)
    {
        bDdata.isCharging = rM.bat.isCharging;
        dis->setCursor(cursorX, ChargingHeight);
        setTextSize(batteryTextSize);

        String chargingStr = BOOL_STR(rM.bat.isCharging);
        while (chargingStr.length() < 6)
        {
            chargingStr = chargingStr + " ";
        }

        writeTextReplaceBack(DEBUG_BATTERY_CHARGING + chargingStr, cursorX, ChargingHeight);
        dUChange = true;
    }
#if ATCHY_VER == YATCHY
    if (bDdata.isFullyCharged != rM.bat.isFullyCharged)
    {
        bDdata.isFullyCharged = rM.bat.isFullyCharged;
        dis->setCursor(cursorX, fullyChargedHeight);
        setTextSize(batteryTextSize);

        String chargingStr = BOOL_STR(rM.bat.isCharging);
        while (chargingStr.length() < 6)
        {
            chargingStr = chargingStr + " ";
        }

        writeTextReplaceBack(DEBUG_BATTERY_FULLY + chargingStr, cursorX, ChargingHeight);
        dUChange = true;
    }
#endif
    if (bDdata.percentage != rM.bat.percentage)
    {
        bDdata.percentage = rM.bat.percentage;
        dis->setCursor(cursorX, PercentageHeight);
        setTextSize(batteryTextSize);

        String percentageStr = String(rM.bat.percentage);
        while (percentageStr.length() < 6)
        {
            percentageStr = percentageStr + " ";
        }

        writeTextReplaceBack(DEBUG_BATTERY_LEVEL + percentageStr, cursorX, PercentageHeight);
        dUChange = true;
    }
    useButtonBlank();
    disUp();
}

#endif