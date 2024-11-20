#ifndef AXC_H
#define AXC_H

#include "defines/defines.h"

#if AXC_ENABLED

extern StableBMA SBMA;

void initAxc();

#endif

uint16_t getSteps();

#endif
