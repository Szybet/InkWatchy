#include "defines/defines.h"

#if DEBUG
int loopDumpDelayMs = 0;
#endif

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
      debugLog("Set loop task priority, exiting...");
      vTaskDelete(NULL);
    }
    delayTask(100);
  }
}

esp_sleep_wakeup_cause_t wakeUpReason;
void setup()
{
#if DEBUG
#if PUT_LOGS_TO_SERIAL // This is here first because of watchy 3
  Serial.begin(SERIAL_BAUDRATE);
#if ATCHY_VER == WATCHY_3
  Serial.setDebugOutput(true);
#endif
#if WAIT_FOR_MONITOR
  delay(5000);
#endif
  initLogs(); // This has #if in it, so it can run always
#endif
  wakeUpReason = esp_sleep_get_wakeup_cause();
  // ESP_SLEEP_WAKEUP_EXT0 RTC alarm
  // ESP_SLEEP_WAKEUP_EXT1 Button press

  bool wakedUpFromSleep = false;
  if (wakeUpReason == RTC_WAKEUP_REASON || wakeUpReason == BUTTON_WAKEUP_REASON)
  {
    wakedUpFromSleep = true;
    if (wakeUpReason == RTC_WAKEUP_REASON)
    {
      debugLog("Waked up because of RTC");
      setupTimeStructure();
      alarmManageRTC();
    }
    else if (wakeUpReason == BUTTON_WAKEUP_REASON)
    {
      debugLog("Waked up because of buttons");
      manageButtonWakeUp();
    }
  }

  initHardware(wakedUpFromSleep, wakeUpReason);

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
  if (wakeUpReason != RTC_WAKEUP_REASON)
  {
    initButtonTask();
    turnOnInterrupts();
  }

  initWatchdogTask();

  // Not sure
  //if (wakeUpReason != RTC_WAKEUP_REASON)
  //{
    xTaskCreate(
        priorityLoopSet,
        "priorityLoop",
        1000,
        NULL,
        20,
        &priorityLoopHandle);
  //}
}

void loop()
{
  watchdogPing();
  alarmManageRTC();
  loopBattery();
  loopManager();

#if !DEBUG || !NO_SYNC
  regularSync();
#endif

#if DEBUG && EINK_COUNTER
  showEinkCounter();
#endif

#if DEBUG && SPEED_THROUGH_TIME
  debugLog("Speeding time");
  timeRTC.Minute = timeRTC.Minute + 1;
  if (String(timeRTC.Minute).indexOf("5") != -1)
  {
    timeRTC.Hour = timeRTC.Hour + 1;
    timeRTC.Day = timeRTC.Day + 1;
    // timeRTC.Month = timeRTC.Month + 1; // We rely on previous day to clean up so this makes things break
  }
  if (timeRTC.Minute == 60)
  {
    timeRTC.Minute = 0;
    timeRTC.Hour = timeRTC.Hour + 1;
  }
  if (timeRTC.Hour == 24)
  {
    timeRTC.Hour = 0;
  }
  if (timeRTC.Day == 32)
  {
    timeRTC.Day = 1;
  }
  if (timeRTC.Month == 12)
  {
    timeRTC.Month = 0;
  }
  saveRTC();
  return;
#endif

#if DEBUG && (DUMP_LOOP_DEBUG || DUMP_LOOP_SOFTWARE_DEBUG)
  if (millis() - loopDumpDelayMs > DUMP_LOOP_DELAY)
  {
    loopDumpDelayMs = millis();
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
