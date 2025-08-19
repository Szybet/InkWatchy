#include "inkField.h"
#include "inkField_localization.h"
#include <MoonPhase.h>

#define TEMP_CORD 48, 120
#define HUMIDITY_CORD 25, 124
#define PRESSURE_CORD 25, 132
#define PREPICITATION_CORD 25, 140
#define VISIBILITY_CORD 25, 148
#define WIND_SPEED_CORD 25, 156

#define WIND_GUTS_CORD 46, 132
#define CLOUD_COVER_CORD 46, 140
#define SUNRISE_CORD 46, 148
#define SUNSET_CORD 46, 156

#define WEATHER_ICON_CORD 63, 124

#define TEMP_RECT_CORD 49, 114
#define TEMP_RECT_SIZE 19, 7

void weatherDrawHelper(uint16_t value, int max, int min, uint16_t x, uint16_t y)
{
    int percent = (value - min) * 100 / (max - min);

    if (percent < 0)
    {
        percent = 0;
    }
    else if (percent > 99)
    {
        percent = 99;
    }

    writeTextReplaceBack(addZero(String(percent), 2), x, y, SCBlack, SCWhite, true, 1);
}

// Use the same weather icon mapping as Taychron for consistency
String getWeatherIcon(uint8_t weatherCode)
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

void inkDrawWeather()
{
    debugLog("Drawing weather in watchface");
    setTextSize(1);
    setFont(getFont("dogicapixel4"));

#if WEATHER_INFO
    OM_OneHourWeather wData = weatherGetDataHourly(WEATHER_WATCHFACE_HOUR_OFFSET);
    if (wData.fine == true)
    {
        // Use global temperature system with proper conversion
        dis->fillRect(TEMP_RECT_CORD, TEMP_RECT_SIZE, SCWhite);
        String temps = formatTemperature(wData.temp);  // Use global function
        if(temps.length() > 3) {
            temps = temps.substring(0, 3);
        }
        writeTextReplaceBack(temps, TEMP_CORD, SCBlack, SCWhite, true, 1);
        // Humidity, already %
        weatherDrawHelper(wData.humidity, 99, 0, HUMIDITY_CORD);
        // Pressure, it's in hPa
        weatherDrawHelper(wData.pressure, PRESSURE_HPA_MAX, PRESSURE_HPA_MIN, PRESSURE_CORD);
        // Prepecitation propability, in %
        weatherDrawHelper(wData.precipitation, 99, 0, PREPICITATION_CORD);
        // Visibility, in meters
        weatherDrawHelper(wData.visibility, VISIBILITY_M_MAX, VISIBILITY_M_MIN, VISIBILITY_CORD);
        // Wind speed, km/h
        weatherDrawHelper(wData.wind_speed, WIND_SPEED_MAX, WIND_SPEED_MIN, WIND_SPEED_CORD);
        // Wind guts, km/h
        weatherDrawHelper(wData.wind_gust, WIND_SPEED_MAX, WIND_SPEED_MIN, WIND_GUTS_CORD);
        // Cloud cover, in %
        weatherDrawHelper(wData.cloud_cover, 99, 0, CLOUD_COVER_CORD);
        debugLog("Sunrise: " + String(wData.sunrise));
        // Sunrise
        writeTextReplaceBack(addZero(String(hour(wData.sunrise)), 2), SUNRISE_CORD);
        // Sunset
        writeTextReplaceBack(addZero(String(hour(wData.sunset)), 2), SUNSET_CORD);

        writeImageN(WEATHER_ICON_CORD, getImg("inkfield/" + getWeatherIcon(wData.weather_code)));
    }
    else
    {
        // Use global language system for error messages
        writeTextReplaceBack(getInkFieldLocalizedError(), TEMP_CORD, SCBlack, SCWhite, true, 1);
        writeTextReplaceBack(getInkFieldLocalizedErrorShort(), HUMIDITY_CORD, SCBlack, SCWhite, true, 1);
        writeTextReplaceBack(getInkFieldLocalizedErrorShort(), PRESSURE_CORD, SCBlack, SCWhite, true, 1);
        writeTextReplaceBack(getInkFieldLocalizedErrorShort(), PREPICITATION_CORD, SCBlack, SCWhite, true, 1);
        writeTextReplaceBack(getInkFieldLocalizedErrorShort(), VISIBILITY_CORD, SCBlack, SCWhite, true, 1);
        writeTextReplaceBack(getInkFieldLocalizedErrorShort(), WIND_SPEED_CORD, SCBlack, SCWhite, true, 1);
        writeTextReplaceBack(getInkFieldLocalizedErrorShort(), WIND_GUTS_CORD, SCBlack, SCWhite, true, 1);
        writeTextReplaceBack(getInkFieldLocalizedErrorShort(), CLOUD_COVER_CORD, SCBlack, SCWhite, true, 1);
        writeTextReplaceBack(getInkFieldLocalizedErrorShort(), SUNRISE_CORD, SCBlack, SCWhite, true, 1);
        writeTextReplaceBack(getInkFieldLocalizedErrorShort(), SUNSET_CORD, SCBlack, SCWhite, true, 1);
        writeImageN(WEATHER_ICON_CORD, getImg("inkfield/" + getWeatherIcon(255)));
    }
#else
    // Use global language system for error messages
    writeTextReplaceBack(getInkFieldLocalizedError(), TEMP_CORD, SCBlack, SCWhite, true, 1);
    writeTextReplaceBack(getInkFieldLocalizedErrorShort(), HUMIDITY_CORD, SCBlack, SCWhite, true, 1);
    writeTextReplaceBack(getInkFieldLocalizedErrorShort(), PRESSURE_CORD, SCBlack, SCWhite, true, 1);
    writeTextReplaceBack(getInkFieldLocalizedErrorShort(), PREPICITATION_CORD, SCBlack, SCWhite, true, 1);
    writeTextReplaceBack(getInkFieldLocalizedErrorShort(), VISIBILITY_CORD, SCBlack, SCWhite, true, 1);
    writeTextReplaceBack(getInkFieldLocalizedErrorShort(), WIND_SPEED_CORD, SCBlack, SCWhite, true, 1);
    writeTextReplaceBack(getInkFieldLocalizedErrorShort(), WIND_GUTS_CORD, SCBlack, SCWhite, true, 1);
    writeTextReplaceBack(getInkFieldLocalizedErrorShort(), CLOUD_COVER_CORD, SCBlack, SCWhite, true, 1);
    writeTextReplaceBack(getInkFieldLocalizedErrorShort(), SUNRISE_CORD, SCBlack, SCWhite, true, 1);
    writeTextReplaceBack(getInkFieldLocalizedErrorShort(), SUNSET_CORD, SCBlack, SCWhite, true, 1);
    writeImageN(WEATHER_ICON_CORD, getImg("inkfield/meteor"));
#endif
}

// Moon stuff
#define MOON_AGE_CORD 136, 133
#define MOON_LIGHT_CORD 125, 145
#define MOON_DISTANCE_CORD 125, 157
#define MOON_IMG_CORD 160, 124

String removeSpaces(String input)
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

void inkDrawMoon()
{
    MoonPhase mp;
    mp.calculate(getUnixTime(timeRTCLocal));
    /*
    double mp.age;		// Age in days of the current cycle.
    double mp.fraction;	// The illumination fraction, from 0% - 100%.
    double mp.distance;	// Moon distance in earth radii.
    const char * mp.phaseName;	// The name of the moon phase: New, Full, etc. - https://github.com/signetica/MoonPhase/blob/d6944c576b418a897ad5cf158041389a7c668a04/MoonPhase.cpp#L20-L22

    So:
    age: Days, 1-31 I ques
    illumination fraction, Percents
    distance, radius, defined by MOON_MIN_RADIUS and MOON_MAX_RADIUS
    */
    setTextSize(1);
    setFont(getFont("inkfield/Speculum9"));
    writeTextReplaceBack(String(int(mp.age)), MOON_AGE_CORD, SCBlack, SCWhite);
    debugLog("mp.fraction: " + String(mp.fraction));
    writeTextReplaceBack(String(int(mp.fraction * 100.0)), MOON_LIGHT_CORD, SCBlack, SCWhite);
    debugLog("mp.distance: " + String(mp.distance));
    int distance = int((mp.distance - MOON_MIN_RADIUS) * 100 / (MOON_MAX_RADIUS - MOON_MIN_RADIUS));
    checkMaxMin(&distance, 100, 0, false);
    writeTextReplaceBack(String(int(distance)), MOON_DISTANCE_CORD, SCBlack, SCWhite);

    writeImageN(MOON_IMG_CORD, getImg("inkfield/" + removeSpaces(String(mp.phaseName))));
}