#pragma once

#include "defines.h"

#if DEBUG_MENUS

void initAccDebug();
void loopAccDebug();
void exitAccDebug();

#if ACC_ENABLED
void loopAcc3D();
#endif

#endif