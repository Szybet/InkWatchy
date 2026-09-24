#pragma once

#include "defines.h"

void initpowerMenu();
void toggleWakeUpEntry();
void toggleDisableVibrationsEntry();
#if SLEEP_INTERVAL_GUI
void setSleepIntervalEntry();
#endif