#include "wManageOne.h"
#include "rtcMem.h"

bool disableWatchfaceFastOperating = false;

void wManageOneInit(const watchfaceDefOne *wdo)
{
    debugLog("Executing init watch face");

    rM.wFTime.Second = timeRTCLocal.Second;
    rM.wFTime.Minute = timeRTCLocal.Minute;
    rM.wFTime.Hour = timeRTCLocal.Hour;
    rM.wFTime.Day = timeRTCLocal.Day;
    rM.wFTime.Month = timeRTCLocal.Month;
    rM.wFTime.Year = timeRTCLocal.Year;
    rM.batteryPercantageWF = rM.bat.percentage;

    // dumpRTCTime(rM.wFTime);
    // dumpRTCTime(timeRTCLocal);

    wManageOneDrawAll(wdo);
}

bool wentToSleep = false; // Don't go to sleep after one try of noClickedButton - maybe a sync is going on?
void wManageOneLoop(const watchfaceDefOne *wdo)
{
    // debugLog("Executing loop watch face");
    bool timeHappened = true;
    if (rM.wFTime.Minute != timeRTCLocal.Minute || rM.wFTime.Hour != timeRTCLocal.Hour) // Hour too because of timezone
    {
        debugLog("Watchface updating something");
        dUChange = true;

#if LP_CORE == true
        screenTimeChanged = true;
        if (screenForceNextFullTimeWrite == true)
        {
            screenForceNextFullTimeWrite = false;
            lpCoreScreenPrepare(false);
            wdo->showTimeFull();
        }
        else
#endif
        {
            wdo->drawTimeBeforeApply();

            rM.wFTime.Minute = timeRTCLocal.Minute;
            rM.wFTime.Hour = timeRTCLocal.Hour;

            if (rM.disableSomeDrawing == false)
            {
                wdo->drawTimeAfterApply(false);
            }
        }

        if (rM.disableSomeDrawing == false)
        {
            // wdo->drawTimeAfterApply(); // TODO: lp core not sure
            if (rM.batteryPercantageWF != rM.bat.percentage)
            {
                rM.batteryPercantageWF = rM.bat.percentage;
                wdo->drawBattery();
            }

            if (rM.wFTime.Day != timeRTCLocal.Day)
            {
                rM.wFTime.Day = timeRTCLocal.Day;
                wdo->drawDay();
            }

            if (rM.wFTime.Month != timeRTCLocal.Month)
            {
                rM.wFTime.Month = timeRTCLocal.Month;
                wdo->drawMonth();
            }
        }
        if (wdo->watchfaceModules == true)
        {
            debugLog("getUnixTime(timeRTCLocal): " + String(getUnixTime(timeRTCLocal)));
            debugLog("rM.latestModuleUpdate: " + String(rM.latestModuleUpdate));
#if MODULE_UPDATE_LIMIT_S != 0
            if (getUnixTime(timeRTCLocal) - rM.latestModuleUpdate > MODULE_UPDATE_LIMIT_S)
            {
                debugLog("Finally updating modules via time trigger");
                wfModulesManage(None);
            }
#endif
        }
    }
    else
    {
        timeHappened = false;
    }

#if MODULES_FAST
    if (isFullMode() == true)
    {
        wfModulesManage(None);
    }
#endif

    buttonState bt = useAllButtons();
    wdo->manageInput(bt);

// if(dUChange == false) {
// debugLog("No change in watchface, skipping timer");
// Always go to sleep in watchface after loop
// Well not if it's charging
#if MODULES_OPERATING_FAST
    if (wdo->watchfaceModules == true)
    {

        if (bt != None && currentPlaceIndex == 0)
        {
            wentToSleep = true;
            if (disableWatchfaceFastOperating == false)
            {
                // TODO: Should be simplified to setSleepDelay
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
    // debugLog("dUChange in the end of watchface: " + BOOL_STR(dUChange));
    if (dUChange == true)
    {
        screenTimeChanged = true;
        if (screenForceNextFullTimeWrite == true)
        {
            screenForceNextFullTimeWrite = false;
            lpCoreScreenPrepare(false);
            wdo->showTimeFull();
        }
    }
#endif

    // We ignore sleep because probably we will want to go to sleep fast
    disUp(dUChange, false, true);

    if (timeHappened == false && bt == None && wentToSleep == true)
    {
        // debugLog("Nothing happened, delay...");
        delayTask(LOOP_NO_SCREEN_WRITE_DELAY_MS);
    }
}

void wManageOneLaunch(const watchfaceDefOne *wdo, bool init)
{
    if (init == true)
    {
        wManageOneInit(wdo);
    }
    else
    {
        wManageOneLoop(wdo);
    }
}

void wManageOneDrawAll(const watchfaceDefOne *wdo)
{
    wdo->initWatchface();
    wdo->showTimeFull();
    wdo->drawBattery();
    wdo->drawTimeAfterApply(true);
    wdo->drawDay();
    wdo->drawMonth();

    if (wdo->watchfaceModules == true)
    {
        wfModulesManage(None, true);
        drawModuleCount(true);
    }

    disUp(true, false, true);
}