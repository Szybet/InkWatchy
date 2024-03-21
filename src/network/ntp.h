#ifndef NTP_H
#define NTP_H

#include "defines/defines.h"

void syncNtp();
void checkDrift();
void manageDriftTiming(time_t *timeDifference, int *drift);

#endif
