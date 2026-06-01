#pragma once

#include "defines.h"

#if DEBUG_MENUS

void initAccDebug();
void loopAccDebug();
void exitAccDebug();

#if ACC_ENABLED
void loopAcc3D();
float getAxisDegrees(int16_t val, int16_t axisA, int16_t axisB);
#endif

#endif
