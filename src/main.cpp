#include "defines.h"
#include "rtcMem.h"

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

    // setup() runs directly inside loopTask, so passing NULL sets its priority immediately
    vTaskPrioritySet(NULL, MAIN_LOOP_PRIORITY);
  }

#if INK_ALARMS
  checkAlarms();
#endif
}

void loop()
{
  if (isFullMode() == true)
  {
#if WATCHDOG_TASK_TIMEOUT
    watchdogPing();
#endif

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
