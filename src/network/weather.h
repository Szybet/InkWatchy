#ifndef WEATHER_H
#define WEATHER_H

#include "defines.h"

#if WEATHER_INFO

void syncWeather();

#if DEBUG
void dumpWeather();
#endif

String weatherConditionIdToStr(u8_t weatherCode);

#endif
#endif
