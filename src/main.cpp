#include "defines/defines.h"

// https://github.com/espressif/arduino-esp32/blob/337058ac94e7e3df11d273a93e88d1ea605e6f5f/cores/esp32/main.cpp#L105
// Why is that? well anyway this task set's it and exits
TaskHandle_t priorityLoopHandle;
void priorityLoopSet(void *parameter)
{
  while (true)
  {
    TaskHandle_t loopTaskHandle = xTaskGetHandle("loopTask");
    if (loopTaskHandle != NULL)
    {
      vTaskPrioritySet(loopTaskHandle, MAIN_LOOP_PRIORITY);
      // debugLog("Set loop task priority, exiting...");
      vTaskDelete(NULL);
    }
    delayTask(350);
  }
}

esp_sleep_wakeup_cause_t wakeUpReason;
void setup()
{
#if DEBUG
#if PUT_LOGS_TO_SERIAL // This is here first because of watchy 3
  Serial.begin(SERIAL_BAUDRATE);
#if ATCHY_VER == WATCHY_3 || ATCHY_VER == YATCHY
  Serial.setDebugOutput(true);
#endif
#endif
#if WAIT_FOR_MONITOR
  delay(7500);
#endif
  initLogs();
#endif

  wakeUpReason = esp_sleep_get_wakeup_cause();
  // ESP_SLEEP_WAKEUP_EXT0 RTC alarm
  // ESP_SLEEP_WAKEUP_EXT1 Button press

  debugLog("Sleep wakeup reason: " + wakeupSourceToString(wakeUpReason));
  debugLog("esp_sleep_get_ext1_wakeup_status: " + String(esp_sleep_get_ext1_wakeup_status()));
  bool wakedUpFromSleep = false;

  if (isRtcWakeUpReason(wakeUpReason) == true || wakeUpReason == BUTTON_WAKEUP_REASON)
  {
    wakedUpFromSleep = true;
    if (isRtcWakeUpReason(wakeUpReason) == true)
    {
      debugLog("Waked up because of RTC");
    }
    else if (wakeUpReason == BUTTON_WAKEUP_REASON)
    {
      debugLog("Waked up because of buttons");
      manageButtonWakeUp();
    }
  }

  initHardware(wakedUpFromSleep, wakeUpReason);

  debugLog("Starting millis: " + String(millisBetter()));

#if DEBUG
#if DUMP_INIT_DEBUG
  initHardwareDebug();
  loopHardwareDebug();
#endif
#if DUMP_LOOP_SOFTWARE_DEBUG
  loopGeneralDebug();
#endif
#if VOLTAGE_PRINT_ON
  xTaskCreate(
      dumpBatteryScreen,
      "dumpBatteryTask",
      TASK_STACK_DUMP_BATTERY,
      NULL,
      0,
      nullptr);
#endif

#if SCOM_TASK_ENABLED
  initScomTask();
#endif
#endif

  initManager();

  if (isRtcWakeUpReason(wakeUpReason) == false)
  {
    // I trust myself enough now to not need watchdog task running all the time
    initWatchdogTask();
    watchdogPing();

    turnOnButtons();

    xTaskCreate(
        priorityLoopSet,
        "priorityLoop",
        1000,
        NULL,
        20,
        &priorityLoopHandle);
  }
}

void loop()
{
#if TEMP_CHECKS_ENABLED
  tempChecker();
#endif
  if (isRtcWakeUpReason(wakeUpReason) == false)
  {
    watchdogPing();
    alarmManageRTC();
  }
  loopManager();

#if !DEBUG || !NO_SYNC
  regularSync();
#endif

#if DEBUG && EINK_COUNTER
  showEinkCounter();
#endif

#if DEBUG && SPEED_THROUGH_TIME
  debugLog("Speeding time");
  timeRTCUTC0.Minute = timeRTCUTC0.Minute + 1;
  if (String(timeRTCUTC0.Minute).indexOf("5") != -1)
  {
    timeRTCUTC0.Hour = timeRTCUTC0.Hour + 1;
    timeRTCUTC0.Day = timeRTCUTC0.Day + 1;
    // timeRTCUTC0.Month = timeRTCUTC0.Month + 1; // We rely on previous day to clean up so this makes things break
  }
  if (timeRTCUTC0.Minute == 60)
  {
    timeRTCUTC0.Minute = 0;
    timeRTCUTC0.Hour = timeRTCUTC0.Hour + 1;
  }
  if (timeRTCUTC0.Hour == 24)
  {
    timeRTCUTC0.Hour = 0;
  }
  if (timeRTCUTC0.Day == 32)
  {
    timeRTCUTC0.Day = 1;
  }
  if (timeRTCUTC0.Month == 12)
  {
    timeRTCUTC0.Month = 0;
  }
  saveRTC(timeRTCUTC0);
  return;
#endif

#if DEBUG && (DUMP_LOOP_DEBUG || DUMP_LOOP_SOFTWARE_DEBUG)
  if (millisBetter() - loopDumpDelayMs > DUMP_LOOP_DELAY)
  {
    loopDumpDelayMs = millisBetter();
#if DUMP_LOOP_DEBUG
    loopHardwareDebug();
#endif
#if DUMP_LOOP_SOFTWARE_DEBUG
    loopGeneralDebug();
#endif
  }
#endif

#if !DISABLE_SLEEP
  manageSleep();
#endif
}
