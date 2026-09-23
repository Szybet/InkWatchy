#pragma once

#include "defines.h"

#if SET_TIME_GUI

extern int setTimeMinute;
extern int setTimeHour;
extern bool setTimeIsSleepInterval;

void initSetTime();
void loopSetTime();
void exitSetTime();

#endif