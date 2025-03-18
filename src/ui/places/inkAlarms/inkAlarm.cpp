#include "inkAlarm.h"
#include "rtcMem.h"

#if INK_ALARMS

uint64_t offsetToAddForDays(uint64_t unixTimeNow, inkAlarm *theAlarm, tmElements_t timeAlarm)
{
    int day = weekday(unixTimeNow);
    switch (day)
    {
    case 1:
    {
        // "Sun";
        day = 6;
        break;
    }
    case 2:
    {
        // "Mon";
        day = 0;
        break;
    }
    case 3:
    {
        // "Tues";
        day = 1;
        break;
    }
    case 4:
    {
        // "Wed";
        day = 2;
        break;
    }
    case 5:
    {
        // "Thurs";
        day = 3;
        break;
    }
    case 6:
    {
        // "Fri";
        day = 4;
        break;
    }
    case 7:
    {
        // "Sat";
        day = 5;
        break;
    }
    }
    debugLog("Day is now: " + String(day));
    debugLog("The days we want to alarm: " + alarmGetDays(theAlarm));

    debugLog("timeAlarm.Hour: " + String(timeAlarm.Hour));
    debugLog("timeAlarm.Minute: " + String(timeAlarm.Minute));
    if (getBit(theAlarm->days, day) == true && (theAlarm->hour > timeAlarm.Hour || (theAlarm->hour == timeAlarm.Hour && theAlarm->minute > timeAlarm.Minute)))
    {
        debugLog("It's gonna trigger today, offset for days is zero then");
        return 0;
    }
    else
    {
        // Overcomplicated that one I think
        // Find the nearest day
        // 1 so we don't start from the same day again
        int c = 0;
        for (int i = day + 1; i < day + 8; i++)
        {
            c = c + 1;
            int ii = i;
            if (ii > 6)
            {
                ii = ii - 7;
            }
            debugLog("ii is in iter: " + String(ii));
            if (getBit(theAlarm->days, ii) == true)
            {
                debugLog("So final ii is: " + String(ii) + " when days is: " + String(theAlarm->days) + " and i is: " + String(i) + " and c is: " + String(c));
                return 86400 * c;
            }
        }
    }

    debugLog("We should not be here, or no days are enabled? " + String(theAlarm->days));
    return 0;
}

uint64_t getUnixTimeOfAlarm(inkAlarm *theAlarm)
{
    if (!theAlarm->enabled)
    {
        return UINT64_MAX;
    }
    uint64_t offsetToAdd = 0;

    readRTC(); // To get the latest and greatest unix time...
    tmElements_t timeAlarm = timeRTCLocal;
    uint64_t unixTimeNow = getUnixTime(timeRTCLocal);
    if (theAlarm->onlyOnce == true)
    {

        if (theAlarm->hour < timeAlarm.Hour || (theAlarm->hour == timeAlarm.Hour && theAlarm->minute < timeAlarm.Minute))
        {
            offsetToAdd = offsetToAdd + 86400; // One day
        }
    }
    else
    {
        if(theAlarm->days == 0) {
            debugLog("Alarm is set to days but no days selected, returning errored one");
            return UINT64_MAX;
        }
        uint64_t daysOffset = offsetToAddForDays(unixTimeNow, theAlarm, timeAlarm);
        debugLog("offsetToAddForDays added: " + String(daysOffset));
        offsetToAdd = offsetToAdd + daysOffset;
    }

    timeAlarm.Hour = theAlarm->hour;
    timeAlarm.Minute = theAlarm->minute;

    uint64_t unixTimeCalc = getUnixTime(timeAlarm);
    unixTimeCalc = (unixTimeCalc / 60) * 60; // Remove seconds
    unixTimeCalc = unixTimeCalc + offsetToAdd;
    debugLog("Unix time now is: " + String(unixTimeNow));
    debugLog("Unix time for alarm is: " + String(unixTimeCalc));
    debugLog("The alarm name is: " + alarmNameGet(theAlarm));
    return unixTimeCalc;
}

void calculateNextAlarm()
{
    uint64_t smallestUnix = UINT64_MAX;
    uint8_t c = 0;
    for (uint8_t i = 0; i < MAX_ALARMS; i++)
    {
        uint64_t alarmUnix = getUnixTimeOfAlarm(&rM.alarms[i]);
        if (alarmUnix < smallestUnix)
        {
            smallestUnix = alarmUnix;
            c = i;
        }
    }
    if (smallestUnix == UINT64_MAX)
    {
        smallestUnix = 0;
        c = 0;
    }
    debugLog("Next alarm is: " + String(smallestUnix) + " on index: " + String(c));
    rM.nextAlarm = smallestUnix;
    rM.nextAlarmIndex = c;
#if LP_CORE
    setAlarmForLpCore();
#endif
}

String alarmGetTime(inkAlarm *theAlarm)
{
    String hour = String(theAlarm->hour);
    if (hour.length() < 2)
    {
        hour = "0" + hour;
    }
    String minute = String(theAlarm->minute);
    if (minute.length() < 2)
    {
        minute = "0" + minute;
    }
    return hour + ":" + minute;
}

String alarmGetDays(inkAlarm *theAlarm)
{
    String days = "";
    if(theAlarm->days == 127) {
        days = "All days";
        return days;
    }
    if (getBit(theAlarm->days, 0) == true)
    {
        days = days + "Mo ";
    }
    if (getBit(theAlarm->days, 1) == true)
    {
        days = days + "Tu ";
    }
    if (getBit(theAlarm->days, 2) == true)
    {
        days = days + "We ";
    }
    if (getBit(theAlarm->days, 3) == true)
    {
        days = days + "Th ";
    }
    if (getBit(theAlarm->days, 4) == true)
    {
        days = days + "Fr ";
    }
    if (getBit(theAlarm->days, 5) == true)
    {
        days = days + "Sa ";
    }
    if (getBit(theAlarm->days, 6) == true)
    {
        days = days + "Su ";
    }
    if (days.length() > 1)
    {
        days.remove(days.length() - 1);
    }
    else
    {
        days = "No days";
    }
    return days;
}

String alarmNameGet(inkAlarm *theAlarm, int id)
{
    String alarmName = "";
    alarmName = alarmGetTime(theAlarm);
    if (theAlarm->onlyOnce == true)
    {
        alarmName = alarmName + ", once";
    }
    else
    {
        alarmName = alarmName + ", " + alarmGetDays(theAlarm);
    }

    if(theAlarm->requireWifi == true) {
        alarmName = alarmName + ", wifi";
    }

    if(id != -1) {
        if(id == ALARM_QUICK_ID) {
            alarmName = alarmName + ", quick";
        }
        if(id == ALARM_POMODORO_ID) {
            alarmName = alarmName + ", pomodoro";
        }
    }

    return alarmName;
}

bool willAlarmTrigger()
{
    uint64_t unixNow = getUnixTime(timeRTCLocal);
    if(rM.nextAlarm != 0) {
        if (rM.nextAlarm < unixNow || llabs(int64_t(unixNow) - int64_t(rM.nextAlarm)) < 20)
        {
            return true;
        }
    }
    return false;
}

void checkAlarms()
{
    // 20 seconds tolerance
    if (willAlarmTrigger() == true)
    {
        debugLog("Alarm ringing!");
        if(rM.placeTree[currentPlaceIndex] != alarmRing) {
            switchAlarmRing();
        } else {
            debugLog("Not switching alarm because it's already on!");
        }
    }
}

#endif