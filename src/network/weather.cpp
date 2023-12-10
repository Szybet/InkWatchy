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
    log("Weather forecast finished: " + BOOL_STR(status));
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
  log("City name:  " + forecast->city_name);
  log("Latitude:  " + String(ow.lat));
  log("Longitude:  " + String(ow.lon));
  log("Timezone:  " + forecast->timezone);

  if (forecast)
  {
    for (int i = 0; i < (MAX_DAYS * 8); i++)
    {
      log("dt_txt: " + forecast->dt_txt[i]);
      log("temp: " + String(forecast->temp[i]));
      log("temp.min: " + String(forecast->temp_min[i]));
      log("temp.max: " + String(forecast->temp_max[i]));

      log("pressure: " + String(forecast->pressure[i]));
      log("sea_level: " + String(forecast->sea_level[i]));
      log("grnd_level: " + String(forecast->grnd_level[i]));
      log("humidity: " + String(forecast->humidity[i]));

      log("clouds: " + String(forecast->clouds_all[i]));
      log("wind_speed: " + String(forecast->wind_speed[i]));
      log("wind_deg: " + String(forecast->wind_deg[i]));
      log("wind_gust: " + String(forecast->wind_gust[i]));

      log("visibility: " + String(forecast->visibility[i]));
      log("pop: " + String(forecast->pop[i]));

      log("id: " + String(forecast->id[i]));
      log("main: " + forecast->main[i]);
      log("description: " + forecast->description[i]);
      log("icon: " + forecast->icon[i]);
    }
  }
}
#endif
