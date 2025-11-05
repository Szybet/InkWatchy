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

    float wet_bulb = {0};
    float cape = {0};

    float uv_index = {0};
    float uv_index_clear_sky = {0};

    bool is_day = {0};

    float dew_point = {0};
    float precipitation_amount = {0};
    float snow_depth = {0};
    // float vapour_pressure_deficit = {0};
    // float evapotranspiration = {0};
    float lifted_index = {0};
    float convective_inhibition = {0};
    float terrestrial_radiation = {0};
    // float global_tilted_irradiance = {0};
    // float direct_normal_irradiance = {0};
    float diffuse_radiation = {0};
    float direct_radiation = {0};
    // float shortwave_radiation = {0};
    // float total_water_vapour = {0};

    // Daily things
    time_t daily_time = {0};
    float sunshine = {0};
    uint32_t sunrise = {0};
    uint32_t sunset = {0};
};

OM_OneHourWeather weatherGetDataHourly(uint8_t hourOffset);

// Weather menu functions
void initWeatherMenu();

void initWeatherDateMenu();
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
void showUVIndex();
void showUVIndexClearSky();
void showWetBulb();
void showCAPE();

void showDewPoint2m();
void showPrecipitationAmount();
void showSnowDepth();
// void showVapourPressureDeficit();
// void showEvapotranspiration();
void showLiftedIndex();
void showConvectiveInhibition();
void showTerrestrialRadiation();
// void showGlobalTiltedIrradiance();
// void showDirectNormalIrradiance();
void showDiffuseRadiation();
void showDirectRadiation();
// void showShortwaveRadiation();
// void showTotalColumnIntegratedWaterVapour();

#endif
