#include "batteryDebug.h"
#include "rtcMem.h"

#if DEBUG || DEBUG_MENUS

batteryInfo bDdata;

uint8_t lineCurrentVoltage;
uint8_t linePercentage;
uint8_t lineCharging;
uint8_t lineFullyCharged;

void initBatteryDebugDisplay()
{
    bDdata = rM.bat;
    
    init_general_page(5);
    general_page_set_title(DEBUG_MENU_BATTERY);

    lineCurrentVoltage = genpage_add(String(DEBUG_BATTERY_CURRENT_V + String(bDdata.curV)).c_str());

    genpage_add(String(DEBUG_BATTERY_MINIMUM_V + String(BATTERY_MIN_VOLTAGE)).c_str());
    genpage_add(String(DEBUG_BATTERY_MAXIMUM_V + String(BATTERY_MAX_VOLTAGE)).c_str());
    genpage_add(String(DEBUG_BATTERY_CRITICAL_V + String(BATTERY_CRIT_VOLTAGE)).c_str());
    
    linePercentage = genpage_add(String(DEBUG_BATTERY_LEVEL + String(bDdata.percentage)).c_str());
    lineCharging = genpage_add(String(DEBUG_BATTERY_CHARGING + BOOL_STR(bDdata.isCharging)).c_str());

#if ATCHY_VER == YATCHY
    lineFullyCharged = genpage_add(String(DEBUG_BATTERY_FULLY + BOOL_STR(bDdata.isFullyCharged)).c_str());
#endif

    resetSleepDelay(SLEEP_EVERY_MS);
    general_page_set_main();
}

void loopBatteryDebugDisplay()
{
    loopBattery();
    isChargingCheck();
        
    if (bDdata.curV > rM.bat.curV + 0.01 || bDdata.curV < rM.bat.curV - 0.01)
    {
        bDdata.curV = rM.bat.curV;
        genpage_change(String(DEBUG_BATTERY_CURRENT_V + String(rM.bat.curV)).c_str(), lineCurrentVoltage);
    }
    
    if (bDdata.isCharging != rM.bat.isCharging)
    {
        bDdata.isCharging = rM.bat.isCharging;
        genpage_change(String(DEBUG_BATTERY_CHARGING + BOOL_STR(rM.bat.isCharging)).c_str(), lineCharging);
    }
    
#if ATCHY_VER == YATCHY
    if (bDdata.isFullyCharged != rM.bat.isFullyCharged)
    {
        bDdata.isFullyCharged = rM.bat.isFullyCharged;
        genpage_change(String(DEBUG_BATTERY_FULLY + BOOL_STR(rM.bat.isFullyCharged)).c_str(), lineFullyCharged);
    }
#endif
    
    if (bDdata.percentage != rM.bat.percentage)
    {
        bDdata.percentage = rM.bat.percentage;
        genpage_change(String(DEBUG_BATTERY_LEVEL + String(rM.bat.percentage)).c_str(), linePercentage);
    }
    
    general_page_set_main();
    slint_loop();
}

#endif