#ifndef BATTERYDEBUG_H
#define BATTERYDEBUG_H

#include "../defines/defines.h"
#if DEBUG == 1 || DEBUG_MENUS == 1
struct debugDisplay {
    batteryInfo battery;
};

void loopDebugDisplay();
void initDebugDisplay();
#endif
#endif
