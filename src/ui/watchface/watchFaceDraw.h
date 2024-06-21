#ifndef WATCHFACE_LOGIC_H
#define WATCHFACE_LOGIC_H

#include "../../defines/defines.h"

#define TIME_FONT getFont("JackInput40")
#define DATE_FONT getFont("JackInput17")
#define DAY_NAME_FONT getFont("Speculum13")
#define MONTH_NAME_FONT getFont("Speculum9")

#define DAY_NAME_CORD 13, 87
#define DATE_CORD 8, 113
#define MONTH_NAME_CORD 48, 109
#define MONTH_NUMBER_1_CORD 89, 93
#define MONTH_NUMBER_2_CORD 89, 102
#define TO_DAY_BAR_CORD 136, 68
#define TO_DAY_BAR_SIZE 54, 6
#define BATT_BAR_CORD 135, 83

extern int percentOfDay;
extern int batteryPercantageWF;

void drawTimeBeforeApply();
void drawTimeAfterApply();
void drawDay();
void drawMonth();
void drawBattery();

void showFullWatchface();

#define SOME_RECT_X 0
#define SOME_RECT_Y 62
#define SOME_RECT_W 200
#define SOME_RECT_H 138
void cleanSomeDrawing(); // disableSomeDrawing

#endif
