#include "defines/defines.h"

#if DEBUG
int loopDumpDelayMs = 0;
#endif

esp_sleep_wakeup_cause_t wakeUpReason;
void setup()
{
  isDebug(Serial.begin(115200));
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
#endif

  initManager();
  if (wakeUpReason != RTC_WAKEUP_REASON)
  {
    initButtonTask();
  }

  regularSync();
}

void loop()
{
  alarmManageRTC();
  loopBattery();
  loopPowerSavings();
  loopManager();

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

  //debugLog("sleepDelayMs is:" + String(sleepDelayMs));
  //debugLog("millis is:" + String(long(millis())));
  if (long(millis()) - sleepDelayMs >= SLEEP_EVERY_MS)
  {
    if (isWifiTaskRunning == true)
    {
      debugLog("Wifi is turned on, waiting...");
      sleepDelayMs = millis();
      //debugLog("sleepDelayMs is after change:" + String(sleepDelayMs));
      return void();
    }
#if WIFI_TOOL
    if (wifiToolRunning == true)
    {
      debugLog("Wifi tool is running, waiting...");
      sleepDelayMs = millis();
      return void();
    }
#endif
#if APPLE_JOKE
    if (appleJokeRunning == true)
    {
      debugLog("Apple joke is running, waiting...");
      sleepDelayMs = millis();
      return void();
    }
#endif
    if (currentPlace != FIRST_PLACE)
    {
      debugLog("SLEEP_EVERY_MS runned out, Showing watchface");

      currentPlace = NoPlace;
      currentPlaceIndex = 0;

      sleepDelayMs = sleepDelayMs + TIME_FOR_WATCHFACE_TO_SHOW_MS;
    }
    else
    {
      debugLog("SLEEP_EVERY_MS runned out, going to sleep");
      resetSleepDelay();
      goSleep();
    }
  }
}
