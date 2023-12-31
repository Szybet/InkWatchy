#ifndef RTC_H
#define RTC_H
#include "defines/defines.h"

extern RTC_DATA_ATTR tmElements_t timeRTC;

extern RTC_DATA_ATTR SmallRTC SRTC;

void initRTC(bool isFromWakeUp, esp_sleep_wakeup_cause_t wakeUpReason);
void saveRTC();
void readRTC();
void wakeUpManageRTC();
void alarmManageRTC();

String getHourMinute();
String getDayName();
String getMonthName(int monthNumber);

#if DEBUG
void initRTCDebug();
void loopRTCDebug();
void dumpRTCTime();
#endif

#endif
