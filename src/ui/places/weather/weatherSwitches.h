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

    float wet_bulb = {0}; // New field
    float cape = {0};     // New field

    float uv_index = {0};           // Add this
    float uv_index_clear_sky = {0}; // Add this

    bool is_day = {0};

    // Daily things
    time_t daily_time = {0};

    float sunshine = {0}; // New field
    uint32_t sunrise = {0};
    uint32_t sunset = {0};
};

OM_OneHourWeather weatherGetDataHourly(uint8_t hourOffset);

struct OM_OneHourAirQuality
{
    bool fine;
    uint16_t european_aqi = {0};
    uint16_t us_aqi = {0};
    uint16_t european_aqi_pm2_5 = {0};
    uint16_t european_aqi_pm10 = {0};
    uint16_t european_aqi_nitrogen_dioxide = {0};
    uint16_t european_aqi_ozone = {0};
    uint16_t european_aqi_sulphur_dioxide = {0};
};

OM_OneHourAirQuality airQualityGetDataHourly(uint8_t hourOffset);

// Weather menu functions
void initWeatherMenu();
void initWeatherConditionMenu();
OM_HourlyForecastReturn generalWeatherGetData();

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