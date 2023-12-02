#include "batteryDebug.h"

#if DEBUG == 1 || DEBUG_MENUS == 1
debugDisplay ui;
#define cursorX 0
int currentVoltageHeight;
int ChargingHeight;
#define batteryTextSize 1

void initBatteryDebugDisplay()
{
    uint16_t h;
    ui.battery = bat;
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

    writeLine("Current batt V: " + String(ui.battery.curV), cursorX, &currentHeight);
    currentVoltageHeight = currentHeight - maxHeight;

    writeLine("Minimum batt V: " + String(ui.battery.minV), cursorX, &currentHeight);

    writeLine("Maximum batt V: " + String(ui.battery.maxV), cursorX, &currentHeight);

    writeLine("Critical batt V: " + String(ui.battery.critV), cursorX, &currentHeight);

    writeLine("Charging: " + BOOL_STR(ui.battery.isCharging), cursorX, &currentHeight);
    ChargingHeight = currentHeight - maxHeight;
    display.display(FULL_UPDATE);
}

void loopBatteryDebugDisplay()
{
    if (ui.battery.curV > bat.curV + 0.01 || ui.battery.curV < bat.curV - 0.01)
    {
        ui.battery.curV = bat.curV;
        display.setCursor(cursorX, currentVoltageHeight);
        setTextSize(batteryTextSize);

        String battVoltageStr = String(bat.curV);
        while (battVoltageStr.length() < 5)
        {
            battVoltageStr = battVoltageStr + " ";
        }

        writeTextReplaceBack("Current batt V: " + battVoltageStr, cursorX, currentVoltageHeight);
        display.display(PARTIAL_UPDATE);
    }
    if (ui.battery.isCharging != bat.isCharging)
    {
        ui.battery.isCharging = bat.isCharging;
        display.setCursor(cursorX, ChargingHeight);
        setTextSize(batteryTextSize);

        String chargingStr = BOOL_STR(bat.isCharging);
        while (chargingStr.length() < 6)
        {
            chargingStr = chargingStr + " ";
        }

        writeTextReplaceBack("Charging: " + chargingStr, cursorX, ChargingHeight);
        display.display(PARTIAL_UPDATE);
    }
}

#endif
