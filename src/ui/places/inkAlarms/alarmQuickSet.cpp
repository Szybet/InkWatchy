#include "alarmQuickSet.h"
#include "rtcMem.h"

#if INK_ALARMS

// Central quick alarm function
void setAlarmQuick(int minutes, int id)
{
    readRTC();
    int hourNow = timeRTCLocal.Hour;
    int minutesNow = timeRTCLocal.Minute;

    minutesNow += minutes; //HUGE ERROR, if you set a time for 1 minute it rings upon next minute, not in 60 seconds !!! not sure how to fix but need swork
    while (minutesNow >= 60)
    {
        minutesNow -= 60;
        hourNow += 1;
    }
    while (hourNow >= 24)
    {
        hourNow -= 24;
    }

    if (minutes == 1440)
    {
        minutesNow = minutesNow - 1;
    }
    if (minutesNow < 0)
    {
        minutesNow = 59;
        hourNow = hourNow - 1;
        if (hourNow < 0)
        {
            hourNow = 23;
        }
    }

    debugLog("For quick alarm added " + String(minutes) + " and the final time is: " + String(hourNow) + ":" + String(minutesNow));
    rM.alarms[id].days = 0;
    rM.alarms[id].onlyOnce = true;
    rM.alarms[id].hour = hourNow;
    rM.alarms[id].minute = minutesNow;
    rM.alarms[id].enabled = true;
    rM.alarms[id].requireWifi = false;
    calculateNextAlarm();
}

// Wrapper functions for menu callback compatibility
#define DEFINE_SAQ_FUNC(MIN) \
void sAQ##MIN() { setAlarmQuick(MIN, ALARM_QUICK_ID); }

DEFINE_SAQ_FUNC(5)
DEFINE_SAQ_FUNC(15)
DEFINE_SAQ_FUNC(30)
DEFINE_SAQ_FUNC(60)
DEFINE_SAQ_FUNC(90)
DEFINE_SAQ_FUNC(120)
DEFINE_SAQ_FUNC(180)
DEFINE_SAQ_FUNC(360)
DEFINE_SAQ_FUNC(480)
DEFINE_SAQ_FUNC(1440)

// Menu init
void initAlarmQuickSet()
{
    static entryMenu buttons[] = {
        {"5 minutes", &emptyImgPack, sAQ5},
        {"15 minutes", &emptyImgPack, sAQ15},
        {"30 minutes", &emptyImgPack, sAQ30},
        {"1 hour", &emptyImgPack, sAQ60},
        {"1.5 hours", &emptyImgPack, sAQ90},
        {"2 hours", &emptyImgPack, sAQ120},
        {"3 hours", &emptyImgPack, sAQ180},
        {"6 hours", &emptyImgPack, sAQ360},
        {"8 hours", &emptyImgPack, sAQ480},
        {"24 hours", &emptyImgPack, sAQ1440},
    };

    initMenu(buttons, sizeof(buttons) / sizeof(buttons[0]), "Quick alarm", 1);
}

#endif

