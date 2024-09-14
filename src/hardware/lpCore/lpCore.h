#ifndef LP_CORE_H
#define LP_CORE_H

#include "defines/defines.h"

#if LP_CORE

bool loadLpCore();
bool runLpCore();

#if DEBUG
void monitorLpCore();

#endif
#endif
#endif