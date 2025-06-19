#include "taychron.h"

#if WATCHFACE_TAYCHRON

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

void tayDrawWeather()
{
    debugLog("Drawing weather in watchface");
    setTextSize(1);
    setFont(getFont("taychron/Mono13"));

#if WEATHER_INFO
    OM_OneHourWeather wData = weatherGetDataHourly(WEATHER_WATCHFACE_HOUR_OFFSET);
    if (wData.fine == true)
    {
        // Use global temperature system with proper conversion
        dis->fillRect(TEMP_RECT_CORD, TEMP_RECT_SIZE, GxEPD_WHITE);
        String temps = formatTemperature(wData.temp);  // Use global function
        if (temps.length() > 3)
        {
            temps = temps.substring(0, 3);
        }
        writeTextReplaceBack(temps, TEMP_CORD, GxEPD_BLACK, GxEPD_WHITE, true, 1);
        writeImageN(WEATHER_ICON_CORD, getImg("taychron/" + tay_getWeatherIcon(wData.weather_code)));
    }
    else
    {
        // Use global language system for error message
        writeTextReplaceBack(WF_T_ERROR, TEMP_CORD, GxEPD_BLACK, GxEPD_WHITE, true, 1);
        writeImageN(WEATHER_ICON_CORD, getImg("taychron/" + tay_getWeatherIcon(255)));
    }
#else
    // Use global language system for error message
    writeTextReplaceBack(WF_T_ERROR, TEMP_CORD, GxEPD_BLACK, GxEPD_WHITE, true, 1);
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

#endif