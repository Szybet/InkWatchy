#ifndef SCOM_TASK_H
#define SCOM_TASK_H

#include "defines/defines.h"

#if SCOM_TASK && DEBUG
extern bool scomChanged;
extern bool printEndPacket;
extern TaskHandle_t mainTask;

extern bool mainLoopWait;
extern bool mainLoopWaiting;

void loopScomTask(void *parameter);
void initScomTask();
void getMainTask();
#endif

#endif