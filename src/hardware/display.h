#ifndef DISPLAY_H
#define DISPLAY_H

#include "defines/defines.h"

extern RTC_DATA_ATTR GxEPD2_BW<GxEPD2_154_D67, GxEPD2_154_D67::HEIGHT> display;

void initDisplay(bool isFromWakeUp);
extern bool dUChange; // Display update change boolean, to simplify the code
void disUp(bool reallyUpdate = false, bool ignoreCounter = false);

#if DEBUG

void initDisplayDebug();

#endif

#endif
