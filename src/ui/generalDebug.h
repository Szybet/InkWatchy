#ifndef GENERALDEBUG_H
#define GENERALDEBUG_H

#include "defines/defines.h"

void initGeneralDebugDisplay();
void loopGeneralDebugDisplay();

#if DEBUG
void loopGeneralDebug();
void initGeneralDebug();
#endif

#endif
