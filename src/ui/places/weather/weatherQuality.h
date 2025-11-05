#ifndef WEATHER_QUALITY_H
#define WEATHER_QUALITY_H

#include "defines.h"

#if WEATHER_INFO

struct OM_AirQualityForecastReturn
{
    bool fine;
    OM_AirQualityForecast data;
};

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

#if WEATHER_AIR_ENABLED
OM_AirQualityForecastReturn generalAirQualityGetData();
void initAirQualityDateMenu();
void initAirQualityConditionMenu();

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
void showNitrogenMonoxide();
void showPeroxyacylNitrates();

#endif // WEATHER_AIR_ENABLED
#endif // WEATHER_INFO
#endif // WEATHER_QUALITY_H
