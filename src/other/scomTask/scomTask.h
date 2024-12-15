#pragma once

#include "defines.h"

#if SCOM_TASK_ENABLED
extern bool scomChanged;
extern bool printEndPacket;
extern TaskHandle_t mainTask;

void loopScomTask(void *parameter);
void initScomTask();
void getMainTask();
#endif

