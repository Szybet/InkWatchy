#ifndef HARDWARE_H
#define HARDWARE_H

#include "defines/defines.h"

extern RTC_DATA_ATTR float HWVer;
extern int sleepDelayMs;

void initAllHardware();
void initWakeUpHardware(bool isFromWakeUp);
void resetSleepDelay();

void vibrateMotor(int vTime = VIBRATION_BUTTON_TIME, bool add = false);

#if DEBUG
void initHardwareDebug();
void loopHardwareDebug();
#endif

#endif
