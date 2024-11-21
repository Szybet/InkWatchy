#ifndef WATCHDOG_TASK_H
#define WATCHDOG_TASK_H

#include "defines.h"

extern TaskHandle_t watchdogTask;

bool allButtonCheck();
void loopWatchdogTask(void *parameter);
void initWatchdogTask();
void deInitWatchdogTask();

void watchdogPing();

#endif
