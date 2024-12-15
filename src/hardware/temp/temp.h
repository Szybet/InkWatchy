#pragma once

#include "defines.h"

#if TEMP_CHECKS_ENABLED

float getTemp();
extern float initialTemp;
void tempChecker();

#endif
