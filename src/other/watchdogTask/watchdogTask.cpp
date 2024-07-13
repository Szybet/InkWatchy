#include "watchdogTask.h"

RTC_DATA_ATTR bool everythingIsFine = false;
std::mutex watchdogFine;
TaskHandle_t watchdogTask = NULL;

bool allButtonCheck()
{
    if (digitalRead(BACK_PIN) == BUT_CLICK_STATE && digitalRead(MENU_PIN) == BUT_CLICK_STATE && digitalRead(UP_PIN) == BUT_CLICK_STATE && digitalRead(DOWN_PIN) == BUT_CLICK_STATE)
    {
        return true;
    }
    return false;
}

#if WATCHDOG_TASK
void loopWatchdogTask(void *parameter)
{
    debugLog("Watchdog starting");
    while (true)
    {
        // debugLog("Watchdog cycle");
        if (allButtonCheck() == true)
        {
            debugLog("Detected all buttons high, resetting...");
            assert(true == false);
        }
        watchdogFine.lock();
        if (everythingIsFine == false)
        {
            debugLog("everythingIsFine is false, resetting...");
            assert(true == false);
        }
        else
        {
            everythingIsFine = false;
            watchdogFine.unlock();
            delayTask(30000);
        }
        // It's at the end to run the button check at the start (of the watch)
        delayTask(30000);
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
    everythingIsFine = true;
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