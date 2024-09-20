#ifndef LP_CORE_H
#define LP_CORE_H

#include "defines/defines.h"

#if LP_CORE

void lpCoreScreenPrepare();
void stopLpCore();
bool loadLpCore();
void initRtcGpio();
void deInitRtcGpio();
bool runLpCore();

#if DEBUG
void monitorLpCore();
const char *getLpLog(uint8_t id);

#endif
#endif
#endif