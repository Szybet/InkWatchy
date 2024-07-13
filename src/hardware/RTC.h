#ifndef RTC_H
#define RTC_H
#include "defines/defines.h"

extern tmElements_t timeRTC;

extern SmallRTC SRTC;
// Chatgpt said 38 is max, I trust him :)
#define POSIX_TIMEZONE_MAX_LENGTH 50
extern char posixTimeZone[POSIX_TIMEZONE_MAX_LENGTH];
void timeZoneApply();
extern uint64_t lastTimeRead;

void initRTC(bool isFromWakeUp, esp_sleep_wakeup_cause_t wakeUpReason);
void saveRTC();
void readRTC();
void wakeUpManageRTC();
void alarmManageRTC();

void setupMillisComparators();

// Not using reference made it... really broken
String getHourMinute(tmElements_t timeEl = timeRTC);
String getDayName(int offset = 0);
String getMonthName(int monthNumber);
long getUnixTime();
String getFormattedTime(time_t rawTime);
String unixToDate(unsigned long unixTime);
long getHourDifference(time_t currentTime, time_t targetTime);

#if DEBUG
void initRTCDebug();
void loopRTCDebug();
void dumpRTCTime(tmElements_t timeEl = timeRTC);
void dumpRTCTimeSmall(tmElements_t timeEl);
#endif

#endif
