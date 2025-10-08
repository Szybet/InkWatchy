#include "defines.h"
#include "rtcMem.h"

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

  if (isFullMode() == true)
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

#if INK_ALARMS
  checkAlarms();
#endif
  rM.gpioExpander.setPinMode(MCP_ACC_INT_1, MCP_INPUT);
  rM.gpioExpander.setPinMode(MCP_ACC_INT_2, MCP_INPUT);
  getSteps();

  // bma4_set_advance_power_save(0, &rM.SBMA._devFptr4);
}

void loop()
{
  getSteps();
  debugLog("Acc int state 1: " + String(rM.gpioExpander.digitalRead(MCP_ACC_INT_1)));
  debugLog("Acc int state 2: " + String(rM.gpioExpander.digitalRead(MCP_ACC_INT_2)));
  debugLog("Error code: " + String(rM.SBMA.getErrorCode()));
  debugLog("Status code: " + String(rM.SBMA.getStatus()));
  debugLog("Step counter: " + String(rM.SBMA.isStepCounter()));
  uint8_t power = 0;
  // bma4_set_advance_power_save(0, &rM.SBMA._devFptr4);
  bma4_get_advance_power_save(&power, &rM.SBMA._devFptr4);
  debugLog("bma4_get_advance_power_save: " + String(power));
  if (rM.SBMA.getStatus() == 16)
  {
    debugLog("FUCK");
    delayTask(5000);
    // rM.SBMA.enableAccel();
    // rM.SBMA.enableStepCount();
  }
  delayTask(500);

  if (isFullMode() == true)
  {
    watchdogPing();
    manageRTC();
#if INK_ALARMS
    checkAlarms();
#endif
    loopBattery(false);
#if !DEBUG || !NO_SYNC
    regularSync();
#endif
  }
#if TEMP_CHECKS_ENABLED
  tempChecker();
#endif

  loopManager();

#if DEBUG
  endLoopDebug();
#endif

#if !DISABLE_SLEEP || !DEBUG
  manageSleep();
#endif
}
