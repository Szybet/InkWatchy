#include "OpenMeteo.h"
#include <Arduino.h>
#include <WiFi.h>
#include <time.h>
#include <TimeLib.h>

// &start_date=2024-07-17&end_date=2024-07-17
String unixToDateOM(uint64_t unixTime)
{
  int gotDay = day(unixTime);
  int gotMonth = month(unixTime);
  int gotYear = year(unixTime);

  char dateString[11];
  sprintf(dateString, "%04d-%02d-%02d", gotYear, gotMonth, gotDay);

  return String(dateString);
}

bool getHourlyForecast(OM_HourlyForecast *structure, float latitude, float longitude, uint64_t unixTime, String apiLink)
{
    String date = unixToDateOM(unixTime);
    String url = "http://api.open-meteo.com/v1/forecast?latitude=" + String(latitude) + "&longitude=" + String(longitude) + apiLink + "&start_date=" + date + "&end_date=" + date;
#if OM_LOGS_ENABLED
    Serial.println("Final hourly url: " + url);
#endif
    String jsonString = getStringRequest(url);
    JsonDocument jsonDoc;
    DeserializationError error = deserializeJson(jsonDoc, jsonString);

#if OM_LOGS_ENABLED
    Serial.print("deserializeJson() returned: ");
    Serial.println(error.c_str());
#endif

    if (error.code() != DeserializationError::Ok)
    {
        return false;
    }

    for (size_t i = 0; i < OM_WEATHER_MAX_HOURS; i++)
    {
        structure->hourly_time[i] = jsonDoc["hourly"]["time"][i];
        structure->temp[i] = jsonDoc["hourly"]["temperature_2m"][i];
        structure->humidity[i] = jsonDoc["hourly"]["relative_humidity_2m"][i];
        structure->apparent_temp[i] = jsonDoc["hourly"]["apparent_temperature"][i];
        structure->precipitation[i] = jsonDoc["hourly"]["precipitation_probability"][i];
        structure->weather_code[i] = jsonDoc["hourly"]["weather_code"][i];
        structure->pressure[i] = jsonDoc["hourly"]["pressure_msl"][i];
        structure->cloud_cover[i] = jsonDoc["hourly"]["cloud_cover"][i];
        structure->visibility[i] = jsonDoc["hourly"]["visibility"][i];
        structure->wind_speed[i] = jsonDoc["hourly"]["wind_speed_10m"][i];
        structure->wind_deg[i] = jsonDoc["hourly"]["wind_direction_10m"][i];
        structure->wind_gust[i] = jsonDoc["hourly"]["wind_gusts_10m"][i];
        structure->is_day[i] = jsonDoc["hourly"]["is_day"][i];
        structure->wet_bulb_temperature_2m[i] = jsonDoc["hourly"]["wet_bulb_temperature_2m"][i];  // New
        structure->cape[i] = jsonDoc["hourly"]["cape"][i];     
        structure->uv_index[i] = jsonDoc["hourly"]["uv_index"][i];                   // Add this
        structure->uv_index_clear_sky[i] = jsonDoc["hourly"]["uv_index_clear_sky"][i]; // Add this
        

    }
    for (size_t i = 0; i < OM_WEATHER_MAX_DAYS; i++)
    {
        structure->daily_time[i] = jsonDoc["daily"]["time"][i];
        structure->sunrise[i] = jsonDoc["daily"]["sunrise"][i];
        structure->sunset[i] = jsonDoc["daily"]["sunset"][i];
        structure->sunshine_duration[i] = jsonDoc["daily"]["sunshine_duration"][i];  // New
    }
    return true;
}

bool getAirQualityForecast(OM_AirQualityForecast *structure, float latitude, float longitude, uint64_t unixTime, String apiLink) {
    String date = unixToDateOM(unixTime);
    String url = "http://air-quality-api.open-meteo.com/v1/air-quality?latitude=" + String(latitude) + "&longitude=" + String(longitude) + apiLink + "&start_date=" + date + "&end_date=" + date;
    
#if OM_LOGS_ENABLED
    Serial.println("Final hourly url: " + url);
#endif
    String jsonString = getStringRequest(url);
    JsonDocument jsonDoc;
    DeserializationError error = deserializeJson(jsonDoc, jsonString);

#if OM_LOGS_ENABLED
    Serial.print("deserializeJson() returned: ");
    Serial.println(error.c_str());
#endif

    if (error.code() != DeserializationError::Ok)
    {
        return false;
    }

    for (size_t i = 0; i < OM_AIR_QUALITY_MAX_HOURS; i++)
    {
        structure->hourly_time[i] = jsonDoc["hourly"]["time"][i];
        structure->european_aqi[i] = jsonDoc["hourly"]["european_aqi"][i];
        structure->us_aqi[i] = jsonDoc["hourly"]["us_aqi"][i];
        structure->european_aqi_pm2_5[i] = jsonDoc["hourly"]["european_aqi_pm2_5"][i];
        structure->european_aqi_pm10[i] = jsonDoc["hourly"]["european_aqi_pm10"][i];
        structure->european_aqi_nitrogen_dioxide[i] = jsonDoc["hourly"]["european_aqi_nitrogen_dioxide"][i];
        structure->european_aqi_ozone[i] = jsonDoc["hourly"]["european_aqi_ozone"][i];
        structure->european_aqi_sulphur_dioxide[i] = jsonDoc["hourly"]["european_aqi_sulphur_dioxide"][i];        
    }
    return true;
}

/*
void getDailyForecast(OM_DailyForecast *structure, float latitude, float longitude, String apiLink)
{
    String url = "http://api.open-meteo.com/v1/forecast?latitude=" + String(latitude) + "&longitude=" + String(longitude) + apiLink;
    String jsonString = getStringRequest(url);
    JsonDocument jsonDoc;
    DeserializationError error = deserializeJson(jsonDoc, jsonString);

#ifdef OM_LOGS_ENABLED
    if (error)
    {
        Serial.print("deserializeJson() returned ");
        Serial.println(error.c_str());
        return;
    }
#endif

    for (size_t i = 0; i < OM_WEATHER_MAX_DAYS; i++)
    {
        structure->daily_time[i] = jsonDoc["daily"]["time"][i];
        structure->weather_code[i] = jsonDoc["daily"]["weather_code"][i];
        structure->temp_max[i] = jsonDoc["daily"]["temperature_2m_max"][i];
        structure->temp_min[i] = jsonDoc["daily"]["temperature_2m_min"][i];
        structure->apparent_temp_max[i] = jsonDoc["daily"]["apparent_temperature_max"][i];
        structure->apparent_temp_min[i] = jsonDoc["daily"]["apparent_temperature_min"][i];
        structure->sunrise[i] = jsonDoc["daily"]["sunrise"][i];
        structure->sunset[i] = jsonDoc["daily"]["sunset"][i];
        structure->precipitation_hours[i] = jsonDoc["daily"]["precipitation_hours"][i];
        structure->precipitation_max[i] = jsonDoc["daily"]["precipitation_probability_max"][i];
        structure->wind_speed_max[i] = jsonDoc["daily"]["wind_speed_10m_max"][i];
        structure->wind_gust_max[i] = jsonDoc["daily"]["wind_gusts_10m_max"][i];
        structure->wind_deg_dominant[i] = jsonDoc["daily"]["wind_direction_10m_dominant"][i];
    }
}

void getCurrentWeather(OM_CurrentWeather *structure, float latitude, float longitude, String apiLink)
{
    String url = "http://api.open-meteo.com/v1/forecast?latitude=" + String(latitude) + "&longitude=" + String(longitude) + apiLink;
    String jsonString = getStringRequest(url);
    JsonDocument jsonDoc;
    DeserializationError error = deserializeJson(jsonDoc, jsonString);

#ifdef OM_LOGS_ENABLED
    if (error)
    {
        Serial.print("deserializeJson() returned ");
        Serial.println(error.c_str());
        return;
    }
#endif

    structure->time = jsonDoc["current"]["time"];
    structure->temp = jsonDoc["current"]["temperature_2m"];
    structure->humidity = jsonDoc["current"]["relative_humidity_2m"];
    structure->apparent_temp = jsonDoc["current"]["apparent_temperature"];
    structure->is_day = jsonDoc["current"]["is_day"];
    structure->weather_code = jsonDoc["current"]["weather_code"];
    structure->cloud_cover = jsonDoc["current"]["cloud_cover"];
    structure->pressure = jsonDoc["current"]["pressure_msl"];
    structure->wind_speed = jsonDoc["current"]["wind_speed_10m"];
    structure->wind_deg = jsonDoc["current"]["wind_direction_10m"];
    structure->wind_gust = jsonDoc["current"]["wind_gusts_10m"];
}

void getAirQualityForecast(OM_AirQualityForecast *structure, float latitude, float longitude, String apiLink)
{
    String url = "http://air-quality-api.open-meteo.com/v1/air-quality?latitude=" + String(latitude) + "&longitude=" + String(longitude) + apiLink;
    String jsonString = getStringRequest(url);
    JsonDocument jsonDoc;
    DeserializationError error = deserializeJson(jsonDoc, jsonString);

#ifdef OM_LOGS_ENABLED
    if (error)
    {
        Serial.print("deserializeJson() returned ");
        Serial.println(error.c_str());
        return;
    }
#endif

    for (size_t i = 0; i < OM_AIR_QUALITY_MAX_HOURS; i++)
    {
        structure->time[i] = jsonDoc["hourly"]["time"][i];
        structure->EU_AQI[i] = jsonDoc["hourly"]["european_aqi"][i];
    }
}

void getCurrentAirQuality(OM_CurrentAirQuality *structure, float latitude, float longitude, String apiLink)
{
    String url = "http://air-quality-api.open-meteo.com/v1/air-quality?latitude=" + String(latitude) + "&longitude=" + String(longitude) + apiLink;
    String jsonString = getStringRequest(url);
    JsonDocument jsonDoc;
    DeserializationError error = deserializeJson(jsonDoc, jsonString);

#ifdef OM_LOGS_ENABLED
    if (error)
    {
        Serial.print("deserializeJson() returned ");
        Serial.println(error.c_str());
        return;
    }
#endif

    structure->time = jsonDoc["hourly"]["time"];
    structure->EU_AQI = jsonDoc["hourly"]["european_aqi"];
}
*/

String getStringRequest(String url)
{
    WiFiClient client;
    HTTPClient http;

    http.begin(client, url);
    http.setTimeout(17500);
    http.setConnectTimeout(17500);

    // Error codes are here: /home/szybet/.platformio/packages/framework-arduinoespressif32/libraries/HTTPClient/src/HTTPClient.h
    int httpResponseCode = http.GET();

    String payload = "{}";

#if OM_LOGS_ENABLED
        Serial.println("Error code: " + String(httpResponseCode));
#endif

    if (httpResponseCode > 0)
    {
        payload = http.getString();
    } else {
        payload = "";
    }

    // Free resources
    http.end();

    return payload;
}