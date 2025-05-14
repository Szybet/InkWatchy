#pragma once

#include "defines.h"

#if DEBUG
void showEinkCounter();
#if SCREENSHOT_ENDPOINT
void screenshotEndpointInit();
#endif
#if DUMP_LOOP_ACC
void initAccLog();
void loopAccLog();
#endif
#endif

