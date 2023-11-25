#include "debug.h"
#include "Fonts/FreeSansBold9pt7b.h"

debugDisplay ui;

int cursorX = 0;
int maxHeight;
int currentVoltageHeight;
#define textSize 1

void initDebugDisplay()
{
    int16_t x, y;
    uint16_t w, h;
    int cursorY = 1;
    ui.battery = bat;
    display.setFont(&FreeSansBold9pt7b);
    display.setTextSize(textSize);
    display.setCursor(cursorX, cursorY);
    String menuName = "Debug Menu: Batt";
    display.getTextBounds(menuName, cursorX, cursorY, &x, &y, &w, &h);
    // log("Menu string cord: " + String(x) + "x" + String(y) + " " + String(w) + "x" = String(h));
    maxHeight = h;
    cursorY = cursorY + maxHeight;
    uint16_t currentHeight = cursorY;
    display.setCursor(cursorX, currentHeight - 3);
    display.print(menuName);

    display.fillRect(0, currentHeight, display.width(), 3, GxEPD_BLACK);

    currentHeight = currentHeight + maxHeight;
    display.setCursor(cursorX, currentHeight);
    display.print("Current batt V: ");
    currentHeight = currentHeight + maxHeight;
    display.setCursor(cursorX, currentHeight);
    currentVoltageHeight = currentHeight;
    display.print(ui.battery.curV);
    currentHeight = currentHeight + maxHeight;
    display.setCursor(cursorX, currentHeight);
    display.print("Minimum batt V: ");
    currentHeight = currentHeight + maxHeight;
    display.setCursor(cursorX, currentHeight);
    display.print(ui.battery.minV);
    currentHeight = currentHeight + maxHeight;
    display.setCursor(cursorX, currentHeight);
    display.print("Maximum batt V: ");
    currentHeight = currentHeight + maxHeight;
    display.setCursor(cursorX, currentHeight);
    display.print(ui.battery.maxV);
    currentHeight = currentHeight + maxHeight;
    display.setCursor(cursorX, currentHeight);
    display.print("Critical batt V: ");
    currentHeight = currentHeight + maxHeight;
    display.setCursor(cursorX, currentHeight);
    display.print(ui.battery.critV);
    display.display(FULL_UPDATE);
}

void loopDebugDisplay()
{
    // if (ui.battery.curV != bat.curV)
    {
        ui.battery.curV = bat.curV;
        display.setCursor(cursorX, currentVoltageHeight);
        display.setTextSize(textSize);
        // https://learn.adafruit.com/adafruit-gfx-graphics-library/using-fonts
        // use bitmap drawing here
        int16_t x, y;
        uint16_t w, h;
        display.getTextBounds("      ", cursorX, currentVoltageHeight, &x, &y, &w, &h);
        display.fillRect(x, y, w, h, GxEPD_WHITE);
        display.print(ui.battery.curV);
        display.display(PARTIAL_UPDATE);
    }
}