#pragma once

#include "defines.h"

#if WEATHER_INFO

struct OM_HourlyForecastReturn
{
    bool fine;
    OM_HourlyForecast data;
};

struct OM_AirQualityForecastReturn
{
    bool fine;
    OM_AirQualityForecast data;
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

    float pm2_5 = {0};
    float pm10 = {0};
    float carbon_monoxide = {0};
    float carbon_dioxide = {0};
    float nitrogen_dioxide = {0};
    float sulphur_dioxide = {0};
    float ozone = {0};
    float aerosol_optical_depth = {0};
    float dust = {0};
    float methane = {0};
    float formaldehyde = {0};
    float glyoxal = {0};
    //  float sea_salt_aerosol = {0};
    float nitrogen_monoxide = {0};
    float peroxyacyl_nitrates = {0};
};

OM_OneHourAirQuality airQualityGetDataHourly(uint8_t hourOffset);

// Weather menu functions
void initWeatherMenu();

void initWeatherDateMenu();
void initWeatherConditionMenu();
OM_HourlyForecastReturn generalWeatherGetData();

// Air Quality menu functions
void initAirQualityDateMenu();
void initAirQualityConditionMenu();
OM_AirQualityForecastReturn generalAirQualityGetData();

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

// Switches for air quality
void showEuropeanAQI();
void showUS_AQI();
void showEUAQI_PM2_5();
void showEUAQI_PM10();
void showEUAQI_NO2();
void showEUAQI_O3();
void showEUAQI_SO2();

void showPM2_5();
void showPM10();
void showCarbonMonoxide();
void showCarbonDioxide();
void showNitrogenDioxide();
void showSulphurDioxide();
void showOzone();
void showAerosolOpticalDepth();
void showDust();
void showMethane();
void showFormaldehyde();
void showGlyoxal();
// void showSeaSaltAerosol();
void showNitrogenMonoxide();
void showPeroxyacylNitrates();
#endif