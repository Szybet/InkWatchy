#include "weather.h"

OW_Weather ow;
bool isWeatherAvailable = false;
OW_forecast *forecast = NULL;
RTC_DATA_ATTR savedWeatherData weatherDataDays[MAX_DAYS][WEATHER_PER_DAY]; // Days / Data for these days

void syncWeather()
{
  if (OPEN_WEATHER_API_KEY != "" && WEATHER_LONGTIT != "" && WEATHER_LATIT != "")
  {
    forecast = new OW_forecast;
    bool status = ow.getForecast(forecast, OPEN_WEATHER_API_KEY, WEATHER_LATIT, WEATHER_LONGTIT, WEATHER_UNIT, WEATHER_LANG, false);
    debugLog("Weather forecast finished: " + BOOL_STR(status));
    if (status == true && forecast)
    {
      uint8_t c = 0;
      for (int i = 0; i < MAX_DAYS; i++)
      {
        for (int j = 0; j < WEATHER_PER_DAY; j++)
        {
          weatherDataDays[i][j].dt = forecast->dt[c];
          weatherDataDays[i][j].temp = forecast->feels_like[c];
          weatherDataDays[i][j].minTemp = forecast->temp_min[c];
          weatherDataDays[i][j].pressure = forecast->pressure[c];
          weatherDataDays[i][j].humidity = forecast->humidity[c];
          weatherDataDays[i][j].weatherConditionId = forecast->id[c];
          weatherDataDays[i][j].cloudsPerc = forecast->clouds_all[c];
          weatherDataDays[i][j].windSpeed = forecast->wind_speed[c];
          weatherDataDays[i][j].windGusts = forecast->wind_gust[c];
          weatherDataDays[i][j].visibility = forecast->visibility[c];
          weatherDataDays[i][j].pop = forecast->pop[c] * 100;
          weatherDataDays[i][j].sunrise = forecast->sunrise;
          weatherDataDays[i][j].sunset = forecast->sunset;
          c = c + 1;
        }
      }
      isWeatherAvailable = true;
      isDebug(dumpWeather());
      delete forecast;
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
  debugLog("City name: " + forecast->city_name);
  debugLog("Latitude: " + String(ow.lat));
  debugLog("Longitude: " + String(ow.lon));
  debugLog("Timezone: " + String(forecast->timezone));
  for (int i = 0; i < MAX_DAYS; i++)
  {
    Serial.println("");
    debugLog("Day number: " + String(i + 1) + " weather info:");
    for (int j = 0; j < WEATHER_PER_DAY; j++)
    {
      Serial.println("");
      debugLog("Information for day " + String(i) + " number " + String(j));
      String time = strTime(weatherDataDays[i][j].dt);
      time.remove(time.length() - 1);
      debugLog("Information for time: " + time);
      debugLog("Temperature: " + String(weatherDataDays[i][j].temp) + "C");
      debugLog("Minimal temperature: " + String(weatherDataDays[i][j].minTemp));
      debugLog("Pressure: " + String(weatherDataDays[i][j].pressure) + "hPa");
      debugLog("Humidity: " + String(weatherDataDays[i][j].humidity) + "%");
      debugLog("Weather condition: " + weatherConditionIdToStr(weatherDataDays[i][j].weatherConditionId));
      debugLog("Cloudiness: " + String(weatherDataDays[i][j].cloudsPerc) + "%");
      debugLog("WindSpeed: " + String(weatherDataDays[i][j].windSpeed) + "m/s");
      debugLog("Wind guts: " + String(weatherDataDays[i][j].windGusts) + "m/s");
      debugLog("Visibility: " + String(weatherDataDays[i][j].visibility) + "km");
      debugLog("Probability of precipitation: " + String(weatherDataDays[i][j].pop) + "%");
      String sunrise = strTime(weatherDataDays[i][j].sunrise);
      sunrise.remove(sunrise.length() - 1);
      debugLog("Sunrise: " + sunrise);
      String sunset = strTime(weatherDataDays[i][j].sunset);
      sunset.remove(sunset.length() - 1);
      debugLog("Sunset: " + sunset);
    }
  }
  /*
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
  */
}
#endif

// https://openweathermap.org/weather-conditions
String weatherConditionIdToStr(int weatherCode)
{
  switch (weatherCode)
  {
  // Group 2xx: Thunderstorm
  case 200:
    return "Thunderstorm with light rain";
  case 201:
    return "Thunderstorm with rain";
  case 202:
    return "Thunderstorm with heavy rain";
  case 210:
    return "Light thunderstorm";
  case 211:
    return "Thunderstorm";
  case 212:
    return "Heavy thunderstorm";
  case 221:
    return "Ragged thunderstorm";
  case 230:
    return "Thunderstorm with light drizzle";
  case 231:
    return "Thunderstorm with drizzle";
  case 232:
    return "Thunderstorm with heavy drizzle";

  // Group 3xx: Drizzle
  case 300:
    return "Light intensity drizzle";
  case 301:
    return "Drizzle";
  case 302:
    return "Heavy intensity drizzle";
  case 310:
    return "Light intensity drizzle rain";
  case 311:
    return "Drizzle rain";
  case 312:
    return "Heavy intensity drizzle rain";
  case 313:
    return "Shower rain and drizzle";
  case 314:
    return "Heavy shower rain and drizzle";
  case 321:
    return "Shower drizzle";

  // Group 5xx: Rain
  case 500:
    return "Light rain";
  case 501:
    return "Moderate rain";
  case 502:
    return "Heavy intensity rain";
  case 503:
    return "Very heavy rain";
  case 504:
    return "Extreme rain";
  case 511:
    return "Freezing rain";
  case 520:
    return "Light intensity shower rain";
  case 521:
    return "Shower rain";
  case 522:
    return "Heavy intensity shower rain";
  case 531:
    return "Ragged shower rain";

  // Group 6xx: Snow
  case 600:
    return "Light snow";
  case 601:
    return "Snow";
  case 602:
    return "Heavy snow";
  case 611:
    return "Sleet";
  case 612:
    return "Light shower sleet";
  case 613:
    return "Shower sleet";
  case 615:
    return "Light rain and snow";
  case 616:
    return "Rain and snow";
  case 620:
    return "Light shower snow";
  case 621:
    return "Shower snow";
  case 622:
    return "Heavy shower snow";

  // Group 7xx: Atmosphere
  case 701:
    return "Mist";
  case 711:
    return "Smoke";
  case 721:
    return "Haze";
  case 731:
    return "Sand/dust whirls";
  case 741:
    return "Fog";
  case 751:
    return "Sand";
  case 761:
    return "Dust";
  case 762:
    return "Volcanic ash";
  case 771:
    return "Squalls";
  case 781:
    return "Tornado";

  // Group 800: Clear
  case 800:
    return "Clear sky";

  // Group 80x: Clouds
  case 801:
    return "Few clouds: 11-25%";
  case 802:
    return "Scattered clouds: 25-50%";
  case 803:
    return "Broken clouds: 51-84%";
  case 804:
    return "Overcast clouds: 85-100%";

  // Default case
  default:
    return "Unknown weather code";
  }
}
