#ifndef TEMPS_H
#define TEMPS_H

#include "defines/defines.h"

#if TEMP_CHECKS_ENABLED

float getTemp();
extern float initialTemp;
void tempChecker();

#endif
#endif
