#pragma once

#include "defines.h"

#if DEBUG
void showEinkCounter();

#if DUMP_LOOP_ACC
void initAccLog();
void loopAccLog();
#endif
#endif

