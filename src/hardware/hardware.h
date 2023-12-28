#ifndef HARDWARE_H
#define HARDWARE_H

#include "defines/defines.h"

extern RTC_DATA_ATTR float HWVer;
extern int sleepDelayMs;

void initAllHardware();
void initWakeUpHardware(bool isFromWakeUp);
void resetSleepDelay();

extern TaskHandle_t motorTask;
extern bool motorTaskRunning;
void vibrateMotor(int vTime = VIBRATION_BUTTON_TIME, bool add = false);

typedef enum 
{
    minimalSpeed,
    normalSpeed,
    maxSpeed,
} cpuSpeed;

void setCpuMhz(cpuSpeed speed);

#if DEBUG
void initHardwareDebug();
void loopHardwareDebug();
#endif

#endif
