#ifndef SCOM_TASK_H
#define SCOM_TASK_H

#include "defines/defines.h"

#if SCOM_TASK_ENABLED
extern bool scomChanged;
extern bool printEndPacket;
extern TaskHandle_t mainTask;

void loopScomTask(void *parameter);
void initScomTask();
void getMainTask();
#endif

#endif