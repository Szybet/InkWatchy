#ifndef WEATHER_H
#define WEATHER_H

#include "defines/defines.h"

#if WEATHER_INFO

extern bool isWeatherAvailable;

void syncWeather();
void loadWeatherData();

#if DEBUG
void dumpWeather();
#endif

String weatherConditionIdToStr(int weatherCode);

#endif
