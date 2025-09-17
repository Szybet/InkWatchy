#pragma once

#include "defines.h"

#if SET_DATE_GUI

extern int setDateDay;
extern int setDateMonth;
extern int setDateYear;

void initSetDate();
void loopSetDate();
void exitSetDate();

#endif