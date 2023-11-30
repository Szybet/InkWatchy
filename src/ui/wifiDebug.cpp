#include "wifiDebug.h"

#if DEBUG == 1 || DEBUG_MENUS == 1
int cursorXwifi = 0;
int StatusHeight;
#define TextSize 1

void initWifiDebugDisplay() {
    int16_t x, y;
    uint16_t w, h;
    setFont(&FreeSansBold9pt7b);
    setTextSize(TextSize);
    display.setCursor(cursorXwifi, 1);
    String menuName = "Debug Menu: Wifi";
    display.getTextBounds(menuName, cursorXwifi, 1, &x, &y, &w, &h);
    // log("Menu string cord: " + String(x) + "x" + String(y) + " " + String(w) + "x" = String(h));
    maxHeight = h;
    uint16_t currentHeight = maxHeight;
    display.setCursor(cursorXwifi, currentHeight - 3);
    display.print(menuName);

    display.fillRect(0, currentHeight, display.width(), 3, GxEPD_BLACK);
    currentHeight = currentHeight + maxHeight;
    centerText("Wifi status: ", &currentHeight);
    centerText(String(wifiStatus()), &currentHeight);
    StatusHeight = currentHeight - maxHeight;
    display.fillRect(0, currentHeight - h/2 - 2, display.width(), 1, GxEPD_BLACK);
    currentHeight = currentHeight + 5;
    writeLine("Turn wifi on",cursorXwifi, &currentHeight);
    writeLine("Turn wifi off",cursorXwifi, &currentHeight);
    display.display(FULL_UPDATE);
}

void loopWifiDebugDisplay()
{
    setFont(&FreeSansBold9pt7b);
    setTextSize(TextSize);
   writeTextCenterReplaceBack(String(wifiStatus()), StatusHeight);
    display.display(PARTIAL_UPDATE);

}
#endif