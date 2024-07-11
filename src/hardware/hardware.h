#ifndef HARDWARE_H
#define HARDWARE_H

#include "defines/defines.h"

extern float HWVer;
extern uint64_t sleepDelayMs;

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

extern cpuSpeed savedCpuSpeed;
void setCpuSpeed(cpuSpeed speed);
cpuSpeed getCpuSpeed();
void restoreCpuSpeed();
String resetReasonToString(esp_reset_reason_t reason);

void softStartDelay();

#if DEBUG
extern uint64_t loopDumpDelayMs;
void initHardwareDebug();
void loopHardwareDebug();
String wakeupSourceToString(esp_sleep_source_t source);
#endif

#endif
