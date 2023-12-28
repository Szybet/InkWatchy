#include "defines/defines.h"

#if DEBUG
int loopDumpDelayMs = 0;
#endif

#define RTC_WAKEUP ESP_SLEEP_WAKEUP_EXT0
#define BUTTON_WAKEUP ESP_SLEEP_WAKEUP_EXT1

esp_sleep_wakeup_cause_t wakeUpReason;
void setup()
{
  wakeUpReason = esp_sleep_get_wakeup_cause();
  // ESP_SLEEP_WAKEUP_EXT0 RTC alarm
  // ESP_SLEEP_WAKEUP_EXT1 Button press
  
  bool wakedUpFromSleep = false;
  if (wakeUpReason == RTC_WAKEUP || wakeUpReason == BUTTON_WAKEUP)
  {
    wakedUpFromSleep = true;
    if (wakeUpReason == RTC_WAKEUP)
    {
      debugLog("Waked up because of RTC");
      alarmManageRTC();
    }
    else if (wakeUpReason == BUTTON_WAKEUP)
    {
      debugLog("Waked up because of buttons");
      manageButtonWakeUp();
    }
  }

  if (wakedUpFromSleep == true)
  {
    initWakeUpHardware(true);
  }
  else
  {
    initAllHardware();
  }

#if DEBUG
#if DUMP_INIT_DEBUG
  initHardwareDebug();
  loopHardwareDebug();
#endif
#if DUMP_LOOP_SOFTWARE_DEBUG
    loopGeneralDebug();
#endif
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

  if(wakeUpReason == RTC_WAKEUP) {
    debugLog("Skipping timer");
    sleepDelayMs = sleepDelayMs + SLEEP_EVERY_MS;
  }

  if (millis() - sleepDelayMs > SLEEP_EVERY_MS)
  {
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
