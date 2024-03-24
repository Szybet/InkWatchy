#include "watchdogTask.h"

RTC_DATA_ATTR bool everythingIsFine = false;
TaskHandle_t watchdogTask = NULL;

#if WATCHDOG_TASK
void loopWatchdogTask(void *parameter)
{
    debugLog("Watchdog starting");
    while(true) {
        delayTask(5000);
        // debugLog("Watchdog cycle");
        if(digitalRead(BACK_PIN) == HIGH && digitalRead(MENU_PIN) == HIGH && digitalRead(UP_PIN) == HIGH && digitalRead(DOWN_PIN) == HIGH) {
            debugLog("Detected all buttons high, resetting...");
            ESP.restart();
        }
        if(everythingIsFine == false) {
            debugLog("everythingIsFine is false, resetting...");
            ESP.restart();
        } else {
            everythingIsFine = false;
            delayTask(3000);
        }
    }
}

void initWatchdogTask()
{
    debugLog("- done");
    xTaskCreate(
        loopWatchdogTask,
        "watchdogTask",
        2000,
        NULL,
        0,
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
    //debugLog("watchdogPing called");
    everythingIsFine = true;
}

void leaveFlashMessage(String message)
{
    debugLog("leaveFlashMessage: " + message);
    initNvsManage();
    String str = NVS.getString(NVS_WATCHDOG_DEBUG_DATA);
    if(str.indexOf(message) != -1) {
        str = str + " " + message;
        NVS.setString(NVS_WATCHDOG_DEBUG_DATA, str, true);
        debugLog("Saving this message");
    }
}

void readFlashMessage() {
    debugLog("readFlashMessage inside");
#if DEBUG
    initNvsManage();
    String str = NVS.getString(NVS_WATCHDOG_DEBUG_DATA);
    debugLog("Flash message: " + str);
#endif
}

#else
void loopWatchdogTask(void *parameter)
{
    return void();
}

void initWatchdogTask()
{
    return void();
}

void deInitWatchdogTask()
{
    return void();
}

void watchdogPing()
{
    return void();
}

void leaveFlashMessage(String message)
{
    return void();
}

void readFlashMessage() {
    return void();
}
#endif