#pragma once

#include "defines.h"

extern int64_t sleepDelayMs;

typedef enum
{
    unknown = 0,
    rtc = 1,
    button = 2,
    ulp = 3,
} wakeUpReason;

struct wakeUpInfo {
    bool fromWakeup = false;
    wakeUpReason reason = unknown;
    esp_sleep_wakeup_cause_t bareEspCause;
    esp_reset_reason_t resetReason;
    bool alarmTriggeredWakeup = false;
};

extern wakeUpInfo bootStatus;

void initHardware();
void resetSleepDelay(int addMs = 0);
void setSleepDelay(int addMs);

typedef enum
{
    minimalSpeed = 0,
    normalSpeed = 1,
    maxSpeed = 2,
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

int64_t millisBetter();
void firstWakeUpManage();
bool isFullMode();

void cleanAllMemory();