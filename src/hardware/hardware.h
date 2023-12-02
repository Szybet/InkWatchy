#ifndef HARDWARE_H
#define HARDWARE_H

#include "defines/defines.h"

extern RTC_DATA_ATTR float HWVer;
extern int sleepDelayMs;

void initAllHardware();
void initWakeUpHardware(bool isFromWakeUp);
void resetSleepDelay();

#if DEBUG
void initHardwareDebug();
void loopHardwareDebug();
#endif

#endif
