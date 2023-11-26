#include "debug.h"

#if DEBUG && EINK_COUNTER
#define SKIP_CYCLES 7
bool randomChoosed = false;
int counterX;
int counterY;
int countSkip = SKIP_CYCLES;
int counter = 0;
void showEinkCounter()
{
    log("countSkip: " + String(countSkip));
    if (countSkip == SKIP_CYCLES)
    {
        countSkip = 0;
        setTextSize(2);
        if (randomChoosed == false)
        {
            randomChoosed = true;
            counterX = random(50, 150);
            counterY = random(50, 150);
        }
        display.setCursor(counterX, counterY);
        counter = counter + 1;
        log("Showing einkCounter: " + String(counter));
        writeTextReplaceBack(String(counter), counterX, counterY);
        display.display(PARTIAL_UPDATE);
    }
    countSkip = countSkip + 1;
}
#endif

// Move everything below to batteryDebug
debugDisplay ui;

int cursorX = 0;
int currentVoltageHeight;
int ChargingHeight;
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

    writeLine("Charging: " + BOOL_STR(ui.battery.isCharging), cursorX, &currentHeight);
    ChargingHeight = currentHeight - maxHeight;
    display.display(FULL_UPDATE);
}

void loopDebugDisplay()
{
    if (ui.battery.curV > bat.curV + 0.01 || ui.battery.curV < bat.curV - 0.01)
    {
        ui.battery.curV = bat.curV;
        display.setCursor(cursorX, currentVoltageHeight);
        setTextSize(textSize);

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
        setTextSize(textSize);

        String chargingStr = BOOL_STR(bat.isCharging);
        while (chargingStr.length() < 5)
        {
            chargingStr = chargingStr + " ";
        }

        writeTextReplaceBack("Charging: " + chargingStr, cursorX, ChargingHeight);
        display.display(PARTIAL_UPDATE);
    }
}
