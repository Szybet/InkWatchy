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
TaskHandle_t watchdogTask = NULL;

#define WATCHDOG_DELAY 30 * 1000
#define WATCHDOG_SMALL_DELAY 3 * 1000
#if WATCHDOG_TASK_TIMEOUT
int64_t watchdogMillis = 0;
std::atomic<bool> watchdogFlag{false};
#endif

void loopWatchdogTask(void *parameter)
{
// debugLog("Watchdog starting, slowly");
// delayTask(WATCHDOG_DELAY);
// debugLog("Watchdog finally started!");
#if WATCHDOG_TASK_TIMEOUT
    watchdogMillis = millisBetter();
#endif
    delayTask(WATCHDOG_SMALL_DELAY);
    while (true)
    {
        // debugLog("Watchdog cycle");
        if (allButtonCheck() == true)
        {
            // debugLog("Detected all buttons high, resetting...");
            assert(0);
        }
#if WATCHDOG_TASK_TIMEOUT
        if (millisBetter() - watchdogMillis > WATCHDOG_DELAY)
        {
            watchdogMillis = millisBetter();
            if (watchdogFlag.load(std::memory_order_relaxed) == false)
            {
                // debugLog("rM.everythingIsFine is false, resetting...");
                assert(0);
            }
            else
            {
                watchdogFlag.store(false, std::memory_order_relaxed);
            }
        }
#endif
        delayTask(WATCHDOG_SMALL_DELAY);
    }
}

void initWatchdogTask()
{
    debugLog("Init watchdog");
    watchdogPing();
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
#if WATCHDOG_TASK_TIMEOUT
    watchdogFlag.store(true, std::memory_order_relaxed);
#endif
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
