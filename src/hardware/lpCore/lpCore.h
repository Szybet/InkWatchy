#pragma once

#include "defines.h"

#if LP_CORE

extern bool screenForceNextFullTimeWrite;
extern bool screenTimeChanged;

void setAlarmForLpCore();
void clearLpCoreRtcMem();
void lpCoreScreenPrepare(bool now, bool setDuChange = true);
void stopLpCore();
void loadLpCore();
void runLpCore();
void initManageLpCore();

#if DEBUG
void startLpCoreTest();
void monitorLpCore();
const char *getLpLog(uint8_t id);

#endif
#endif
