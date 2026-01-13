#pragma once

#include "defines.h"

#if SET_CLOCK_GUI
void initSetTimezoneUtcWatch();
void exitSetTimezoneUtcWatch();

void initSetTimezoneContinentWatch();
void initSetTimezoneCityWatch();
void exitTimezoneCityWatch();
#endif
