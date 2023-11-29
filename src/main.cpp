#include "defines/defines.h"

void setup()
{
  esp_sleep_wakeup_cause_t wakeup_reason = esp_sleep_get_wakeup_cause();
  /*
  switch (wakeup_reason)
  {
    case ESP_SLEEP_WAKEUP_EXT1:
    {

    }
  }
  */

#if SLEEP_AT_START
  if (wakeup_reason != ESP_SLEEP_WAKEUP_EXT1)
  {
    goSleep();
  }
#endif

  setupHardware();
  initBattery();
  initWifiDebugDisplay();
}

void loop()
{
  loopBattery();


#if DEBUG && EINK_COUNTER
  showEinkCounter();
#endif

#if DEBUG
  //dumpButtons();
  //showSetupResults();
  //debugRTC();
#endif
  delay(LOOP_DELAY);
}
