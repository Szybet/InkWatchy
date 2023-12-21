#ifndef WEATHER_H
#define WEATHER_H

#include "defines/defines.h"

struct savedWeatherData {
    uint32_t dt; // Use strTime
    float temp;
    float pressure;
    uint8_t humidity;
    uint16_t weatherConditionId; // Use weatherConditionIdToStr
    uint8_t cloudsPerc; // % of clouds
    float windSpeed;
    float windGusts;
    uint32_t visibility;
    uint8_t pop; // Probability of precipitation in percents
    uint32_t sunrise;
    uint32_t sunset;
    float minTemp;
};

#define WEATHER_PER_DAY 8
extern RTC_DATA_ATTR savedWeatherData weatherDataDays[MAX_DAYS][WEATHER_PER_DAY]; // Days / Data for these days

void syncWeather();

#if DEBUG
void dumpWeather();
#endif

String weatherConditionIdToStr(int weatherCode);

#endif
