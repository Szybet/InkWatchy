#include "alarmQuickSet.h"
#include "rtcMem.h"

#if INK_ALARMS

void setAlarmQuick(int minutes)
{
    readRTC();
    int hourNow = timeRTCLocal.Hour;
    int minutesNow = timeRTCLocal.Hour;

    minutesNow += minutes;
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
    rM.alarms[ALARM_QUICK_ID].days = 0;
    rM.alarms[ALARM_QUICK_ID].onlyOnce = true;
    rM.alarms[ALARM_QUICK_ID].hour = hourNow;
    rM.alarms[ALARM_QUICK_ID].minute = minutesNow;
    rM.alarms[ALARM_QUICK_ID].enabled = true;
    rM.alarms[ALARM_QUICK_ID].requireWifi = false;
    calculateNextAlarm();
}

void sAQ5()
{
    setAlarmQuick(5);
}

void sAQ15()
{
    setAlarmQuick(15);
}

void sAQ30()
{
    setAlarmQuick(30);
}

// 1h
void sAQ60()
{
    setAlarmQuick(60);
}

// 1.5h
void sAQ90()
{
    setAlarmQuick(90);
}

// 2h
void sAQ120()
{
    setAlarmQuick(120);
}

// 3h
void sAQ180()
{
    setAlarmQuick(180);
}

// 6h
void sAQ360()
{
    setAlarmQuick(360);
}

// 8h
void sAQ480()
{
    setAlarmQuick(480);
}

// 24h
void sAQ1440()
{
    setAlarmQuick(1440);
}

void initAlarmQuickSet()
{
    int c = -1;
    entryMenu buttons[10];
    {
        c = c + 1;
        buttons[c] = {.text = "5 minutes", .image = &emptyImgPack, .function = sAQ5};
    }
    {
        c = c + 1;
        buttons[c] = {.text = "15 minutes", .image = &emptyImgPack, .function = sAQ15};
    }
    {
        c = c + 1;
        buttons[c] = {.text = "30 minutes", .image = &emptyImgPack, .function = sAQ30};
    }
    {
        c = c + 1;
        buttons[c] = {.text = "1 hour", .image = &emptyImgPack, .function = sAQ60};
    }
    {
        c = c + 1;
        buttons[c] = {.text = "1.5 hours", .image = &emptyImgPack, .function = sAQ90};
    }
    {
        c = c + 1;
        buttons[c] = {.text = "2 hours", .image = &emptyImgPack, .function = sAQ120};
    }
    {
        c = c + 1;
        buttons[c] = {.text = "3 hours", .image = &emptyImgPack, .function = sAQ180};
    }
    {
        c = c + 1;
        buttons[c] = {.text = "6 hours", .image = &emptyImgPack, .function = sAQ360};
    }
    {
        c = c + 1;
        buttons[c] = {.text = "8 hours", .image = &emptyImgPack, .function = sAQ480};
    }
    {
        c = c + 1;
        buttons[c] = {.text = "24 hours", .image = &emptyImgPack, .function = sAQ1440};
    }
    c = c + 1;
    initMenu(buttons, c, "Quick alarm", 1);
}

#endif