#pragma once

#include "defines.h"

#if ALARM_MODULE && INK_ALARMS

void wfAlarmcheckShow(bool *showBool, bool *redrawBool);
void wfAlarmrequestShow(buttonState button, bool *showBool);

#endif
