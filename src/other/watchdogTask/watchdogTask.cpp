#include "watchdogTask.h"
#include "rtcMem.h"

bool allButtonCheck()
{
    if (buttonRead(BACK_PIN) == BUT_CLICK_STATE && buttonRead(MENU_PIN) == BUT_CLICK_STATE && buttonRead(UP_PIN) == BUT_CLICK_STATE && buttonRead(DOWN_PIN) == BUT_CLICK_STATE)
    {
        return true;
    }
    return false;
}

bool anyButtonCheck()
{
    if (buttonRead(BACK_PIN) == BUT_CLICK_STATE || buttonRead(MENU_PIN) == BUT_CLICK_STATE || buttonRead(UP_PIN) == BUT_CLICK_STATE || buttonRead(DOWN_PIN) == BUT_CLICK_STATE)
    {
        return true;
    }
    return false;
}

#if WATCHDOG_TASK

std::mutex watchdogFine;
TaskHandle_t watchdogTask = NULL;

#define WATCHDOG_DELAY 15000

void loopWatchdogTask(void *parameter)
{
    debugLog("Watchdog starting, slowly");
    delayTask(WATCHDOG_DELAY);
    debugLog("Watchdog finally started!");
    while (true)
    {
        // debugLog("Watchdog cycle");
        if (allButtonCheck() == true)
        {
            debugLog("Detected all buttons high, resetting...");
            assert(true == false);
        }
        watchdogFine.lock();
        if (rM.everythingIsFine == false)
        {
            debugLog("rM.everythingIsFine is false, resetting...");
            assert(true == false);
        }
        else
        {
            rM.everythingIsFine = false;
            watchdogFine.unlock();
            delayTask(WATCHDOG_DELAY);
        }
        // It's at the end to run the button check at the start (of the watch)
        delayTask(WATCHDOG_DELAY);
    }
}

void initWatchdogTask()
{
    debugLog("- done");
    xTaskCreate(
        loopWatchdogTask,
        "watchdogTask",
        TASK_STACK_WATCHDOG,
        NULL,
        WATCHDOG_PRIORITY,
        &watchdogTask);
}

void deInitWatchdogTask()
{
    if (watchdogTask != NULL)
    {
        debugLog("Shutting down watchdog task");
        vTaskDelete(watchdogTask);
        watchdogTask = NULL;
    }
}

void watchdogPing()
{
    // debugLog("watchdogPing called");
    watchdogFine.lock();
    rM.everythingIsFine = true;
    watchdogFine.unlock();
}

#else
void loopWatchdogTask(void *parameter)
{
    return;
}

void initWatchdogTask()
{
    return;
}

void deInitWatchdogTask()
{
    return;
}

void watchdogPing()
{
    return;
}
#endif