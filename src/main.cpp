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

  turnOnWifi();
  // initWifiDebugDisplay();
  // initBatteryDebugDisplay();

  initButtonTask();

  //float cambridge_max_temp_22[] = {68.23, 29.87, 42.11, 91.5, 10.78, 50.32, 75.44, 84.69, 9.14, 61.98, 17.82, 98.63};
  //float cambridge_min_temp_22[] = {61.42, 19.34, 26.08, 79.72, 6.91, 45.77, 62.46, 77.85, 3.74, 47.53, 14.25, 90.04};

  //showDoubleDataBarChart(cambridge_max_temp_22, cambridge_min_temp_22, 12, "asdasd");
  //display.display(PARTIAL_UPDATE);
  // initBatteryDebugDisplay();
  initGeneralDebugDisplay();
  GeneralDebug();
}

void loop()
{
  GeneralDebugLoop();
  loopGeneralDebugDisplay();
  loopBattery();
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
