#ifndef RTC_H
#define RTC_H
#include "defines/defines.h"

extern tmElements_t timeRTC;

extern SmallRTC SRTC;
// Chatgpt said 38 is max, I trust him :)
#define POSIX_TIMEZONE_MAX_LENGTH 50
extern char posixTimeZone[POSIX_TIMEZONE_MAX_LENGTH];
extern bool dontTouchTimeZone;
void timeZoneApply();
extern uint64_t lastTimeRead;

void initRTC(bool isFromWakeUp, esp_sleep_wakeup_cause_t wakeUpReason);
void saveRTC(tmElements_t timeToSave);
void readRTC();
void wakeUpManageRTC();
void alarmManageRTC();

void setupMillisComparators();

// Not using reference made it... really broken
String getHourMinute(tmElements_t timeEl);
String getDayName(int offset = 0);
String getMonthName(int monthNumber);
uint64_t getUnixTime();
uint64_t simplifyUnix(uint64_t unixTime);
uint64_t dateToUnix(String date);
String getFormattedTime(time_t rawTime);
String unixToDate(uint64_t unixTime);
long getHourDifference(time_t currentTime, time_t targetTime);
uint64_t getLastTimeReadSec();
uint getCurrentSeconds();

#if DEBUG
void initRTCDebug();
void loopRTCDebug();
void dumpRTCTime(tmElements_t timeEl);
void dumpRTCTimeSmall(tmElements_t timeEl);
#endif

#endif
