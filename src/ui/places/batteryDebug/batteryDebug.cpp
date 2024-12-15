#include "batteryDebug.h"
#include "rtcMem.h"

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
    display.setCursor(cursorX, 1);
    String menuName = "Debug Menu: Batt";
    getTextBounds(menuName, NULL, NULL, NULL, &h);
    maxHeight = h;
    uint16_t currentHeight = maxHeight;
    display.setCursor(cursorX, currentHeight - 3);
    display.print(menuName);

    display.fillRect(0, currentHeight, display.width(), 3, GxEPD_BLACK);
    currentHeight = currentHeight + maxHeight;

    writeLine("Current V: " + String(bDdata.curV), cursorX, &currentHeight);
    currentVoltageHeight = currentHeight - maxHeight;

    writeLine("Minimum V: " + String(bDdata.minV), cursorX, &currentHeight);

    writeLine("Maximum V: " + String(bDdata.maxV), cursorX, &currentHeight);

    writeLine("Critical V: " + String(bDdata.critV), cursorX, &currentHeight);

    writeLine("Level %: " + String(bDdata.percentage), cursorX, &currentHeight);
    PercentageHeight = currentHeight - maxHeight;

    writeLine("Charging: " + BOOL_STR(bDdata.isCharging), cursorX, &currentHeight);
    ChargingHeight = currentHeight - maxHeight;

#if ATCHY_VER == YATCHY
    writeLine("Fully: " + BOOL_STR(bDdata.isFullyCharged), cursorX, &currentHeight);
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
        display.setCursor(cursorX, currentVoltageHeight);
        setTextSize(batteryTextSize);

        String battVoltageStr = String(rM.bat.curV);
        while (battVoltageStr.length() < 5)
        {
            battVoltageStr = battVoltageStr + " ";
        }

        writeTextReplaceBack("Current V: " + battVoltageStr, cursorX, currentVoltageHeight);
        dUChange = true;
    }
    if (bDdata.isCharging != rM.bat.isCharging)
    {
        bDdata.isCharging = rM.bat.isCharging;
        display.setCursor(cursorX, ChargingHeight);
        setTextSize(batteryTextSize);

        String chargingStr = BOOL_STR(rM.bat.isCharging);
        while (chargingStr.length() < 6)
        {
            chargingStr = chargingStr + " ";
        }

        writeTextReplaceBack("Charging: " + chargingStr, cursorX, ChargingHeight);
        dUChange = true;
    }
#if ATCHY_VER == YATCHY
    if (bDdata.isFullyCharged != rM.bat.isFullyCharged)
    {
        bDdata.isFullyCharged = rM.bat.isFullyCharged;
        display.setCursor(cursorX, fullyChargedHeight);
        setTextSize(batteryTextSize);

        String chargingStr = BOOL_STR(rM.bat.isCharging);
        while (chargingStr.length() < 6)
        {
            chargingStr = chargingStr + " ";
        }

        writeTextReplaceBack("Fully: " + chargingStr, cursorX, ChargingHeight);
        dUChange = true;
    }
#endif
    if (bDdata.percentage != rM.bat.percentage)
    {
        bDdata.percentage = rM.bat.percentage;
        display.setCursor(cursorX, PercentageHeight);
        setTextSize(batteryTextSize);

        String percentageStr = String(rM.bat.percentage);
        while (percentageStr.length() < 6)
        {
            percentageStr = percentageStr + " ";
        }

        writeTextReplaceBack("Level %: " + percentageStr, cursorX, PercentageHeight);
        dUChange = true;
    }
    useButtonBlank();
    disUp();
}

#endif
