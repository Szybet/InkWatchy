#include "taychron.h"
#include <MoonPhase.h>

#define WEATHER_ICON_CORD 38, 162

#define TEMP_CORD 72, 185
#define TEMP_RECT_CORD 72, 171
#define TEMP_RECT_SIZE 42, 15

String tay_getWeatherIcon(uint8_t weatherCode)
{
    switch (weatherCode)
    {
    case 0:
        return "clear_sky";
        // return "Clear sky";
    case 1:
        return "mainly_clear";
        // return "Mainly clear";
    case 2:
        return "partly_cloud";
        // return "Partly cloudy";
    case 3:
        return "overcast";
        // return "Overcast";
    case 45:
        return "fog";
        // return "Fog";
    case 48:
        return "freezing";
        // return "Depositing rime fog";
    case 51:
        return "drizzle_all";
        // return "Drizzle: Light intensity";
    case 53:
        return "drizzle_all";
        // return "Drizzle: Moderate intensity";
    case 55:
        return "drizzle_all";
        // return "Drizzle: Dense intensity";
    case 56:
        return "freezing";
        // return "Freezing Drizzle: Light intensity";
    case 57:
        return "freezing";
        // return "Freezing Drizzle: Dense intensity";
    case 61:
        return "rain";
        // return "Rain: Slight intensity";
    case 63:
        return "rain";
        // return "Rain: Moderate intensity";
    case 65:
        return "rain";
        // return "Rain: Heavy intensity";
    case 66:
        return "freezing";
        // return "Freezing Rain: Light intensity";
    case 67:
        return "freezing";
        // return "Freezing Rain: Heavy intensity";
    case 71:
        return "snow";
        // return "Snow fall: Slight intensity";
    case 73:
        return "snow";
        // return "Snow fall: Moderate intensity";
    case 75:
        return "snow";
        // return "Snow fall: Heavy intensity";
    case 77:
        return "snow";
        // return "Snow grains";
    case 80:
        return "rain";
        // return "Rain showers: Slight intensity";
    case 81:
        return "rain";
        // return "Rain showers: Moderate intensity";
    case 82:
        return "rain";
        // return "Rain showers: Violent intensity";
    case 85:
        return "snow";
        // return "Snow showers: Slight intensity";
    case 86:
        return "snow";
        // return "Snow showers: Heavy intensity";
    case 95:
        return "thunderstorm";
        // return "Thunderstorm: Slight or moderate";
    case 96:
        return "thunderstorm";
        // return "Thunderstorm with slight hail";
    case 99:
        return "thunderstorm";
        // return "Thunderstorm with heavy hail";
    default:
        return "no_weather_data";
        // return "Unknown weather condition";
    }
}

void tayDrawWeather()
{
    debugLog("Drawing weather in watchface");
    setTextSize(1);
    setFont(getFont("taychron/Mono13"));

#if WEATHER_INFO
    OM_OneHourWeather wData = weatherGetDataHourly(WEATHER_WATCHFACE_HOUR_OFFSET);
    if (wData.fine == true)
    {
        // Temp
        dis->fillRect(TEMP_RECT_CORD, TEMP_RECT_SIZE, GxEPD_WHITE);
        String temps = String(int16_t(roundf(wData.temp))) + "C";
        if (temps.length() > 3)
        {
            temps = temps.substring(0, 3);
        }
        writeTextReplaceBack(temps, TEMP_CORD, GxEPD_BLACK, GxEPD_WHITE, true, 1);
        writeImageN(WEATHER_ICON_CORD, getImg("taychron/" + tay_getWeatherIcon(wData.weather_code)));
    }
    else
    {
        writeTextReplaceBack("ERR", TEMP_CORD, GxEPD_BLACK, GxEPD_WHITE, true, 1);
        writeImageN(WEATHER_ICON_CORD, getImg("taychron/" + tay_getWeatherIcon(255)));
    }
#else
    writeTextReplaceBack("ERR", TEMP_CORD, GxEPD_BLACK, GxEPD_WHITE, true, 1);
    writeImageN(WEATHER_ICON_CORD, getImg("taychron/meteor"));
#endif
}

#define MOON_IMG_CORD 127, 160

String tayRemoveSpaces(String input)
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

void tayDrawMoon()
{
    MoonPhase mp;
    mp.calculate(getUnixTime(timeRTCLocal));
    writeImageN(MOON_IMG_CORD, getImg("taychron/" + tayRemoveSpaces(String(mp.phaseName))));
}