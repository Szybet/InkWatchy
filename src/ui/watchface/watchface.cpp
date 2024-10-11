#include "watchface.h"

RTC_DATA_ATTR tmElements_t wFTime;
RTC_DATA_ATTR bool disableSomeDrawing = false;
bool disableWatchfaceFastOperating = false;

void initWatchfaceDisplay()
{
  debugLog("Executing init watch face");

  wFTime.Second = timeRTCLocal.Second;
  wFTime.Minute = timeRTCLocal.Minute;
  wFTime.Hour = timeRTCLocal.Hour;
  wFTime.Day = timeRTCLocal.Day;
  wFTime.Month = timeRTCLocal.Month;
  wFTime.Year = timeRTCLocal.Year;
  // dumpRTCTime(wFTime);
  // dumpRTCTime(timeRTCLocal);

  showFullWatchface();

  // TODO: test and remote it, its done in showfull watchface?
  wfModulesManage(None);
  drawPosMarker();

  disUp(true, false, true);
}

bool wentToSleep = false; // Don't go to sleep after one try of noClickedButton - maybe a sync is going on?
void loopWatchfaceLoop()
{
  debugLog("Executing loop watch face");
  bool timeHappened = true;
  if (wFTime.Minute != timeRTCLocal.Minute || wFTime.Hour != timeRTCLocal.Hour) // Hour too because of timezone
  {
    dUChange = true;

#if LP_CORE == true
    screenTimeChanged = true;
    if (screenForceNextFullTimeWrite == true)
    {
      screenForceNextFullTimeWrite = false;
      lpCoreScreenPrepare(false);
      showTimeFull();
    }
    else
#endif
    {
      drawTimeBeforeApply();

      wFTime.Minute = timeRTCLocal.Minute;
      wFTime.Hour = timeRTCLocal.Hour;

      if (disableSomeDrawing == false)
      {
        drawTimeAfterApply();
      }
    }

    if (disableSomeDrawing == false)
    {
      drawTimeAfterApply();

      if (wFTime.Day != timeRTCLocal.Day)
      {
        wFTime.Day = timeRTCLocal.Day;
        drawDay();
      }

      if (wFTime.Month != timeRTCLocal.Month)
      {
        wFTime.Month = timeRTCLocal.Month;
        drawMonth();
      }
    }
    debugLog("getUnixTime(timeRTCLocal): " + String(getUnixTime(timeRTCLocal)));
    debugLog("latestModuleUpdate: " + String(latestModuleUpdate));
#if MODULE_UPDATE_LIMIT_S != 0
    if (getUnixTime(timeRTCLocal) - latestModuleUpdate > MODULE_UPDATE_LIMIT_S)
    {
      debugLog("Finally updating modules via time trigger");
      wfModulesManage(None);
    }
#endif
  }
  else
  {
    timeHappened = false;
  }

  // Hmm this could be in the minute checker
  if (disableSomeDrawing == false)
  {
    if (batteryPercantageWF != bat.percentage)
    {
      batteryPercantageWF = bat.percentage;
      drawBattery();
      dUChange = true;
    }
  }

  buttonState bt = useAllButtons();
  modulesButtons(bt);

// if(dUChange == false) {
// debugLog("No change in watchface, skipping timer");
// Always go to sleep in watchface after loop
// Well not if it's charging
#if MODULES_OPERATING_FAST
  if (bt != None && currentPlaceIndex == 0)
  {
    wentToSleep = true;
    if (disableWatchfaceFastOperating == false)
    {
      // Should be simplfied to setSleepDelay
      debugLog("Watchface modules fast operating done");
      int makeMinus = -1 * (SLEEP_EVERY_MS * MODULES_WAITING_DIVISION);
      debugLog("makeMinus: " + String(makeMinus));
      resetSleepDelay(makeMinus);
    }
    else
    {
      debugLog("Watchface modules fast operating is disabled");
      sleepDelayMs = sleepDelayMs - SLEEP_EVERY_MS;
    }
  }
#endif
  if (bt == None && wentToSleep == false)
  {
    // We dont want resetDelay because if something wants to sleep, we dont want to be the reason for forcing it
    debugLog("Watchface requesting sleep");
    sleepDelayMs = sleepDelayMs - SLEEP_EVERY_MS;
    // sleepDelayMs = millisBetter() - SLEEP_EVERY_MS;
    wentToSleep = true;
  }

#if LP_CORE == true
  // Because the arduino program doesn't know about the screen buffer, if we update anything it will dissapear :(
  debugLog("dUChange in the end of watchface: " + BOOL_STR(dUChange));
  if (dUChange == true)
  {
    screenTimeChanged = true;
    if (screenForceNextFullTimeWrite == true)
    {
      screenForceNextFullTimeWrite = false;
      lpCoreScreenPrepare(false);
      showTimeFull();
    }
  }
#endif

  // We ignore sleep because probably we will want to go to sleep fast
  disUp(dUChange, false, true);

  if (timeHappened == false && bt == None && wentToSleep == true)
  {
    debugLog("Nothing happened, delay...");
    delayTask(LOOP_NO_SCREEN_WRITE_DELAY_MS);
  }
}