#include "defines/defines.h"

#if DEBUG
int loopDumpDelayMs = 0;
#endif

bool almostSleep = false;
void setup()
{
  esp_sleep_wakeup_cause_t wakeUpReason = esp_sleep_get_wakeup_cause();
  // ESP_SLEEP_WAKEUP_EXT0 RTC alarm
  // ESP_SLEEP_WAKEUP_EXT1 Button press
  bool wakedUpFromSleep = false;
  if (wakeUpReason == ESP_SLEEP_WAKEUP_EXT0 || wakeUpReason == ESP_SLEEP_WAKEUP_EXT1)
  {
    wakedUpFromSleep = true;
#if DEBUG
    if (wakeUpReason == ESP_SLEEP_WAKEUP_EXT0)
    {
      debugLog("Waked up because of RTC");
    }
    else if (wakeUpReason == ESP_SLEEP_WAKEUP_EXT1)
    {
      debugLog("Waked up because of buttons");
      manageButtonWakeUp();
    }
#endif
  }

  if (wakedUpFromSleep == true)
  {
    initWakeUpHardware(true);
  }
  else
  {
    initAllHardware();
  }

#if DEBUG && DUMP_INIT_DEBUG
  initHardwareDebug();
  loopHardwareDebug();
#endif

  // turnOnWifi();
  initManager();
  initButtonTask();
}

void loop()
{
  loopBattery();
  loopManager();

#if DEBUG && EINK_COUNTER
  showEinkCounter();
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

  if (millis() - sleepDelayMs > SLEEP_EVERY_MS)
  {
    if (almostSleep == false && currentPlace != FIRST_PLACE)
    {
      debugLog("SLEEP_EVERY_MS runned out, Showing watchface");
      almostSleep = true;

      currentPlace = NoPlace;
      currentPlaceIndex = 0;

      sleepDelayMs = sleepDelayMs + TIME_FOR_WATCHFACE_TO_SHOW_MS;
    }
    else
    {
      debugLog("SLEEP_EVERY_MS runned out, going to sleep");
      goSleep();
    }
  }
}
