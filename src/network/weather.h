#ifndef WEATHER_H
#define WEATHER_H

#include "defines/defines.h"

#if WEATHER_INFO

extern bool isWeatherAvailable;

struct OM_HourlyForecastMinimal
{
    time_t hourly_time[24] = {0};

    float temp[24] = {0};
    uint8_t humidity[24] = {0};
    float apparent_temp[24] = {0};

    float pressure[24] = {0};

    uint8_t precipitation[24] = {0};
    uint8_t cloud_cover[24] = {0};
    uint16_t visibility[24] = {0};

    float wind_speed[24] = {0};
    uint16_t wind_deg[24] = {0};
    float wind_gust[24] = {0};

    uint8_t weather_code[24] = {0};

    bool is_day[24] = {0};
};

void syncWeather();
void loadWeatherData();

#if DEBUG
void dumpWeather();
#endif

String weatherConditionIdToStr(int weatherCode);

#endif
#endif
