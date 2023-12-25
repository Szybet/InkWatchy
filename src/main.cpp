#include "defines/defines.h"

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
    if(wakeUpReason == ESP_SLEEP_WAKEUP_EXT0) {
      debugLog("Waked up because of RTC");
    } else if(wakeUpReason == ESP_SLEEP_WAKEUP_EXT1) {
      debugLog("Waked up because of buttons");
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
    initManager();
  }

#if DEBUG && DUMP_INIT_DEBUG
  initHardwareDebug();
  loopHardwareDebug();
#endif

  // turnOnWifi();
  // initWifiDebugDisplay();
  // initBatteryDebugDisplay();

  initButtonTask();
  // initBatteryDebugDisplay();
}

void loop()
{
  loopBattery();
  loopManager();
  // loopBatteryDebugDisplay();

  // loopWifiDebugDisplay();
  // loopBatteryDebugDisplay();

  // Needs to run forever because it deletes the wifi task. Use other conditions to determine if we need to do something with wifi too
  // if(isWifiConnected() == true) {

  //}

#if DEBUG && EINK_COUNTER
  showEinkCounter();
#endif

#if DEBUG && DUMP_LOOP_DEBUG
  loopHardwareDebug();
#endif

  delay(LOOP_DELAY);
  // Note for later: not all menus support waking up from wake ups because if they would, RTC_DATA_ATTR would be everywhere and we have only 8Kb

  if (millis() - sleepDelayMs > SLEEP_EVERY_MS)
  {
    debugLog("SLEEP_EVERY_MS runned out, going to sleep");
    goSleep();
  }
}
