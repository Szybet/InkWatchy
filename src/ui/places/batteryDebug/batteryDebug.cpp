#include "batteryDebug.h"

#if DEBUG == 1 || DEBUG_MENUS == 1

batteryInfo bDdata;

#define cursorX 0
int currentVoltageHeight;
int ChargingHeight;
int PercentageHeight;
#define batteryTextSize 1

void initBatteryDebugDisplay()
{
    debugLog("initBatteryDebugDisplay called");
    uint16_t h;
    bDdata = bat;
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
    disUp(true);
}

void loopBatteryDebugDisplay()
{
    loopBattery();
    if (bDdata.curV > bat.curV + 0.01 || bDdata.curV < bat.curV - 0.01)
    {
        bDdata.curV = bat.curV;
        display.setCursor(cursorX, currentVoltageHeight);
        setTextSize(batteryTextSize);

        String battVoltageStr = String(bat.curV);
        while (battVoltageStr.length() < 5)
        {
            battVoltageStr = battVoltageStr + " ";
        }

        writeTextReplaceBack("Current V: " + battVoltageStr, cursorX, currentVoltageHeight);
        dUChange = true;
    }
    if (bDdata.isCharging != bat.isCharging)
    {
        bDdata.isCharging = bat.isCharging;
        display.setCursor(cursorX, ChargingHeight);
        setTextSize(batteryTextSize);

        String chargingStr = BOOL_STR(bat.isCharging);
        while (chargingStr.length() < 6)
        {
            chargingStr = chargingStr + " ";
        }

        writeTextReplaceBack("Charging: " + chargingStr, cursorX, ChargingHeight);
        dUChange = true;
    }
    if (bDdata.percentage != bat.percentage)
    {
        bDdata.percentage = bat.percentage;
        display.setCursor(cursorX, PercentageHeight);
        setTextSize(batteryTextSize);

        String percentageStr = String(bat.percentage);
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
