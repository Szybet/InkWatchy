#ifndef LP_CORE_H
#define LP_CORE_H

#include "defines/defines.h"

#if LP_CORE

bool loadLpCore();
bool runLpCore();

#if DEBUG
void monitorLpCore();
const char *getLpLog(uint8_t id);

#endif
#endif
#endif