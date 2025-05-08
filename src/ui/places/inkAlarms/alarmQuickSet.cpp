// alarmQuickSet.cpp
#include "alarmQuickSet.h"
#include "rtcMem.h"
#include "config.h"

#if INK_ALARMS

#define FONT getFont("taychron/Mono13")

void setAlarmQuick(int minutes, int id)
{
    readRTC();
    int hourNow    = timeRTCLocal.Hour;
    int minutesNow = timeRTCLocal.Minute;
    int secondsNow = timeRTCLocal.Second;

// one minute alarms should work betterly this way
    if (secondsNow > 0) {
        minutesNow += 1;
    }

    minutesNow += minutes;

    hourNow    += minutesNow / 60;
    minutesNow  = minutesNow % 60;
    hourNow     = hourNow % 24;

    if (minutes == 1440) {
        minutesNow = (minutesNow + 59) % 60;
        hourNow    = (hourNow + 23) % 24;
    }

    debugLog("Quick alarm set +" + String(minutes) +
             "min " + String(hourNow) + ":" + String(minutesNow));

    rM.alarms[id].days        = 0;
    rM.alarms[id].onlyOnce    = true;
    rM.alarms[id].hour        = hourNow;
    rM.alarms[id].minute      = minutesNow;
    rM.alarms[id].enabled     = true;
    rM.alarms[id].requireWifi = false;
    calculateNextAlarm();

// this shit doesnt work
//    // tell the display to do a full update next loop
//    dUChange = true;
//    // clear the whole screen now
//    disUp(true);
//
//    // 1) Draw the header text
//    setTextSize(1);
//    setFont(FONT);   
//    const int LABEL_X = 10;
//    const int LABEL_Y = 30;
//    writeTextReplaceBack("Alarm will ring at", LABEL_X, LABEL_Y, GxEPD_WHITE, GxEPD_BLACK);
//
//    char timeStr[6];
//    snprintf(timeStr, sizeof(timeStr), "%02d:%02d", hourNow, minutesNow);
//
//    setTextSize(1);
//    setFont(FONT);
//    const int RING_X = 20;
//    const int RING_Y = 20;  
//    writeTextReplaceBack(timeStr, RING_X, RING_Y, GxEPD_WHITE, GxEPD_BLACK);
//// no idea what i am doing
//    while (!useButton()) {
//      delay(10);
//    }


}

#define X(min,label)                  \
void sAQ##min(void) {                 \
    setAlarmQuick(min, ALARM_QUICK_ID); \
}
QUICK_ALARM_LIST(X)
#undef X

#define X(min,label) { label, &emptyImgPack, sAQ##min },
static entryMenu buttons[] = {
    QUICK_ALARM_LIST(X)
};
#undef X

void initAlarmQuickSet(void)
{
    initMenu(
        buttons,
        sizeof(buttons) / sizeof(buttons[0]),
        "Quick alarm",
        1
    );
}

#endif  // INK_ALARMS

