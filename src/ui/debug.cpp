#include "debug.h"

debugDisplay ui;

int cursorX = 0;
int currentVoltageHeight;
#define textSize 1

void initDebugDisplay()
{
    int16_t x, y;
    uint16_t w, h;
    ui.battery = bat;
    setFont(&FreeSansBold9pt7b);
    setTextSize(textSize);
    display.setCursor(cursorX, 1);
    String menuName = "Debug Menu: Batt";
    display.getTextBounds(menuName, cursorX, 1, &x, &y, &w, &h);
    // log("Menu string cord: " + String(x) + "x" + String(y) + " " + String(w) + "x" = String(h));
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
}

void loopDebugDisplay()
{
    // if (ui.battery.curV != bat.curV)
    {
        ui.battery.curV = bat.curV;
        display.setCursor(cursorX, currentVoltageHeight);
        setTextSize(textSize);

        String battVoltageStr = String(bat.curV);
        while(battVoltageStr.length() < 5) {
            battVoltageStr = battVoltageStr + " ";
        }

        writeTextReplaceBack("Current batt V: " + battVoltageStr, cursorX, currentVoltageHeight);
        display.display(PARTIAL_UPDATE);
    }
}
