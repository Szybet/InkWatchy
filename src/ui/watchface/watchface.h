#ifndef WATCHFACE_H
#define WATCHFACE_H

#include "../../defines/defines.h"

#include "watchFaceDraw.h"
#include "modules.h"

extern tmElements_t wFTime;
extern bool disableSomeDrawing;

void initWatchfaceDisplay();
void loopWatchfaceLoop();

// Logic
int calculatePercentageOfDay(int hour, int minute);

#endif
