#ifndef WATCHFACE_H
#define WATCHFACE_H

#include "../../defines/defines.h"

#include "watchFaceDraw.h"
#include "modules.h"

extern RTC_DATA_ATTR tmElements_t wFTime;
extern RTC_DATA_ATTR bool disableSomeDrawing;

void initWatchfaceDisplay();
void loopWatchfaceLoop();

// Logic
int calculatePercentageOfDay(int hour, int minute);

#endif
