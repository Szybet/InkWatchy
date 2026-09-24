#pragma once

#include "defines.h"

#if SET_TIME_GUI

extern int setTimeMinute;
extern int setTimeHour;
#if SLEEP_INTERVAL_GUI
extern bool setTimeIsSleepInterval;
#endif

void initSetTime();
void loopSetTime();
void exitSetTime();

#endif