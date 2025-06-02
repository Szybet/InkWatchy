#pragma once

#include "defines.h"

#if INK_ALARMS

#define __COUNT(min) +1 
enum { MAX_ALARMS = 0 QUICK_ALARM_LIST(__COUNT) };
#undef __COUNT

void setAlarmQuick(int minutes, int id);
void initAlarmQuickSet();

#endif