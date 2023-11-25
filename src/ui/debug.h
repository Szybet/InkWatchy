#ifndef DEBUG_H
#define DEBUG_H

#include "../defines/defines.h"

struct debugDisplay {
    batteryInfo battery;
};

void loopDebugDisplay();
void initDebugDisplay();
#endif