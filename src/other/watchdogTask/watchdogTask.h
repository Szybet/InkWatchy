#pragma once

#include "defines.h"

extern TaskHandle_t watchdogTask;

bool allButtonCheck();
bool anyButtonCheck();
void loopWatchdogTask(void *parameter);
void initWatchdogTask();
void deInitWatchdogTask();

void watchdogPing();

