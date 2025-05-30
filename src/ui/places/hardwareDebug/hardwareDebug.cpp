#include "hardwareDebug.h"
#include "rtcMem.h"
#include "localization.h"

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
    int rtcType = rM.SRTC.getType();
    switch (rtcType)
    {
    case 0:
        return DEBUG_RTC_UNKNOWN;
    case 1:
        return DEBUG_RTC_DS3231;
    case 2:
        return DEBUG_RTC_PCF8563;
    case 3:
        return DEBUG_RTC_INTERNAL;
    default:
        return DEBUG_RTC_INVALID;
    }
}

void initGeneralDebugDisplay()
{
    dis->setTextWrap(false);
    debugLog("initGeneralDebugDisplay called");
    uint16_t h;
    setFont(&FreeSansBold9pt7b);
    setTextSize(GeneralTextSize);
    dis->setCursor(cursorX, 1);
    String menuName = DEBUG_MENU_HARDWARE;
    getTextBounds(menuName, NULL, NULL, NULL, &h);
    if (containsBelowChar(menuName) == true)
    {
        h = h + 2;
    }
    maxHeight = h;
    uint16_t currentHeight = maxHeight;
    dis->setCursor(cursorX, currentHeight - 3);
    dis->print(menuName);

    dis->fillRect(0, currentHeight, dis->width(), 3, GxEPD_BLACK);
    currentHeight = currentHeight + maxHeight;
    String RtcType = getRtcType();

    centerText(DEBUG_HW_CHIP_MODEL, &currentHeight);
    centerText(String(ESP.getChipModel()), &currentHeight);
    writeLine(DEBUG_HW_RTC_TYPE + String(RtcType), cursorX, &currentHeight);

    currentHeight = currentHeight + 2;

    writeLine(DEBUG_HW_USED_HEAP + String((ESP.getHeapSize() - ESP.getFreeHeap()) / 1024) + "/" + String(ESP.getHeapSize() / 1024), cursorX, &currentHeight);
    memoryHeight = currentHeight - maxHeight;

#if TEMP_CHECKS_ENABLED
    writeLine(DEBUG_HW_CPU_TEMP + String(getTemp()), cursorX, &currentHeight);
    tempHeight = currentHeight - maxHeight;

    writeLine(DEBUG_HW_INIT_TEMP + String(rM.initialTemp), cursorX, &currentHeight);
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

        String usedHeapStr = DEBUG_HW_USED_HEAP + String((ESP.getHeapSize() - ESP.getFreeHeap()) / 1024) + "/" + String(ESP.getHeapSize() / 1024); // Replace with actual function or variable

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
    if (currentTemp != previousTempUi)
    {
        previousTempUi = currentTemp;
        writeTextReplaceBack(DEBUG_HW_CPU_TEMP + String(currentTemp), cursorX, tempHeight);
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
    debugLog("Used Heap kB: " + String((ESP.getFreeHeap()) / 1024) + "/" + String(ESP.getHeapSize() / 1024));
    // This gives the same result
    // size_t totalHeap = heap_caps_get_total_size(MALLOC_CAP_DEFAULT);
    // size_t freeHeap = heap_caps_get_free_size(MALLOC_CAP_DEFAULT);
    // debugLog("Free Heap: " + String(freeHeap / 1024) + "/" + String(totalHeap / 1024) + " kB");
    debugLog("Current CPU frequency: " + String(getCpuFrequencyMhz()) + "Mhz");

#if false
    size_t freeSize = heap_caps_get_free_size(MALLOC_CAP_DEFAULT);
    size_t largestFreeBlock = heap_caps_get_largest_free_block(MALLOC_CAP_DEFAULT);
    size_t minimumFreeSize = heap_caps_get_minimum_free_size(MALLOC_CAP_DEFAULT);
    multi_heap_info_t heapInfo;
    heap_caps_get_info(&heapInfo, MALLOC_CAP_DEFAULT);

    debugLog("Free Memory: " + String(freeSize) + " bytes");
    debugLog("Largest Free Block: " + String(largestFreeBlock) + " bytes");
    debugLog("Minimum Free Size Since Boot: " + String(minimumFreeSize) + " bytes");
    debugLog("Total Allocated Bytes: " + String(heapInfo.total_allocated_bytes) + " bytes");
    debugLog("Total Free Bytes: " + String(heapInfo.total_free_bytes) + " bytes");
    debugLog("Largest Free Block in Heap Info: " + String(heapInfo.largest_free_block) + " bytes");
    debugLog("Allocated Blocks: " + String(heapInfo.allocated_blocks));
    debugLog("Free Blocks: " + String(heapInfo.free_blocks));
    debugLog("Total Blocks: " + String(heapInfo.total_blocks));
#endif
}

#endif

#endif