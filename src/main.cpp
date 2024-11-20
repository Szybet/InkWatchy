#include "defines/defines.h"

// https://github.com/espressif/arduino-esp32/blob/337058ac94e7e3df11d273a93e88d1ea605e6f5f/cores/esp32/main.cpp#L105
// Why is that? well anyway this task set's it and exits
TaskHandle_t priorityLoopHandle;
void priorityLoopSet(void *parameter)
{
  while (true)
  {
    delayTask(350);
    TaskHandle_t loopTaskHandle = xTaskGetHandle("loopTask");
    if (loopTaskHandle != NULL)
    {
      vTaskPrioritySet(loopTaskHandle, MAIN_LOOP_PRIORITY);
      // debugLog("Set loop task priority, exiting...");
      vTaskDelete(NULL);
    }
  }
}

void setup()
{
#if DEBUG
  initLog();
#endif

  initHardware();
  // debugLog("Starting millis: " + String(millisBetter()));

#if DEBUG
  showInitLogs();
#endif

  initManager();

  if (bootStatus.reason != rtc)
  {
    // I trust myself enough now to not need watchdog task running all the time
    initWatchdogTask();

    turnOnButtons();

    xTaskCreate(
        priorityLoopSet,
        "priorityLoop",
        1000,
        NULL,
        20,
        &priorityLoopHandle);
  }

  // resetSleepDelay();
}

void loop()
{
#if TEMP_CHECKS_ENABLED
  tempChecker();
#endif
  if (bootStatus.reason != rtc)
  {
    watchdogPing();
    alarmManageRTC();
  }
  loopManager();

#if !DEBUG || !NO_SYNC
  regularSync();
#endif

#if DEBUG
  endLoopDebug();
#endif

#if !DISABLE_SLEEP
  manageSleep();
#endif
}
