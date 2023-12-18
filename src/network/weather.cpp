#include "weather.h"

OW_Weather ow;
bool isWeatherAvailable = false;
OW_forecast *forecast = NULL;

void initWeather()
{
  if (forecast != NULL)
  {
    delete forecast;
  }
  forecast = new OW_forecast;
}

void syncWeather()
{
  if (OPEN_WEATHER_API_KEY != "")
  {
    initWeather();
    bool status = ow.getForecast(forecast, OPEN_WEATHER_API_KEY, WEATHER_LATIT, WEATHER_LONGTIT, WEATHER_UNIT, WEATHER_LANG, false);
    debugLog("Weather forecast finished: " + BOOL_STR(status));
    if (status == true)
    {
      isWeatherAvailable = true;
      dumpWeather();
    }
    else
    {
      isWeatherAvailable = false;
    }
  }
  else
  {
    isWeatherAvailable = false;
  }
}

#if DEBUG
void dumpWeather()
{
  debugLog("City name:  " + forecast->city_name);
  debugLog("Latitude:  " + String(ow.lat));
  debugLog("Longitude:  " + String(ow.lon));
  debugLog("Timezone:  " + forecast->timezone);

  if (forecast)
  {
    for (int i = 0; i < (MAX_DAYS * 8); i++)
    {
      debugLog("dt_txt: " + forecast->dt_txt[i]);
      debugLog("temp: " + String(forecast->temp[i]));
      debugLog("temp.min: " + String(forecast->temp_min[i]));
      debugLog("temp.max: " + String(forecast->temp_max[i]));

      debugLog("pressure: " + String(forecast->pressure[i]));
      debugLog("sea_level: " + String(forecast->sea_level[i]));
      debugLog("grnd_level: " + String(forecast->grnd_level[i]));
      debugLog("humidity: " + String(forecast->humidity[i]));

      debugLog("clouds: " + String(forecast->clouds_all[i]));
      debugLog("wind_speed: " + String(forecast->wind_speed[i]));
      debugLog("wind_deg: " + String(forecast->wind_deg[i]));
      debugLog("wind_gust: " + String(forecast->wind_gust[i]));

      debugLog("visibility: " + String(forecast->visibility[i]));
      debugLog("pop: " + String(forecast->pop[i]));

      debugLog("id: " + String(forecast->id[i]));
      debugLog("main: " + forecast->main[i]);
      debugLog("description: " + forecast->description[i]);
      debugLog("icon: " + forecast->icon[i]);
    }
  }
}
#endif
