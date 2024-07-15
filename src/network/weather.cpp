#include "weather.h"

#if WEATHER_INFO
bool isWeatherAvailable = false;

// http://api.open-meteo.com/v1/forecast?latitude=53.543082&longitude=9.994695&hourly=temperature_2m,relative_humidity_2m,apparent_temperature,precipitation_probability,weather_code,pressure_msl,cloud_cover,visibility,wind_speed_10m,wind_direction_10m,wind_gusts_10m,is_day&daily=sunrise,sunset&timeformat=unixtime&timezone=auto
void syncWeather()
{
  if (strlen(WEATHER_LONGTIT) == 0 || strlen(WEATHER_LATIT) == 0)
  {
    return;
  }
  
  OM_HourlyForecast *forecast = new OM_HourlyForecast;
  getHourlyForecast(forecast, String(WEATHER_LATIT).toFloat(), String(WEATHER_LONGTIT).toFloat());

  // Save every hour seperately, via unix time
}

void loadWeatherData()
{

  isWeatherAvailable = true;
}

#if DEBUG
void dumpWeather()
{
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
#endif