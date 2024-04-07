#ifndef RTC_H
#define RTC_H
#include "defines/defines.h"

extern tmElements_t* timeRTC;

extern RTC_DATA_ATTR SmallRTC SRTC;

void setupTimeStructure();
void initRTC(bool isFromWakeUp, esp_sleep_wakeup_cause_t wakeUpReason);
void saveRTC();
void readRTC();
void wakeUpManageRTC();
void alarmManageRTC();

// Not using reference made it... really broken
String getHourMinute(tmElements_t* timeEl = timeRTC);
String getDayName(int offset = 0);
String getMonthName(int monthNumber);
long getUnixTime();
String getFormattedTime(time_t rawTime);
String unixToDate(unsigned long unixTime);

#if DEBUG
void initRTCDebug();
void loopRTCDebug();
void dumpRTCTime(tmElements_t* timeEl = timeRTC);
void dumpRTCTimeSmall(tmElements_t *timeEl);
#endif

#endif
