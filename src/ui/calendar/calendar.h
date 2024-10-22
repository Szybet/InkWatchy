#ifndef CALENDAR_H
#define CALENDAR_H

#include "../../defines/defines.h"

#if CALENDAR

JsonDocument getCalendarJson(String date);
void switchCalendarDateMenu();
void initCalendar();
void initCalendarMenu();

#endif
#endif
