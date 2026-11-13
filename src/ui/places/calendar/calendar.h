#pragma once

#include "defines.h"

#if CALENDAR

uint64_t calDisplayUnix(uint64_t utcUnix);
JsonDocument getCalendarJson(String date);
void switchCalendarDateMenu();
void initCalendar();
void initCalendarMenu();

#endif
