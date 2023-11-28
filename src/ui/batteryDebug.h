#ifndef BATTERYDEBUG_H
#define BATTERYDEBUG_H

#include "../defines/defines.h"

struct debugDisplay {
    batteryInfo battery;
};

void loopDebugDisplay();
void initDebugDisplay();
#endif
