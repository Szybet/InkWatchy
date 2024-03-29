#ifndef HARDWARE_H
#define HARDWARE_H

#include "defines/defines.h"

extern RTC_DATA_ATTR float HWVer;
extern long sleepDelayMs;

void initHardware(bool isFromWakeUp, esp_sleep_wakeup_cause_t wakeUpReason);
void resetSleepDelay(int addMs = 0);

extern TaskHandle_t motorTask;
extern bool motorTaskRunning;
void vibrateMotor(int vTime = VIBRATION_BUTTON_TIME, bool add = true);

typedef enum 
{
    minimalSpeed,
    normalSpeed,
    maxSpeed,
} cpuSpeed;

void setCpuMhz(cpuSpeed speed);
void initNvsManage();

#if DEBUG
void initHardwareDebug();
void loopHardwareDebug();
#endif

#endif
