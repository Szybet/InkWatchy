#ifndef LP_CORE_H
#define LP_CORE_H

#include "defines.h"

#if LP_CORE

extern bool screenForceNextFullTimeWrite;
extern bool screenTimeChanged;

void clearLpCoreRtcMem();
void lpCoreScreenPrepare(bool now);
void stopLpCore();
bool loadLpCore();
void initRtcGpio();
void deInitRtcGpio();
bool runLpCore();
void initManageLpCore();

#if DEBUG
void startLpCoreTest();
void monitorLpCore();
const char *getLpLog(uint8_t id);

#endif
#endif
#endif