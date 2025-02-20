#pragma once

#include "defines.h"

#if INK_ALARMS

// This is a forward declaration
struct inkAlarm;

extern inkAlarm *choosedAlarm;

void initAlarmEditMenu();

#endif