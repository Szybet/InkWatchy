#include "generalDebug.h"

#if DEBUG == 1 || DEBUG_MENUS == 1
#define cursorX 0
int memoryHeight;
#define GeneralTextSize 1
uint8_t usedHeapLast;
SmallRTC rtc;
uint16_t usedHeapWidth;
uint16_t usedHeapLenght;

void initGeneralDebugDisplay()
{
    debugLog("initGeneralDebugDisplay called");
    uint16_t h;
    setFont(&FreeSansBold9pt7b);
    setTextSize(GeneralTextSize);
    display.setCursor(cursorX, 1);
    String menuName = "Debug Menu: General";
    getTextBounds(menuName, NULL, NULL, NULL, &h);
    maxHeight = h;
    uint16_t currentHeight = maxHeight;
    display.setCursor(cursorX, currentHeight - 3);
    display.print(menuName);

    display.fillRect(0, currentHeight, display.width(), 3, GxEPD_BLACK);
    currentHeight = currentHeight + maxHeight;
    int rtcType = rtc.getType();
    String RtcType;
    if (rtcType == 0)
    {
        RtcType = "Unknown";
    }
    else if (rtcType == 1)
    {
        RtcType = "DS3231";
    }
    else if (rtcType == 2)
    {
        RtcType = "PCF8563";
    }
    else
    {
        RtcType = "Invalid Type";
    }

    centerText("Chip Model:", &currentHeight);
    centerText(String(ESP.getChipModel()),&currentHeight);
    writeLine("RTC type: " + String(RtcType), cursorX, &currentHeight);

    currentHeight = currentHeight + 2;

    writeLine("Watchy version " + String(SRTC.getWatchyHWVer()), cursorX, &currentHeight);

    writeLine("Used Heap KB: " + String((ESP.getHeapSize() - ESP.getFreeHeap()) / 1024) + "/" + String(ESP.getHeapSize() / 1024), cursorX, &currentHeight);
    memoryHeight = currentHeight - maxHeight;
    display.display(PARTIAL_UPDATE);
}

void loopGeneralDebugDisplay()
{
    uint8_t usedHeap = ESP.getFreeHeap();
    if (usedHeap != usedHeapLast)
    {

        usedHeapLast = usedHeap;
        setFont(&FreeSansBold9pt7b);
        setTextSize(GeneralTextSize);

        String usedHeapStr = "Used Heap KB: " + String((ESP.getHeapSize() - ESP.getFreeHeap()) / 1024) + "/" + String(ESP.getHeapSize() / 1024); // Replace with actual function or variable

        getTextBounds(usedHeapStr, NULL, NULL, &usedHeapWidth, NULL);
        while (usedHeapWidth < usedHeapLenght)
        {
            usedHeapStr = usedHeapStr + " ";
            getTextBounds(usedHeapStr, NULL, NULL, &usedHeapWidth, NULL);
        }
        usedHeapLenght = usedHeapWidth;

        writeTextReplaceBack(usedHeapStr, cursorX, memoryHeight);
        display.display(PARTIAL_UPDATE);
    }
}


void GeneralDebug() {
    int rtcType = rtc.getType();
    String RtcType;
    if (rtcType == 0)
    {
        RtcType = "Unknown";
    }
    else if (rtcType == 1)
    {
        RtcType = "DS3231";
    }
    else if (rtcType == 2)
    {
        RtcType = "PCF8563";
    }
    else
    {
        RtcType = "Invalid Type";
    }
    debugLog("Chip Model: " + String(ESP.getChipModel()));
    debugLog("RTC type: " + String(RtcType));
    debugLog("Watchy version " + String(SRTC.getWatchyHWVer()));
    debugLog("Used Heap KB: " + String((ESP.getHeapSize() - ESP.getFreeHeap()) / 1024) + "/" + String(ESP.getHeapSize() / 1024));
}
void GeneralDebugLoop() {
    debugLog("Used Heap KB: " + String((ESP.getHeapSize() - ESP.getFreeHeap()) / 1024) + "/" + String(ESP.getHeapSize() / 1024));
}
#endif
