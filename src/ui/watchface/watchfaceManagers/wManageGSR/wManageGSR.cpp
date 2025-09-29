#include "wManageGSR.h"
#include "rtcMem.h"

#if GSR_WATCHFACES

#if GSR_WATCHFACES_OPTIMISE
RTC_DATA_ATTR uint8_t gsrHour = -1;
RTC_DATA_ATTR uint8_t gsrMinute = -1;
#endif

void wManageGsrLaunch(WatchyGSR *gsr, bool init)
{
    rM.wFTime = timeRTCLocal;
    WatchTime.Local = timeRTCLocal;
    WatchTime.Local.Year = WatchTime.Local.Year + 70; // I hate EVERYONE

    if (init == true)
    {
        gsr->InsertInitWatchStyle(1);
        gsr->RegisterWatchFaces();
    }

    bool draw = true;
#if GSR_WATCHFACES_OPTIMISE
    if (gsrHour != WatchTime.Local.Hour || gsrMinute != WatchTime.Local.Minute || init == true)
    {
        gsrHour = WatchTime.Local.Hour;
        gsrMinute = WatchTime.Local.Minute;
    }
    else
    {
        draw = false;
    }
#endif

    if (useButton() == Menu)
    {
        resetSleepDelay();
        generalSwitch(mainMenu);
    }
    else
    {
        setSleepDelay(0);
        if(draw == true) {
            dis->fillScreen(SCWhite);
            gsr->InsertDrawWeather(1, true);
            gsr->InsertDrawWatchStyle(1);
            dUChange = true;    
        }
    }
    disUp();
}

#endif
