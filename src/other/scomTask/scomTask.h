#ifndef SCOM_TASK_H
#define SCOM_TASK_H

#include "defines/defines.h"

#if SCOM_TASK && DEBUG
extern bool scomChanged;
void loopScomTask(void *parameter);
void initScomTask();
#endif

#endif