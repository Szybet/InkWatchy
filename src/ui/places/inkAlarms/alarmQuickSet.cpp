// alarmQuickSet.cpp
#include "alarmQuickSet.h"
#include "rtcMem.h"

#if INK_ALARMS

void setAlarmQuick(int minutes, int id)
{
    readRTC();
    int hourNow = timeRTCLocal.Hour;
    int minutesNow = timeRTCLocal.Minute;
    int secondsNow = timeRTCLocal.Second;

    // Note: 1-minute alarms are not supported as they may not provide a full minute due to timing issues

    minutesNow += minutes;

    hourNow += minutesNow / 60;
    minutesNow = minutesNow % 60;
    hourNow = hourNow % 24;

    if (minutes == 1440)
    {
        minutesNow = (minutesNow + 59) % 60;
        hourNow = (hourNow + 23) % 24;
    }

    debugLog("Quick alarm set +" + String(minutes) +
             "min " + String(hourNow) + ":" + String(minutesNow));

    rM.alarms[id].days = 0;
    rM.alarms[id].onlyOnce = true;
    rM.alarms[id].hour = hourNow;
    rM.alarms[id].minute = minutesNow;
    rM.alarms[id].enabled = true;
    rM.alarms[id].requireWifi = false;
    calculateNextAlarm();
}

//#define X(min,label)                  \
//void sAQ##min(void) {                 \
//    setAlarmQuick(min, ALARM_QUICK_ID); \
//}
// QUICK_ALARM_LIST(X)
// #undef X

#define X(min, label)                                                         \
    void sAQ##min(void)                                                       \
    {                                                                         \
        readRTC();                                                            \
        int hourNow = timeRTCLocal.Hour;                                      \
        int minutesNow = timeRTCLocal.Minute;                                 \
        int secondsNow = timeRTCLocal.Second;                                 \
                                                                              \
        minutesNow += min;                                                    \
        hourNow += minutesNow / 60;                                           \
        minutesNow = minutesNow % 60;                                         \
        hourNow = hourNow % 24;                                               \
        char timeBuf[6];                                                      \
        snprintf(timeBuf, sizeof(timeBuf), "%02d:%02d", hourNow, minutesNow); \
                                                                              \
        dis->fillScreen(GxEPD_WHITE);                                         \
        setFont(&FreeSansBold9pt7b);                                          \
        setTextSize(1);                                                       \
        dis->setTextWrap(false);                                              \
        dis->setCursor(0, 1);                                                 \
                                                                              \
        String menuName = "Quick Alarm alert";                                \
        uint16_t maxHeight;                                                   \
        getTextBounds(menuName, NULL, NULL, NULL, &maxHeight);                \
        uint16_t currentHeight = maxHeight;                                   \
        maxHeight = maxHeight + 1;                                            \
                                                                              \
        dis->setCursor(0, currentHeight - 3);                                 \
        dis->print(menuName);                                                 \
                                                                              \
        dis->fillRect(0, currentHeight, dis->width(), 3, GxEPD_BLACK);        \
        currentHeight = currentHeight + maxHeight;                            \
                                                                              \
        centerText("Alarm set for:", &currentHeight);                         \
        currentHeight += maxHeight + 2;                                       \
        centerText(String(label), &currentHeight);                            \
        currentHeight += maxHeight + 10;                                      \
        centerText("Will ring at:", &currentHeight);                          \
        currentHeight += maxHeight + 3;                                       \
        centerText(String(timeBuf), &currentHeight);                          \
                                                                              \
        dUChange = true;                                                      \
        disUp(true);                                                          \
        rM.updateCounter = FULL_DISPLAY_UPDATE_QUEUE;                         \
                                                                              \
        setAlarmQuick(min, ALARM_QUICK_ID);                                   \
    }

QUICK_ALARM_LIST(X)
#undef X

#define X(min, label) {label, &emptyImgPack, sAQ##min},
static entryMenu buttons[] = {
    QUICK_ALARM_LIST(X)};
#undef X

void initAlarmQuickSet(void)
{
    initMenu(
        buttons,
        sizeof(buttons) / sizeof(buttons[0]),
        "Quick alarm",
        1);
}

#endif // INK_ALARMS
