#pragma once

#include "defines.h"

#if ACC_ENABLED

void initAcc();
float getAxisDegrees(int16_t val, int16_t axisA, int16_t axisB);

#endif

uint16_t getSteps();

