#include "hardwareDebug.h"
#include "rtcMem.h"

#if DEBUG || DEBUG_MENUS

uint32_t usedHeapLast;
uint8_t lineHeap;
uint8_t lineTemp;

#if TEMP_CHECKS_ENABLED
uint8_t lineInitTemp;
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

String getHeapStr()
{
    String heapStr = String(DEBUG_HW_USED_HEAP) +
                     String((ESP.getHeapSize() - ESP.getFreeHeap()) / 1024) +
                     "/" +
                     String(ESP.getHeapSize() / 1024);
    return heapStr;
}

void initGeneralDebugDisplay()
{
    init_general_page(5);
    general_page_set_title(DEBUG_MENU_HARDWARE);
    genpage_set_center();

    genpage_add(DEBUG_HW_CHIP_MODEL);
    genpage_add(String(ESP.getChipModel()).c_str());

    genpage_add(String(DEBUG_HW_RTC_TYPE + getRtcType()).c_str());

    String heapStr = getHeapStr();
    usedHeapLast = ESP.getFreeHeap();
    lineHeap = genpage_add(heapStr.c_str());

    genpage_add(getLittleFsSizeString().c_str());

#if TEMP_CHECKS_ENABLED
    previousTempUi = getTemp();
    debugLog("Previous temp ui is: " + String(previousTempUi));
    lineTemp = genpage_add(String(DEBUG_HW_CPU_TEMP + String(previousTempUi)).c_str());
    lineInitTemp = genpage_add(String(DEBUG_HW_INIT_TEMP + String(rM.initialTemp)).c_str());
#endif

    resetSleepDelay(SLEEP_EVERY_MS);
    general_page_set_main();
}

void loopGeneralDebugDisplay()
{
    uint32_t newHeap = ESP.getFreeHeap();
    if (newHeap != usedHeapLast)
    {
        usedHeapLast = newHeap;
        String newHeapStr = getHeapStr();
        genpage_change(newHeapStr.c_str(), lineHeap);
    }
#if TEMP_CHECKS_ENABLED
    float tempUi = getTemp();
    if(previousTempUi != tempUi) {
        previousTempUi = tempUi;
        genpage_change(String(DEBUG_HW_CPU_TEMP + String(tempUi)).c_str(), lineTemp);
    }
#endif

    general_page_set_main();
    slint_loop();
}

#endif

#if DEBUG

void initGeneralDebug()
{
    debugLog("Chip Model: " + String(ESP.getChipModel()));
    debugLog("RTC type: " + String(getRtcType()));
    debugLog("Used Heap KB: " + String((ESP.getHeapSize() - ESP.getFreeHeap()) / 1024) + "/" + String(ESP.getHeapSize() / 1024));
}

void loopGeneralDebug()
{
    debugLog(getHeapStr());
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
