#pragma once
#include "defines.h"

extern tmElements_t timeRTCLocal;
extern tmElements_t timeRTCUTC0;

extern int64_t timeZoneOffset;

extern SmallRTC SRTC;
// Chatgpt said 38 is max, I trust him :)
#define POSIX_TIMEZONE_MAX_LENGTH 50
extern char posixTimeZone[POSIX_TIMEZONE_MAX_LENGTH];
extern bool dontTouchTimeZone;
void timeZoneApply();
void removeTimeZoneVars();
extern uint64_t lastTimeRead;

void initRTC();
void saveRTC(tmElements_t timeToSave);
void setRTCTime(time_t epoch);
void setRTCTimeZoneByUtcOffset(int offset);

void readRTC();
void wakeUpManageRTC();
void manageRTC();

void setupMillisComparators();

String getHourMinuteUnix(int64_t unixTime);
String getHourMinute(tmElements_t timeEl);
String unixToDayName(uint64_t unixTime, int offset = 0);
String getDayName(int offset = 0);
String getMonthName(int monthNumber);
String getMonthNameFull(int monthNumber);
uint64_t getUnixTime(tmElements_t tme);
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
