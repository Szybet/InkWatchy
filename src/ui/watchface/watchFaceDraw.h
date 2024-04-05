#ifndef WATCHFACE_LOGIC_H
#define WATCHFACE_LOGIC_H

#include "../../defines/defines.h"

#define TIME_FONT &JackInput40pt7b
#define DATE_FONT &JackInput17pt7b
#define DAY_NAME_FONT &Speculum13pt7b
#define MONTH_NAME_FONT &Speculum9pt7b

#define DAY_NAME_CORD 13, 87
#define DATE_CORD 8, 113
#define MONTH_NAME_CORD 48, 109
#define MONTH_NUMBER_1_CORD 89, 93
#define MONTH_NUMBER_2_CORD 89, 102
#define TO_DAY_BAR_CORD 136, 68
#define TO_DAY_BAR_SIZE 54, 6
#define BATT_BAR_CORD 135, 83

extern RTC_DATA_ATTR int percentOfDay;
extern RTC_DATA_ATTR int batteryPercantageWF;

void drawTimeBeforeApply();
void drawTimeAfterApply();
void drawDay();
void drawMonth();
void drawBattery();

void showFullWatchface();

#endif
