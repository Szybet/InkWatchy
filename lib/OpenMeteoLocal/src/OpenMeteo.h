#ifndef OPENMETEO_H
#define OPENMETEO_H

#include <ArduinoJson.h>
#include <HTTPClient.h>

#define OM_WEATHER_MAX_DAYS 1
#define OM_WEATHER_MAX_HOURS OM_WEATHER_MAX_DAYS * 24

#define OM_AIR_QUALITY_MAX_DAYS 1
#define OM_AIR_QUALITY_MAX_HOURS OM_AIR_QUALITY_MAX_DAYS * 24

#define OM_LOGS_ENABLED 0

String getStringRequest(String url);

// Hourly Forecast with Sunrise Sunset
typedef struct OM_HourlyForecast
{
    // Hourly

    time_t hourly_time[OM_WEATHER_MAX_HOURS] = {0};

    float temp[OM_WEATHER_MAX_HOURS] = {0};
    uint8_t humidity[OM_WEATHER_MAX_HOURS] = {0};
    float apparent_temp[OM_WEATHER_MAX_HOURS] = {0};

    float pressure[OM_WEATHER_MAX_HOURS] = {0};

    uint8_t precipitation[OM_WEATHER_MAX_HOURS] = {0};
    uint8_t cloud_cover[OM_WEATHER_MAX_HOURS] = {0};
    uint16_t visibility[OM_WEATHER_MAX_HOURS] = {0};

    float wind_speed[OM_WEATHER_MAX_HOURS] = {0};
    uint16_t wind_deg[OM_WEATHER_MAX_HOURS] = {0};
    float wind_gust[OM_WEATHER_MAX_HOURS] = {0};

    uint8_t weather_code[OM_WEATHER_MAX_HOURS] = {0};

    bool is_day[OM_WEATHER_MAX_HOURS] = {0};

    // Daily
    time_t daily_time[OM_WEATHER_MAX_DAYS] = {0};

    uint32_t sunrise[OM_WEATHER_MAX_DAYS] = {0};
    uint32_t sunset[OM_WEATHER_MAX_DAYS] = {0};
    float wet_bulb_temperature_2m[OM_WEATHER_MAX_HOURS] = {0};  // New field
    float cape[OM_WEATHER_MAX_HOURS] = {0};                     // New field
    float sunshine_duration[OM_WEATHER_MAX_DAYS] = {0};         // New field
        float uv_index[OM_WEATHER_MAX_HOURS] = {0};             // Add this
    float uv_index_clear_sky[OM_WEATHER_MAX_HOURS] = {0};   // Add this

} OM_HourlyForecast;

/*

typedef struct OM_DailyForecast
{
    time_t daily_time[OM_WEATHER_MAX_DAYS] = {0};

    uint8_t weather_code[OM_WEATHER_MAX_DAYS] = {0};

    float temp_max[OM_WEATHER_MAX_DAYS] = {0};
    float temp_min[OM_WEATHER_MAX_DAYS] = {0};

    float apparent_temp_max[OM_WEATHER_MAX_DAYS] = {0};
    float apparent_temp_min[OM_WEATHER_MAX_DAYS] = {0};

    uint32_t sunrise[OM_WEATHER_MAX_DAYS] = {0};
    uint32_t sunset[OM_WEATHER_MAX_DAYS] = {0};

    uint8_t precipitation_max[OM_WEATHER_MAX_DAYS] = {0};
    uint8_t precipitation_hours[OM_WEATHER_MAX_DAYS] = {0};

    float wind_speed_max[OM_WEATHER_MAX_DAYS] = {0};
    float wind_gust_max[OM_WEATHER_MAX_DAYS] = {0};
    uint16_t wind_deg_dominant[OM_WEATHER_MAX_DAYS] = {0};

} OM_DailyForecast;

typedef struct OM_CurrentWeather
{
    time_t time = 0;

    float temp = 0;
    uint8_t humidity = 0;
    float apparent_temp = 0;

    bool is_day = 0;

    uint8_t weather_code = 0;

    float pressure = 0;
    uint8_t cloud_cover = 0;

    float wind_speed = 0;
    uint16_t wind_deg = 0;
    float wind_gust = 0;

} OM_CurrentWeather;

typedef struct OM_AirQualityForecast
{
    time_t time[OM_AIR_QUALITY_MAX_HOURS] = {0};
    uint8_t EU_AQI[OM_AIR_QUALITY_MAX_HOURS] = {0};
} OM_AirQualityForecast;

typedef struct OM_CurrentAirQuality
{
    time_t time = 0;
    uint8_t EU_AQI = 0;
} OM_CurrentAirQuality;
*/

typedef struct OM_AirQualityForecast {
    time_t hourly_time[OM_AIR_QUALITY_MAX_HOURS] = {0};
    uint16_t european_aqi[OM_AIR_QUALITY_MAX_HOURS] = {0};
    uint16_t us_aqi[OM_AIR_QUALITY_MAX_HOURS] = {0};
    uint16_t european_aqi_pm2_5[OM_AIR_QUALITY_MAX_HOURS] = {0};
    uint16_t european_aqi_pm10[OM_AIR_QUALITY_MAX_HOURS] = {0};
    uint16_t european_aqi_nitrogen_dioxide[OM_AIR_QUALITY_MAX_HOURS] = {0};
    uint16_t european_aqi_ozone[OM_AIR_QUALITY_MAX_HOURS] = {0};
    uint16_t european_aqi_sulphur_dioxide[OM_AIR_QUALITY_MAX_HOURS] = {0};
} OM_AirQualityForecast;


#define HOURLY_API_LINK "&hourly=temperature_2m,relative_humidity_2m,apparent_temperature,precipitation_probability,weather_code,pressure_msl,cloud_cover,visibility,wind_speed_10m,wind_direction_10m,wind_gusts_10m,is_day,wet_bulb_temperature_2m,cape,uv_index,uv_index_clear_sky&daily=sunrise,sunset,sunshine_duration&timeformat=unixtime&timezone=auto"

bool getHourlyForecast(OM_HourlyForecast *structure, float latitude, float longitude, uint64_t unixTime, String apiLink = HOURLY_API_LINK);

#define AIR_HOURLY_API_LINK "&hourly=european_aqi,european_aqi_pm2_5,european_aqi_pm10,us_aqi,european_aqi_nitrogen_dioxide,european_aqi_ozone,european_aqi_sulphur_dioxide&timeformat=unixtime&timezone=auto"

bool getAirQualityForecast(OM_AirQualityForecast *structure, float latitude, float longitude, uint64_t unixTime, String apiLink = AIR_HOURLY_API_LINK);


/*
#define DAILY_API_LINK "&daily=weather_code,temperature_2m_max,temperature_2m_min,apparent_temperature_max,apparent_temperature_min,sunrise,sunset,precipitation_hours,precipitation_probability_max,wind_speed_10m_max,wind_gusts_10m_max,wind_direction_10m_dominant&timeformat=unixtime&timezone=auto&forecast_days=16"

void getDailyForecast(OM_DailyForecast *structure, float latitude, float longitude, String apiLink = DAILY_API_LINK);

#define CURRENT_API_LINK "&current=temperature_2m,relative_humidity_2m,apparent_temperature,is_day,weather_code,cloud_cover,pressure_msl,wind_speed_10m,wind_direction_10m,wind_gusts_10m&timeformat=unixtime&timezone=auto"

void getCurrentWeather(OM_CurrentWeather *structure, float latitude, float longitude, String apiLink = CURRENT_API_LINK);

#define AIR_HOURLY_API_LINK "&hourly=european_aqi&timeformat=unixtime&timezone=auto"

void getAirQualityForecast(OM_AirQualityForecast *structure, float latitude, float longitude, String apiLink = AIR_HOURLY_API_LINK);

#define AIR_CURRENT_API_LINK "&current=european_aqi&timeformat=unixtime&timezone=auto"

void getCurrentAirQuality(OM_CurrentAirQuality *structure, float latitude, float longitude, String apiLink);
*/

#endif