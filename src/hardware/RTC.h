#ifndef RTC_H
#define RTC_H
#include "defines/defines.h"

extern tmElements_t timeRTC;

extern RTC_DATA_ATTR SmallRTC SRTC;

void initRTC();
void saveRTC();
void readRTC();

#if DEBUG
void initRTCDebug();
void loopRTCDebug();
void dumpRTCTime();
#endif

#endif
