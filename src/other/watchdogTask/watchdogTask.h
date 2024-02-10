#ifndef WATCHDOG_TASK_H
#define WATCHDOG_TASK_H

#include "defines/defines.h"

extern TaskHandle_t watchdogTask;

void loopWatchdogTask(void *parameter);
void initWatchdogTask();
void deInitWatchdogTask();

void watchdogPing();
void leaveFlashMessage(String message);
void readFlashMessage();

#endif
