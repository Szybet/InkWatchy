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
#endif

  // turnOnWifi();
  // initWifiDebugDisplay();
  // initBatteryDebugDisplay();

  initButtonTask();

  display.clearScreen();
entryMenu button0 = {"text0", crossImgPack, NULL};
entryMenu button1 = {"text1", acceptImgPack, NULL};
entryMenu button2 = {"text2", emptyImgPack, NULL};
entryMenu button3 = {"text3", emptyImgPack, NULL};
entryMenu button4 = {"text4", emptyImgPack, NULL};
entryMenu button5 = {"text5", emptyImgPack, NULL};
entryMenu button6 = {"text6", emptyImgPack, NULL};
entryMenu button7 = {"text7", emptyImgPack, NULL};
entryMenu button8 = {"text8", emptyImgPack, NULL};
entryMenu button9 = {"text9", emptyImgPack, NULL};
entryMenu button10 = {"text10", emptyImgPack, NULL};
entryMenu button11 = {"text11", emptyImgPack, NULL};
entryMenu button12 = {"text12", emptyImgPack, NULL};
entryMenu button13 = {"text13", emptyImgPack, NULL};
entryMenu button14 = {"text14", emptyImgPack, NULL};

entryMenu buttons[15] = {button0, button1, button2, button3, button4, button5, button6, button7, button8, button9, button10, button11, button12, button13, button14};
initMenu(buttons, 15, "MDKNCMIUEjqi", 2);
}

void loop()
{
  menuLoop();
  loopBattery();

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
    log("SLEEP_EVERY_MS runned out, going to sleep");
    goSleep();
  }
}
