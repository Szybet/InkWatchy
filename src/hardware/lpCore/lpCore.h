#ifndef LP_CORE_H
#define LP_CORE_H

#include "defines.h"

#if LP_CORE

extern bool screenForceNextFullTimeWrite;
extern bool screenTimeChanged;

void clearLpCoreRtcMem();
void lpCoreScreenPrepare(bool now, bool setDuChange = true);
void stopLpCore();
bool loadLpCore();
bool runLpCore();
void initManageLpCore();

#if DEBUG
void startLpCoreTest();
void monitorLpCore();
const char *getLpLog(uint8_t id);

#endif
#endif
#endif