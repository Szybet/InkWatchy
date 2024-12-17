#include "hardwareDebug.h"

#if DEBUG == 1 || DEBUG_MENUS == 1
#define cursorX 0
int memoryHeight;
#define GeneralTextSize 1
uint8_t usedHeapLast;
uint16_t usedHeapWidth;
uint16_t usedHeapLenght;

#if TEMP_CHECKS_ENABLED
int tempHeight;
float previousTempUi;
#endif

String getRtcType()
{
    int rtcType = SRTC.getType();
    switch (rtcType)
    {
    case 0:
        return "Unknown";
    case 1:
        return "DS3231";
    case 2:
        return "PCF8563";
    case 3:
        return "INTERNAL";
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
    dis->setCursor(cursorX, 1);
    String menuName = "Debug Menu: Hardware";
    getTextBounds(menuName, NULL, NULL, NULL, &h);
    if(containsBelowChar(menuName) == true) {
        h = h + 2;
    }
    maxHeight = h;
    uint16_t currentHeight = maxHeight;
    dis->setCursor(cursorX, currentHeight - 3);
    dis->print(menuName);

    dis->fillRect(0, currentHeight, dis->width(), 3, GxEPD_BLACK);
    currentHeight = currentHeight + maxHeight;
    String RtcType = getRtcType();

    centerText("Chip Model:", &currentHeight);
    centerText(String(ESP.getChipModel()), &currentHeight);
    writeLine("RTC type: " + String(RtcType), cursorX, &currentHeight);

    currentHeight = currentHeight + 2;

    writeLine("Used Heap KB: " + String((ESP.getHeapSize() - ESP.getFreeHeap()) / 1024) + "/" + String(ESP.getHeapSize() / 1024), cursorX, &currentHeight);
    memoryHeight = currentHeight - maxHeight;

#if TEMP_CHECKS_ENABLED
    writeLine("CPU temp: " + String(getTemp()), cursorX, &currentHeight);
    tempHeight = currentHeight - maxHeight;

    writeLine("Init temp: " + String(initialTemp), cursorX, &currentHeight);
#endif

    // Double the time before sleeping
    resetSleepDelay(SLEEP_EVERY_MS);
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
    #if TEMP_CHECKS_ENABLED
        float currentTemp = getTemp();
        if(currentTemp != previousTempUi) {
            previousTempUi = currentTemp;
            writeTextReplaceBack("CPU temp: " + String(currentTemp), cursorX, tempHeight);
            dUChange = true;
        }
    #endif
    useButtonBlank();
    disUp();
}

#if DEBUG

void initGeneralDebug()
{
    debugLog("Chip Model: " + String(ESP.getChipModel()));
    debugLog("RTC type: " + String(getRtcType()));
    debugLog("Used Heap KB: " + String((ESP.getHeapSize() - ESP.getFreeHeap()) / 1024) + "/" + String(ESP.getHeapSize() / 1024));
}

void loopGeneralDebug()
{
    debugLog("Used Heap KB: " + String((ESP.getFreeHeap()) / 1024) + "/" + String(ESP.getHeapSize() / 1024));
    // debugLog("Heap caps free size: " + String(heap_caps_get_free_size(MALLOC_CAP_8BIT)));
    debugLog("Current CPU frequency: " + String(getCpuFrequencyMhz()) + "Mhz");
}

#endif

#endif
