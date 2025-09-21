#include "setDateWatch.h"

#if SET_CLOCK_GUI

bool setDateWatchInitialized = false;
void initSetDateWatch()
{
    if (setDateWatchInitialized == false)
    {
        readRTC();
        setDateDay = timeRTCLocal.Day;
        setDateMonth = timeRTCLocal.Month + 1;
        setDateYear = timeRTCLocal.Year + 1970;
        switchSetDate();
        setDateWatchInitialized = true;
    }
    else
    {
        debugLog("initSetDateWatch - It's true!");
        setDateWatchInitialized = false;
    }
}

void loopSetDateWatch()
{
    setButton(LongBack);
}

void exitSetDateWatch()
{
    debugLog("Executed exitSetDateWatch");
    if (setDateWatchInitialized == false)
    {
        readRTC();
        // If timezone moves between days,months, years
        int dayDiff = int(timeRTCLocal.Day) - int(timeRTCUTC0.Day);
        int monthDiff = int(timeRTCLocal.Day) - int(timeRTCUTC0.Day);
        int yearDiff = int(timeRTCLocal.Year) - int(timeRTCUTC0.Year);
        timeRTCUTC0.Day = setDateDay + dayDiff;
        timeRTCUTC0.Month = (setDateMonth - 1) + monthDiff;
        timeRTCUTC0.Year = (setDateYear - 1970) + yearDiff;
        saveRTC(timeRTCUTC0);
        readRTC();
    }
    else
    {
        debugLog("exitSetDateWatch - It's true!");
    }
}

#endif