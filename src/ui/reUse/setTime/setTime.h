#pragma once

#include "defines.h"

#if SET_TIME_GUI

extern int setTimeMinute;
extern int setTimeHour;

void initSetTime();
void loopSetTime();
void exitSetTime();

#endif