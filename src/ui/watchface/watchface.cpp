#include "watchface.h"

RTC_DATA_ATTR tmElements_t wFTime;
RTC_DATA_ATTR bool disableSomeDrawing = false;

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

  wfModulesManage(None);
  drawPosMarker();

  disUp(true, false, true);
}

bool wentToSleep = false; // Don't go to sleep after one try of noClickedButton - maybe a sync is going on?
void loopWatchfaceLoop()
{
  // debugLog("Executing loop watch face");
  if (wFTime.Minute != timeRTCLocal.Minute || wFTime.Hour != timeRTCLocal.Hour) // Hour too because of timezone
  {
    dUChange = true;

    drawTimeBeforeApply();

    wFTime.Minute = timeRTCLocal.Minute;
    wFTime.Hour = timeRTCLocal.Hour;

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

    wfModulesManage(None);
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

  buttonState bt = useButton();
  modulesButtons(bt);

// if(dUChange == false) {
// debugLog("No change in watchface, skipping timer");
// Always go to sleep in watchface after loop
// Well not if it's charging
#if MODULES_OPERATING_FAST
  if (bt != None && currentPlaceIndex == 0)
  {
    // Should be simplfied to setSleepDelay
    debugLog("Watchface modules fast operating done");
    wentToSleep = true;
    int makeMinus = -1 * (SLEEP_EVERY_MS * MODULES_WAITING_DIVISION);
    debugLog("makeMinus: " + String(makeMinus));
    resetSleepDelay(makeMinus);
  }
#endif
  if (bt == None && wentToSleep == false && (bat.isCharging == false)) // || SYNC_WIFI == 0
  {
    // We dont want resetDelay because if something wants to sleep, we dont want to be the reason for forcing it
    debugLog("Watchface requesting sleep");
    sleepDelayMs = sleepDelayMs - SLEEP_EVERY_MS;
    // sleepDelayMs = millisBetter() - SLEEP_EVERY_MS;
    wentToSleep = true;
  }

  // We ignore sleep because probably we will want to go to sleep fast
  disUp(dUChange, false, true);
}