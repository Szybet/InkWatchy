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

// std::mutex watchdogFine;
TaskHandle_t watchdogTask = NULL;

#define WATCHDOG_DELAY 22500
#define WATCHDOG_SMALL_DELAY 3000
int64_t watchdogMillis = 0;

void loopWatchdogTask(void *parameter)
{
    // debugLog("Watchdog starting, slowly");
    // delayTask(WATCHDOG_DELAY);
    // debugLog("Watchdog finally started!");
    watchdogMillis = millisBetter();
    delayTask(WATCHDOG_SMALL_DELAY);
    while (true)
    {
        // debugLog("Watchdog cycle");
        if (allButtonCheck() == true)
        {
            // debugLog("Detected all buttons high, resetting...");
            assert(0);
        }
        if (watchdogMillis + WATCHDOG_DELAY > millisBetter())
        {
            watchdogMillis = millisBetter();
            // watchdogFine.lock();
            if (rM.everythingIsFine == false)
            {
                // debugLog("rM.everythingIsFine is false, resetting...");
                assert(0);
            }
            else
            {
                rM.everythingIsFine = false;
                // watchdogFine.unlock();
                // delayTask(WATCHDOG_DELAY);
            }
        }
        delayTask(WATCHDOG_SMALL_DELAY);
    }
}

void initWatchdogTask()
{
    debugLog("Init watchdog");
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
    // Nah
    // watchdogFine.lock();
    rM.everythingIsFine = true;
    // watchdogFine.unlock();
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