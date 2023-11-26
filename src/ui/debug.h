#ifndef DEBUG_H
#define DEBUG_H

#include "../defines/defines.h"

#if DEBUG
void showEinkCounter();
#endif

struct debugDisplay {
    batteryInfo battery;
};

void loopDebugDisplay();
void initDebugDisplay();
#endif
