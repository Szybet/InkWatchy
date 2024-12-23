#pragma once

#include "defines.h"

#if WEATHER_INFO

struct OM_HourlyForecastReturn
{
    bool fine;
    OM_HourlyForecast data;
};

struct OM_OneHourWeather
{
    bool fine;
    // Weather
    time_t hourly_time = {0};

    float temp = {0};
    uint8_t humidity = {0};
    float apparent_temp = {0};

    float pressure = {0};

    uint8_t precipitation = {0};
    uint8_t cloud_cover = {0};
    uint16_t visibility = {0};

    float wind_speed = {0};
    uint16_t wind_deg = {0};
    float wind_gust = {0};

    uint8_t weather_code = {0};

    bool is_day = {0};

    // Daily things
    time_t daily_time = {0};

    uint32_t sunrise = {0};
    uint32_t sunset = {0};
};

OM_OneHourWeather weatherGetDataHourly(uint8_t hourOffset);

// Switches for weather
void showTemp();
void showPressure();
void showHumidity();
void showWeatherCond();
void showClouds();
void showWindSpeed();
void showWindGuts();
void showVisibility();
void showPop();

#endif
