#ifndef WATCHFACE_H
#define WATCHFACE_H

#include "../../defines/defines.h"

void initWatchfaceDisplay();
void loopWatchfaceLoop();
int calculatePercentageOfDay(int hour, int minute);

#endif
