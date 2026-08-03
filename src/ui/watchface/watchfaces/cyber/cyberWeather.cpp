/**
 * Based off of InkWeather
 */

#include "cyber.h"
#include "cyberHelpers.h"
#include <MoonPhase.h>

#define TEMP_CORD 36, 126
#define WEATHER_ICON_CORD 3, 94

#define MOON_IMG_CORD 170, 91

int cyberPctConvert(uint16_t value)
{
    int percent = (value * 100) / 99;

    if (percent < 0)
    {
        percent = 0;
    }
    else if (percent > 99)
    {
        percent = 99;
    }

    return percent;
}

// Use the same weather icon mapping as Taychron for consistency
String cgetWeatherIcon(uint8_t weatherCode)
{
    switch (weatherCode)
    {
    case 0:
        return "clear_sky";
    case 1:
        return "mainly_clear";
    case 2:
        return "partly_cloud";
    case 3:
        return "overcast";
    case 45:
        return "fog";
    case 48:
        return "freezing";
    case 51:
        return "drizzle_all";
    case 53:
        return "drizzle_all";
    case 55:
        return "drizzle_all";
    case 56:
        return "freezing";
    case 57:
        return "freezing";
    case 61:
        return "rain";
    case 63:
        return "rain";
    case 65:
        return "rain";
    case 66:
        return "freezing";
    case 67:
        return "freezing";
    case 71:
        return "snow";
    case 73:
        return "snow";
    case 75:
        return "snow";
    case 77:
        return "snow";
    case 80:
        return "rain";
    case 81:
        return "rain";
    case 82:
        return "rain";
    case 85:
        return "snow";
    case 86:
        return "snow";
    case 95:
        return "thunderstorm";
    case 96:
        return "thunderstorm";
    case 99:
        return "thunderstorm";
    default:
        return "no_weather_data";
    }
}

String cGetWeatherString(uint8_t weatherCode)
{

    switch (weatherCode)
    {
    case 0:
        return WF_S_WEATHER_CLEAR_SKY;
    case 1:
        return WF_S_WEATHER_MOSTLY_CLEAR;
    case 2:
        return WF_S_WEATHER_PARTLY_CLOUDY;
    case 3:
        return WF_S_WEATHER_OVERCAST;
    case 45:
        return WF_S_WEATHER_FOG;
    case 48:
        return "Ice Fog";
    case 51:
        return WF_S_WEATHER_LIGHT_DRIZZLE;
    case 53:
        return WF_S_WEATHER_DRIZZLE;
    case 55:
        return WF_S_WEATHER_HEAVY_DRIZZLE;
    case 56:
        return "Light Ice Drizzle";
    case 57:
        return "Dense Ice Drizzle";
    case 61:
        return WF_S_WEATHER_LIGHT_RAIN;
    case 63:
        return WF_S_WEATHER_RAIN;
    case 65:
        return WF_S_WEATHER_HEAVY_RAIN;
    case 66:
        return WF_S_WEATHER_LIGHT_FREEZING;
    case 67:
        return WF_S_WEATHER_FREEZING_HARD;
    case 71:
        return WF_S_WEATHER_LIGHT_SNOW;
    case 73:
        return WF_S_WEATHER_SNOW;
    case 75:
        return WF_S_WEATHER_HEAVY_SNOW;
    case 77:
        return WF_S_WEATHER_SNOW_GRAINS;
    case 80:
        return WF_S_WEATHER_LIGHT_SHOWERS;
    case 81:
        return WF_S_WEATHER_SHOWERS;
    case 82:
        return WF_S_WEATHER_HEAVY_SHOWERS;
    case 85:
        return WF_S_WEATHER_SNOW_SHOWERS;
    case 86:
        return "Heavy Snow Shower";
    case 95:
        return WF_S_WEATHER_THUNDERSTORM;
    case 96:
        return WF_S_WEATHER_THUNDERSTORM;
    case 99:
        return WF_S_WEATHER_THUNDERSTORM;
    default:
        return "No Data";
    }
}

OM_HourlyForecastReturn cyberGeneralWeatherGetData()
{
    String unixTimeWeather = String(simplifyUnix(getUnixTime(timeRTCLocal)));
    bufSize weatherData = fsGetBlob(unixTimeWeather, String(WEATHER_HOURLY_DIR) + "/");
    debugLog("Weather size is: " + String(weatherData.size) + " While is should be: " + String(sizeof(OM_HourlyForecast)));
    OM_HourlyForecastReturn forecast = {};
    if (weatherData.size != sizeof(OM_HourlyForecast))
    {
        debugLog("Weather data is bad.");
        free(weatherData.buf);
        // overwriteSwitch(textDialog);
        // showTextDialog(WEATHER_CORRUPTED, true);
        forecast.fine = false;
        return forecast;
    }
    memcpy(&forecast.data, weatherData.buf, weatherData.size);
    free(weatherData.buf);

    forecast.fine = true;
    return forecast;
}

void cyberDrawWeather()
{
#if WEATHER_INFO

    // clear
    dis->fillRect(1, 92, 90, 60, SCWhite);

    // set default strings
    String highLow = "-- / --";
    String temp = "--";
    String weatherIcon = "no-weather";
    String weatherString = "No Data";
    String weatherString2 = "";
    int humidity = -1;
    int precipitation = -1;
    String humidityString = "--";
    String precipitationString = "--";
    float sunrise = 0.0f;
    float sunset = 0.0f;

    // get today's high/low temps
    OM_HourlyForecastReturn forecast = cyberGeneralWeatherGetData();
    if (forecast.fine == true)
    {
        float highNum = getDisplayTemperature(findHighest(forecast.data.temp, 24));
        float lowNum = getDisplayTemperature(findLowest(forecast.data.temp, 27));
        highLow = String(int(round(lowNum))) + " / " + String(int(round(highNum)));
    }

    // get today's weather
    OM_OneHourWeather wData = weatherGetDataHourly(WEATHER_WATCHFACE_HOUR_OFFSET);
    if (wData.fine == true)
    {

        debugLog("Drawing weather in watchface");

        temp = String(getDisplayTemperature(wData.temp)); // formatTemperature(wData.temp);  // Use global function
        if (temp.length() > 2)
        {
            temp = temp.substring(0, 2);
        }

        weatherString = cGetWeatherString(wData.weather_code);
        weatherString2 = "";
        if (weatherString.length() > 14)
        {
            int idx = weatherString.lastIndexOf(" ");
            weatherString2 = weatherString.substring(idx + 1, weatherString.length() - 1);
            weatherString = weatherString.substring(0, idx);
        }
        weatherIcon = cgetWeatherIcon(wData.weather_code);

        humidity = wData.humidity;
        precipitation = wData.precipitation;

        sunrise = hour(wData.sunrise) + (minute(wData.sunrise) / 60.0f);
        sunset = hour(wData.sunset) + (minute(wData.sunset) / 60.0f);
    }
    else
    {
        // Use global language system for error messages
        weatherString = WF_I_ERROR;
        weatherIcon = "error";
    }
    // write the data
    setTextSize(1);
    dis->setTextColor(SCBlack);
    setFont(getFont("cyber/RajdhaniL14"));
    int degPos = cyberRightTextXOffset(temp);
    dis->setCursor(41, 113);
    dis->print(temp);
    dis->drawRect(41 + degPos + 4, 97, 3, 5, SCBlack);

    setFont(getFont("cyber/RajdhaniM6"));
    dis->setCursor(41, 127);
    dis->print(highLow);

    dis->setCursor(5, 140);
    dis->print(weatherString);
    dis->setCursor(5, 150);
    dis->print(weatherString2);
    writeImageN(5, 96, getImg("cyber/" + weatherIcon));

    // weather arc graphs
    writeImageN(93, 95, getImg("cyber/weatherringblock"));
    // humidity
    if (humidity != -1.0f)
    {
        humidityString = String(humidity);
    }
    if (precipitation != -1.0f)
    {
        precipitationString = String(precipitation);
    }
    dis->setCursor(106 - cyberCenterTextXOffset(humidityString), 139);
    dis->print(humidityString);
    cyberDrawPctGraph(humidity / 100.0f, 107, 137);

    // precipitation
    dis->setCursor(140 - cyberCenterTextXOffset(precipitationString), 139);
    dis->print(precipitationString);
    cyberDrawPctGraph(wData.precipitation / 100.0f, 141, 137);
    // SUNRISE/SUNSET graph
    float hPct = ((timeRTCLocal.Hour + timeRTCLocal.Minute / 60.0f) / 24.0f);
    String hPctConv = String(hPct * 100.0f);
    if (hPctConv.length() > 2)
    {
        hPctConv = hPctConv.substring(0, 2);
    }
    dis->setCursor(124 - cyberCenterTextXOffset(hPctConv), 112);
    dis->print(hPctConv);

    if (sunrise > 0 && sunset > 0)
    {
        float sunriseDeg = (((sunrise - 0) * 270.0f) / 24.0f);
        float sunsetDeg = (((sunset - 0) * 270.0f) / 24.0f);
        cyberDrawArc(sunriseDeg - 225, sunsetDeg - sunriseDeg, 124, 109, 10);
    }
    cyberDrawPctGraph(hPct, 124, 109);

#else
    // no errors b/c no one turned on weather
#endif
}

// Moon stuff

String cyberRemoveSpaces(String input)
{
    String output = "";
    for (int i = 0; i < input.length(); i++)
    {
        char character = input.charAt(i);
        if (character != ' ')
        {
            output = output + String(character);
        }
    }
    return output;
}

void cyberDrawMoon()
{
    // From InkWeather
    MoonPhase mp;
    mp.calculate(getUnixTime(timeRTCLocal));
    debugLog(String(mp.phaseName));
    writeImageN(MOON_IMG_CORD, getImg("cyber/" + cyberRemoveSpaces(String(mp.phaseName))));
}
