#include "defines/defines.h"

#if DEBUG
int loopDumpDelayMs = 0;
#endif

bool loopTaskApplied = false;
// The loop task runs in a task, which we want to change a few things
void loopTaskSettings() {
  if(loopTaskApplied == false) {
    loopTaskApplied = true;
    //UBaseType_t prio = uxTaskPriorityGet(NULL);
    //debugLog("Main task priority: " + String(prio));
    vTaskPrioritySet(NULL, MAIN_LOOP_PRIORITY);
  }
}

esp_sleep_wakeup_cause_t wakeUpReason;
void setup()
{
  isDebug(Serial.begin(921600));
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
  debugLog("readFlashMessage outside");
  readFlashMessage();

#if VOLTAGE_PRINT_ON
    xTaskCreate(
        dumpBatteryScreen,
        "dumpBatteryTask",
        2000,
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
  }

  initWatchdogTask();
}

void loop()
{
  loopTaskSettings();

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
    //timeRTC.Month = timeRTC.Month + 1; // We rely on previous day to clean up so this makes things break
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
  manageSleep();
}
