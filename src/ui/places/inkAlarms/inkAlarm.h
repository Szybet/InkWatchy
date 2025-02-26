#pragma once

#include "defines.h"

#if INK_ALARMS

struct inkAlarm {
    bool enabled : 1;
    uint8_t hour : 5;
    uint8_t minute : 6;
    uint8_t days; // Bit 1 means turned on for this day. Bit 0 is monday. No bitfield because pointers
    bool onlyOnce : 1; // Either days or this - it deletes the alarm
    bool requireWifi : 1;
};

uint64_t getUnixTimeOfAlarm(inkAlarm *theAlarm);
void calculateNextAlarm();
String alarmGetTime(inkAlarm *theAlarm);
String alarmGetDays(inkAlarm *theAlarm);
String alarmNameGet(inkAlarm *theAlarm);
bool willAlarmTrigger();
void checkAlarms();

#endif