#include "weather.h"

#if WEATHER_INFO
#define ADD_DAY_UNIX 86400

// http://api.open-meteo.com/v1/forecast?latitude=53.543082&longitude=9.994695&hourly=temperature_2m,relative_humidity_2m,apparent_temperature,precipitation_probability,weather_code,pressure_msl,cloud_cover,visibility,wind_speed_10m,wind_direction_10m,wind_gusts_10m,is_day&daily=sunrise,sunset&timeformat=unixtime&timezone=auto&forecast_days=16
void syncWeather()
{
  if (strlen(WEATHER_LONGTIT) == 0 || strlen(WEATHER_LATIT) == 0)
  {
    debugLog("Weather location not supplied, skipping");
    return;
  }

  debugLog("Syncing weather");
  OM_HourlyForecast *forecast = new OM_HourlyForecast;

  removeDir(WEATHER_DIR);
  fsCreateDir(WEATHER_DIR);
  fsCreateDir(WEATHER_HOURLY_DIR);

  for (u8_t iw = 0; iw < MAX_WEATHER_DAYS; iw++)
  {
    bool status = false;
    uint64_t unixTimeWeat = simplifyUnix(getUnixTime(timeRTCLocal) + (ADD_DAY_UNIX * iw));
    String currentDayDate = unixToDate(unixTimeWeat);
    for (u8_t i = 0; i < WEATHER_TRIES; i++)
    {
      debugLog("Trying to get weather for day: " + String(currentDayDate));
      status = getHourlyForecast(forecast, String(WEATHER_LATIT).toFloat(), String(WEATHER_LONGTIT).toFloat(), unixTimeWeat);
      if (status == true)
      {
        break;
      }
      else
      {
        debugLog("Failed to get weather, retrying...");
        delayTask(2000);
      }
      softStartDelay();
    }
    softStartDelay();

    if (status == false)
    {
      debugLog("Failed to get weather... bye");
      delete forecast;
      return;
    }

#if DEBUG && true == false
    for (int i = 0; i < OM_WEATHER_MAX_HOURS; ++i)
    {
      debugLog("Hour " + String(i) + ": ");
      debugLog("Time: " + String(forecast->hourly_time[i]));
      debugLog(", Temperature: " + String(forecast->temp[i]));
      debugLog(", Humidity: " + String(forecast->humidity[i]));
      debugLog(", Apparent Temperature: " + String(forecast->apparent_temp[i]));
      debugLog(", Pressure: " + String(forecast->pressure[i]));
      debugLog(", Pop: " + String(forecast->precipitation[i]));
      debugLog(", Cloud Cover: " + String(forecast->cloud_cover[i]));
      debugLog(", Visibility: " + String(forecast->visibility[i]));
      debugLog(", Wind Speed: " + String(forecast->wind_speed[i]));
      debugLog(", Wind Direction: " + String(forecast->wind_deg[i]));
      debugLog(", Wind Gust: " + String(forecast->wind_gust[i]));
      debugLog(", Weather Code: " + String(forecast->weather_code[i]));
      debugLog(", is Day: " + String(forecast->is_day[i]));
      debugLog(", Wet Bulb Temperature: " + String(forecast->wet_bulb_temperature_2m[i]));
      debugLog(", CAPE: " + String(forecast->cape[i]));
      debugLog(", UV Index: " + String(forecast->uv_index[i]));
      debugLog(", UV Index Clear Sky: " + String(forecast->uv_index_clear_sky[i]));
      debugLog(", Dew Point: " + String(forecast->dew_point[i]));
      debugLog(", Snow Depth: " + String(forecast->snow_depth[i]));
      // debugLog(", Vapour Pressure Deficit: " + String(forecast->vapour_pressure_deficit[i]));
      // debugLog(", Evapotranspiration: " + String(forecast->evapotranspiration[i]));
      debugLog(", Lifted Index: " + String(forecast->lifted_index[i]));
      debugLog(", Convective Inhibition: " + String(forecast->convective_inhibition[i]));
      debugLog(", Precipitation Amount: " + String(forecast->precipitation_amount[i]));
      debugLog(", Terrestrial Radiation: " + String(forecast->terrestrial_radiation[i]));
      // debugLog(", Global Tilted Irradiance: " + String(forecast->global_tilted_irradiance[i]));
      // debugLog(", Direct Normal Irradiance: " + String(forecast->direct_normal_irradiance[i]));
      debugLog(", Diffuse Radiation: " + String(forecast->diffuse_radiation[i]));
      debugLog(", Direct Radiation: " + String(forecast->direct_radiation[i]));
      // debugLog(", Shortwave Radiation: " + String(forecast->shortwave_radiation[i]));
      // debugLog(", Total Column Water Vapour: " + String(forecast->total_water_vapour[i]));
    }
#endif

    if (fsSetBlob(String(unixTimeWeat), (uint8_t *)forecast, sizeof(OM_HourlyForecast), String(WEATHER_HOURLY_DIR) + "/") == false)
    {
      debugLog("Failed to set weather " + String(currentDayDate));
    }
  }
  delete forecast;

#if WEATHER_AIR_ENABLED
  OM_AirQualityForecast *airForecast = new OM_AirQualityForecast;
  removeDir(AIR_QUALITY_DIR);
  fsCreateDir(AIR_QUALITY_DIR);
  fsCreateDir(AIR_QUALITY_HOURLY_DIR);

  for (u8_t iw = 0; iw < MAX_AIR_DAYS; iw++)
  {
    uint64_t unixTimeAir = simplifyUnix(getUnixTime(timeRTCLocal) + (ADD_DAY_UNIX * iw));
    String currentDayDate = unixToDate(unixTimeAir);
    bool status = false;
    for (u8_t i = 0; i < WEATHER_TRIES; i++)
    {
      debugLog("Trying to get air quality for day: " + String(currentDayDate));
      status = getAirQualityForecast(airForecast, String(WEATHER_LATIT).toFloat(), String(WEATHER_LONGTIT).toFloat(), unixTimeAir);
      if (status == true)
      {
        break;
      }
      else
      {
        debugLog("Failed to get air quality, retrying...");
        delayTask(2000);
      }
      softStartDelay();
    }
    softStartDelay();

    if (status == false)
    {
      debugLog("Failed to get air quality... bye");
      delete airForecast;
      return;
    }
#if DEBUG && true == false
    for (size_t i = 0; i < OM_AIR_QUALITY_MAX_HOURS; i++)
    {
      debugLog(", European AQI: " + String(airForecast->european_aqi[i]));
      debugLog(", US AQI: " + String(airForecast->us_aqi[i]));
      debugLog(", European AQI PM2.5: " + String(airForecast->european_aqi_pm2_5[i]));
      debugLog(", European AQI PM10: " + String(airForecast->european_aqi_pm10[i]));
      debugLog(", European AQI Nitrogen Dioxide: " + String(airForecast->european_aqi_nitrogen_dioxide[i]));
      debugLog(", European AQI Ozone: " + String(airForecast->european_aqi_ozone[i]));
      debugLog(", European AQI Sulphur Dioxide: " + String(airForecast->european_aqi_sulphur_dioxide[i]));
      debugLog(", PM2.5: " + String(airForecast->pm2_5[i]));
      debugLog(", PM10: " + String(airForecast->pm10[i]));
      debugLog(", Carbon Monoxide: " + String(airForecast->carbon_monoxide[i]));
      debugLog(", Carbon Dioxide: " + String(airForecast->carbon_dioxide[i]));
      debugLog(", Nitrogen Dioxide: " + String(airForecast->nitrogen_dioxide[i]));
      debugLog(", Sulphur Dioxide: " + String(airForecast->sulphur_dioxide[i]));
      debugLog(", Ozone: " + String(airForecast->ozone[i]));
      debugLog(", Aerosol Optical Depth: " + String(airForecast->aerosol_optical_depth[i]));
      debugLog(", Dust: " + String(airForecast->dust[i]));
      debugLog(", Methane: " + String(airForecast->methane[i]));
      debugLog(", Formaldehyde: " + String(airForecast->formaldehyde[i]));
      debugLog(", Glyoxal: " + String(airForecast->glyoxal[i]));
      // debugLog(", Sea Salt Aerosol: " + String(airForecast->sea_salt_aerosol[i]));
      debugLog(", Nitrogen Monoxide: " + String(airForecast->nitrogen_monoxide[i]));
      debugLog(", Peroxyacyl Nitrates: " + String(airForecast->peroxyacyl_nitrates[i]));
    }
#endif
    if (fsSetBlob(String(unixTimeAir), (uint8_t *)airForecast, sizeof(OM_AirQualityForecast), String(AIR_QUALITY_HOURLY_DIR) + "/") == false)
    {
      debugLog("Failed to set air quality: " + String(currentDayDate));
    }
  }
  delete airForecast;
#endif
  debugLog("Finished syncing weather/airquality");
}

#if DEBUG
void dumpWeather()
{
}
#endif

String weatherConditionIdToStr(u8_t weatherCode)
{
  switch (weatherCode)
  {
  case 0:
    return "Clear sky";
  case 1:
    return "Mainly clear";
  case 2:
    return "Partly cloudy";
  case 3:
    return "Overcast";
  case 45:
    return "Fog";
  case 48:
    return "Depositing rime fog";
  case 51:
    return "Drizzle: Light intensity";
  case 53:
    return "Drizzle: Moderate intensity";
  case 55:
    return "Drizzle: Dense intensity";
  case 56:
    return "Freezing Drizzle: Light intensity";
  case 57:
    return "Freezing Drizzle: Dense intensity";
  case 61:
    return "Rain: Slight intensity";
  case 63:
    return "Rain: Moderate intensity";
  case 65:
    return "Rain: Heavy intensity";
  case 66:
    return "Freezing Rain: Light intensity";
  case 67:
    return "Freezing Rain: Heavy intensity";
  case 71:
    return "Snow fall: Slight intensity";
  case 73:
    return "Snow fall: Moderate intensity";
  case 75:
    return "Snow fall: Heavy intensity";
  case 77:
    return "Snow grains";
  case 80:
    return "Rain showers: Slight intensity";
  case 81:
    return "Rain showers: Moderate intensity";
  case 82:
    return "Rain showers: Violent intensity";
  case 85:
    return "Snow showers: Slight intensity";
  case 86:
    return "Snow showers: Heavy intensity";
  case 95:
    return "Thunderstorm: Slight or moderate";
  case 96:
    return "Thunderstorm with slight hail";
  case 99:
    return "Thunderstorm with heavy hail";
  default:
    return "Unknown weather condition";
  }
}

#endif
