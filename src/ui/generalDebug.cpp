#include "generalDebug.h"

#if DEBUG == 1 || DEBUG_MENUS == 1
#define cursorX 0
int memoryHeight;
#define GeneralTextSize 1
uint8_t usedHeapLast;
SmallRTC rtc;
uint16_t usedHeapWidth;
uint16_t usedHeapLenght;

String getRtcType()
{
    int rtcType = rtc.getType();
    switch (rtcType)
    {
    case 0:
        return "Unknown";
    case 1:
        return "DS3231";
    case 2:
        return "PCF8563";
    default:
        return "Invalid Type";
    }
}

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
    String RtcType = getRtcType();

    centerText("Chip Model:", &currentHeight);
    centerText(String(ESP.getChipModel()), &currentHeight);
    writeLine("RTC type: " + String(RtcType), cursorX, &currentHeight);

    currentHeight = currentHeight + 2;

    writeLine("Watchy version " + String(SRTC.getWatchyHWVer()), cursorX, &currentHeight);

    writeLine("Used Heap KB: " + String((ESP.getHeapSize() - ESP.getFreeHeap()) / 1024) + "/" + String(ESP.getHeapSize() / 1024), cursorX, &currentHeight);
    memoryHeight = currentHeight - maxHeight;
    disUp(true);
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
        dUChange = true;
    }
    useButtonBlank();
    disUp();
}

#if DEBUG

void initGeneralDebug()
{
    debugLog("Chip Model: " + String(ESP.getChipModel()));
    debugLog("RTC type: " + String(getRtcType()));
    debugLog("Watchy version " + String(SRTC.getWatchyHWVer()));
    debugLog("Used Heap KB: " + String((ESP.getHeapSize() - ESP.getFreeHeap()) / 1024) + "/" + String(ESP.getHeapSize() / 1024));
}

void loopGeneralDebug()
{
    debugLog("Used Heap KB: " + String((ESP.getHeapSize() - ESP.getFreeHeap()) / 1024) + "/" + String(ESP.getHeapSize() / 1024));
}

#endif

#endif
