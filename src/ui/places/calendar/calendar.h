#pragma once

#include "defines.h"

#if CALENDAR

JsonDocument getCalendarJson(String date);
void switchCalendarDateMenu();
void initCalendar();
void initCalendarMenu();

#endif
