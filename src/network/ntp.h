#ifndef NTP_H
#define NTP_H

#include "defines/defines.h"

void syncNtp(bool doDriftThings = true);
void checkDrift();
void manageDriftTiming(float *timeDifference, int *drift);

#endif
