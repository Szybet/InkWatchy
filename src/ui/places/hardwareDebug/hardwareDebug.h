#pragma once

#include "defines.h"

#if DEBUG == 1 || DEBUG_MENUS == 1

void initGeneralDebugDisplay();
void loopGeneralDebugDisplay();

#if DEBUG
void loopGeneralDebug();
void initGeneralDebug();
#endif

#endif
