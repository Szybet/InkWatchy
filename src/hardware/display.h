#ifndef DISPLAY_H
#define DISPLAY_H

#include "defines/defines.h"

extern GxEPD2_BW<GxEPD2_154_D67, GxEPD2_154_D67::HEIGHT> display;

void initDisplay(bool isFromWakeUp);

#if DEBUG

void initDisplayDebug();

#endif

#endif
