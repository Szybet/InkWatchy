#include "domain.h"
#include "domain_localization.h"
#include <MoonPhase.h>

#define TEMP_CORD 48, 120
#define TEMP_RECT_CORD 49, 114
#define TEMP_RECT_SIZE 19, 7

#define HUMIDITY_CORD 25, 124
#define PRESSURE_CORD 25, 132
#define PREPICITATION_CORD 25, 140
#define VISIBILITY_CORD 25, 148
#define WIND_SPEED_CORD 25, 156

#define WIND_GUST_CORD 46, 132
#define CLOUD_COVER_CORD 46, 140
#define SUNRISE_CORD 46, 148
#define SUNSET_CORD 46, 156

#define WEATHER_ICON_CORD 63, 124
#define DAYNIGHT_ICON_CORD 71, 113

#define WETBULB_CORD 121, 133
#define CAPE_CORD 121, 145
#define SUNLIGHT_MINUTES_CORD 121, 157

#define COMPASS_IMG_CORD 66, 73
#define MOON_IMG_CORD 160, 124

#define AQI_EU_CORD 103, 71
#define AQI_US_CORD 103, 79
#define AQI_EU_PM2_5_CORD 103, 87
#define AQI_EU_PM10_CORD 103, 95
#define AQI_EU_NO2_CORD 103, 103
#define AQI_EU_OZONE_CORD 103, 111
#define AQI_EU_SO2_CORD 103, 119

int clampValue(int value, int min, int max)
{
    if (value < min)
        return min;
    if (value > max)
        return max;
    return value;
}

void domainWeatherDrawHelper(uint16_t value, int max, int min, uint16_t x, uint16_t y)
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
String getDomainWeatherIcon(uint8_t weatherCode)
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
        return "ice_fog";
    case 51:
        return "drizzle_light";
    case 53:
        return "drizzle_mid";
    case 55:
        return "drizzle_dense";
    case 56:
        return "ice_drizzle_light";
    case 57:
        return "ice_drizzle_dense";
    case 61:
        return "rain_light";
    case 63:
        return "rain_mid";
    case 65:
        return "rain_heavy";
    case 66:
        return "ice_rain_light";
    case 67:
        return "ice_rain_heavy";
    case 71:
        return "snow_light";
    case 73:
        return "snow_mid";
    case 75:
        return "snow_heavy";
    case 77:
        return "snow_grain";
    case 80:
        return "rain_shower_light";
    case 81:
        return "rain_shower_mid";
    case 82:
        return "rain_shower_violent";
    case 85:
        return "snow_shower_light";
    case 86:
        return "snow_shower_heavy";
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

String getDayNightIcon(uint8_t is_day)
{
    switch (is_day)
    {
    case 0:
        return "nighttime";
    case 1:
        return "daytime";
    default:
        return "huhtime"; // Default for unexpected values
    }
}

void domainDrawWeather()
{
    debugLog("Drawing weather in watchface");
    setTextSize(1);
    setFont(getFont("domain/dogicapixel_dotp4"));

#if WEATHER_INFO
    OM_OneHourWeather wData = weatherGetDataHourly(WEATHER_WATCHFACE_HOUR_OFFSET);
    if (wData.fine == true)
    {
        // Use global apparent temperature system with proper conversion
        dis->fillRect(TEMP_RECT_CORD, TEMP_RECT_SIZE, SCWhite);
        String temps = formatTemperature(wData.apparent_temp); // Use global function
        if (temps.length() > 3)
        {
            temps = temps.substring(0, 3);
        }
        writeTextReplaceBack(temps, TEMP_CORD, SCBlack, SCWhite, true, 1);
        // Humidity, already %
        domainWeatherDrawHelper(wData.humidity, 99, 0, HUMIDITY_CORD);
        // Pressure, it's in hPa
        domainWeatherDrawHelper(wData.pressure, PRESSURE_HPA_MAX, PRESSURE_HPA_MIN, PRESSURE_CORD);
        // Prepecitation propability, in %
        domainWeatherDrawHelper(wData.precipitation, 99, 0, PREPICITATION_CORD);
        // Visibility, in meters
        domainWeatherDrawHelper(wData.visibility, VISIBILITY_M_MAX, VISIBILITY_M_MIN, VISIBILITY_CORD);
        // Wind speed, km/h
        domainWeatherDrawHelper(wData.wind_speed, WIND_SPEED_MAX, WIND_SPEED_MIN, WIND_SPEED_CORD);
        // Wind guts, km/h
        domainWeatherDrawHelper(wData.wind_gust, WIND_SPEED_MAX, WIND_SPEED_MIN, WIND_GUST_CORD);
        // Cloud cover, in %
        domainWeatherDrawHelper(wData.cloud_cover, 99, 0, CLOUD_COVER_CORD);
        debugLog("Sunrise: " + String(wData.sunrise));
        // Alright so, this is annoying. all your weather data could be correct,
        // but you may find that your sunrise/sunset is off (as if you're at GMT 0)
        // in config.h, modify SUNRISE_SUNSET_HOUR_OFFSET to fix this

        // Sunrise
        writeTextReplaceBack(addZero(String((hour(wData.sunrise) + (SUNRISE_SUNSET_HOUR_OFFSET) + 24) % 24), 2), SUNRISE_CORD);
        // Sunset
        writeTextReplaceBack(addZero(String((hour(wData.sunset) + (SUNRISE_SUNSET_HOUR_OFFSET) + 24) % 24), 2), SUNSET_CORD);

        writeImageN(WEATHER_ICON_CORD, getImg("domain/" + getDomainWeatherIcon(wData.weather_code)));
        writeImageN(DAYNIGHT_ICON_CORD, getImg("domain/" + getDayNightIcon(wData.is_day)));

        // additional weather metrics
        setFont(getFont("domain/Speculum9"));

        // Wet Bulb Temperature (celsius)
        int wetbulb = round(wData.wet_bulb);
        wetbulb = (wetbulb <= -99) ? -99 : wetbulb;
        writeTextReplaceBack(addZero(String(wetbulb), 3), WETBULB_CORD, SCBlack, SCWhite);

        // CAPE (Convective Available Potential Energy)
        int cape = round(wData.cape / 10.0);
        cape = (cape >= 1000) ? 999 : cape;
        writeTextReplaceBack(addZero(String(cape), 3), CAPE_CORD, SCBlack, SCWhite);

        // Sunshine Duration (minutes)
        int sunshine = round(wData.sunshine / 60.0);
        sunshine = (sunshine >= 1000) ? 999 : sunshine;
        writeTextReplaceBack(addZero(String(sunshine), 3), SUNLIGHT_MINUTES_CORD, SCBlack, SCWhite);
    }
    else
    {
        // Use global language system for error messages
        writeTextReplaceBack(getDomainLocalizedError(), TEMP_CORD, SCBlack, SCWhite, true, 1);
        writeTextReplaceBack(getDomainLocalizedErrorShort(), HUMIDITY_CORD, SCBlack, SCWhite, true, 1);
        writeTextReplaceBack(getDomainLocalizedErrorShort(), PRESSURE_CORD, SCBlack, SCWhite, true, 1);
        writeTextReplaceBack(getDomainLocalizedErrorShort(), PREPICITATION_CORD, SCBlack, SCWhite, true, 1);
        writeTextReplaceBack(getDomainLocalizedErrorShort(), VISIBILITY_CORD, SCBlack, SCWhite, true, 1);
        writeTextReplaceBack(getDomainLocalizedErrorShort(), WIND_SPEED_CORD, SCBlack, SCWhite, true, 1);
        writeTextReplaceBack(getDomainLocalizedErrorShort(), WIND_GUST_CORD, SCBlack, SCWhite, true, 1);
        writeTextReplaceBack(getDomainLocalizedErrorShort(), CLOUD_COVER_CORD, SCBlack, SCWhite, true, 1);
        writeTextReplaceBack(getDomainLocalizedErrorShort(), SUNRISE_CORD, SCBlack, SCWhite, true, 1);
        writeTextReplaceBack(getDomainLocalizedErrorShort(), SUNSET_CORD, SCBlack, SCWhite, true, 1);
        writeTextReplaceBack(getDomainLocalizedError(), WETBULB_CORD, SCBlack, SCWhite, true, 1);
        writeTextReplaceBack(getDomainLocalizedError(), CAPE_CORD, SCBlack, SCWhite, true, 1);
        writeTextReplaceBack(getDomainLocalizedError(), SUNLIGHT_MINUTES_CORD, SCBlack, SCWhite, true, 1);
        writeImageN(WEATHER_ICON_CORD, getImg("domain/" + getDomainWeatherIcon(255)));
        writeImageN(DAYNIGHT_ICON_CORD, getImg("domain/" + getDayNightIcon(255)));
        writeImageN(COMPASS_IMG_CORD, getImg("domain/wind_error"));
    }
#else
    // Use global language system for error messages
    writeTextReplaceBack(getDomainLocalizedError(), TEMP_CORD, SCBlack, SCWhite, true, 1);
    writeTextReplaceBack(getDomainLocalizedErrorShort(), HUMIDITY_CORD, SCBlack, SCWhite, true, 1);
    writeTextReplaceBack(getDomainLocalizedErrorShort(), PRESSURE_CORD, SCBlack, SCWhite, true, 1);
    writeTextReplaceBack(getDomainLocalizedErrorShort(), PREPICITATION_CORD, SCBlack, SCWhite, true, 1);
    writeTextReplaceBack(getDomainLocalizedErrorShort(), VISIBILITY_CORD, SCBlack, SCWhite, true, 1);
    writeTextReplaceBack(getDomainLocalizedErrorShort(), WIND_SPEED_CORD, SCBlack, SCWhite, true, 1);
    writeTextReplaceBack(getDomainLocalizedErrorShort(), WIND_GUST_CORD, SCBlack, SCWhite, true, 1);
    writeTextReplaceBack(getDomainLocalizedErrorShort(), CLOUD_COVER_CORD, SCBlack, SCWhite, true, 1);
    writeTextReplaceBack(getDomainLocalizedErrorShort(), SUNRISE_CORD, SCBlack, SCWhite, true, 1);
    writeTextReplaceBack(getDomainLocalizedErrorShort(), SUNSET_CORD, SCBlack, SCWhite, true, 1);
    writeTextReplaceBack(getDomainLocalizedErrorLong(), WETBULB_CORD, SCBlack, SCWhite, true, 1);
    writeTextReplaceBack(getDomainLocalizedErrorLong(), CAPE_CORD, SCBlack, SCWhite, true, 1);
    writeTextReplaceBack(getDomainLocalizedErrorLong(), SUNLIGHT_MINUTES_CORD, SCBlack, SCWhite, true, 1);
    writeImageN(WEATHER_ICON_CORD, getImg("domain/meteor"));
    writeImageN(DAYNIGHT_ICON_CORD, getImg("domain/meteortime"));
#endif

    OM_OneHourAirQuality aqData = airQualityGetDataHourly(WEATHER_WATCHFACE_HOUR_OFFSET);
    setFont(getFont("domain/dogicapixel_dotp4"));

    if (aqData.fine == true)
    {
        // Display raw values on screen for debugging
        setFont(getFont("domain/dogicapixel_dotp4"));
        if (aqData.fine == true)
        {
            String euStr = (aqData.european_aqi <= 20) ? "+ " : (aqData.european_aqi <= 40) ? "= "
                                                            : (aqData.european_aqi <= 60)   ? "~ "
                                                            : (aqData.european_aqi <= 80)   ? "- "
                                                            : (aqData.european_aqi <= 100)  ? "x "
                                                            : (aqData.european_aqi <= 125)  ? "* "
                                                                                            : "! ";
            String usStr = (aqData.us_aqi <= 50) ? "+ " : (aqData.us_aqi <= 100) ? "= "
                                                      : (aqData.us_aqi <= 150)   ? "~ "
                                                      : (aqData.us_aqi <= 200)   ? "- "
                                                      : (aqData.us_aqi <= 300)   ? "x "
                                                      : (aqData.us_aqi <= 400)   ? "* "
                                                                                 : "! ";
            String pm25Str = (aqData.european_aqi_pm2_5 <= 10) ? "+ " : (aqData.european_aqi_pm2_5 <= 20) ? "= "
                                                                    : (aqData.european_aqi_pm2_5 <= 25)   ? "~ "
                                                                    : (aqData.european_aqi_pm2_5 <= 50)   ? "- "
                                                                    : (aqData.european_aqi_pm2_5 <= 75)   ? "x "
                                                                    : (aqData.european_aqi_pm2_5 <= 100)  ? "* "
                                                                                                          : "! ";
            String pm10Str = (aqData.european_aqi_pm10 <= 20) ? "+ " : (aqData.european_aqi_pm10 <= 40) ? "= "
                                                                   : (aqData.european_aqi_pm10 <= 50)   ? "~ "
                                                                   : (aqData.european_aqi_pm10 <= 100)  ? "- "
                                                                   : (aqData.european_aqi_pm10 <= 150)  ? "x "
                                                                   : (aqData.european_aqi_pm10 <= 200)  ? "* "
                                                                                                        : "! ";
            String no2Str = (aqData.european_aqi_nitrogen_dioxide <= 40) ? "+ " : (aqData.european_aqi_nitrogen_dioxide <= 90) ? "= "
                                                                              : (aqData.european_aqi_nitrogen_dioxide <= 120)  ? "~ "
                                                                              : (aqData.european_aqi_nitrogen_dioxide <= 230)  ? "- "
                                                                              : (aqData.european_aqi_nitrogen_dioxide <= 340)  ? "x "
                                                                              : (aqData.european_aqi_nitrogen_dioxide <= 500)  ? "* "
                                                                                                                               : "! ";
            String ocy3Str = (aqData.european_aqi_ozone <= 50) ? "+ " : (aqData.european_aqi_ozone <= 100) ? "= "
                                                                    : (aqData.european_aqi_ozone <= 130)   ? "~ "
                                                                    : (aqData.european_aqi_ozone <= 240)   ? "- "
                                                                    : (aqData.european_aqi_ozone <= 380)   ? "x "
                                                                    : (aqData.european_aqi_ozone <= 500)   ? "* "
                                                                                                           : "! ";
            String so2Str = (aqData.european_aqi_sulphur_dioxide <= 100) ? "+ " : (aqData.european_aqi_sulphur_dioxide <= 200) ? "= "
                                                                              : (aqData.european_aqi_sulphur_dioxide <= 350)   ? "~ "
                                                                              : (aqData.european_aqi_sulphur_dioxide <= 500)   ? "- "
                                                                              : (aqData.european_aqi_sulphur_dioxide <= 750)   ? "x "
                                                                              : (aqData.european_aqi_sulphur_dioxide <= 999)   ? "* "
                                                                                                                               : "! ";

            writeTextReplaceBack(euStr, AQI_EU_CORD, SCBlack, SCWhite, true, 1);
            writeTextReplaceBack(usStr, AQI_US_CORD, SCBlack, SCWhite, true, 1);
            writeTextReplaceBack(pm25Str, AQI_EU_PM2_5_CORD, SCBlack, SCWhite, true, 1);
            writeTextReplaceBack(pm10Str, AQI_EU_PM10_CORD, SCBlack, SCWhite, true, 1);
            writeTextReplaceBack(no2Str, AQI_EU_NO2_CORD, SCBlack, SCWhite, true, 1);
            writeTextReplaceBack(ocy3Str, AQI_EU_OZONE_CORD, SCBlack, SCWhite, true, 1);
            writeTextReplaceBack(so2Str, AQI_EU_SO2_CORD, SCBlack, SCWhite, true, 1);
        }
    }
    else
    {
        writeTextReplaceBack("?", AQI_EU_CORD, SCBlack, SCWhite, true, 1);
        writeTextReplaceBack("?", AQI_US_CORD, SCBlack, SCWhite, true, 1);
        writeTextReplaceBack("?", AQI_EU_PM2_5_CORD, SCBlack, SCWhite, true, 1);
        writeTextReplaceBack("?", AQI_EU_PM10_CORD, SCBlack, SCWhite, true, 1);
        writeTextReplaceBack("?", AQI_EU_NO2_CORD, SCBlack, SCWhite, true, 1);
        writeTextReplaceBack("?", AQI_EU_OZONE_CORD, SCBlack, SCWhite, true, 1);
        writeTextReplaceBack("?", AQI_EU_SO2_CORD, SCBlack, SCWhite, true, 1);
    }

    /* if (aqData.fine == true) {
        String euStr = (aqData.european_aqi <= 4) ? "+" :
                       (aqData.european_aqi <= 39) ? "=" : "!";
        String usStr = (aqData.us_aqi <= 4) ? "+" :
                       (aqData.us_aqi <= 99) ? "=" : "!";
        String pm25Str = (aqData.european_aqi_pm2_5 <= 1) ? "+" :
                         (aqData.european_aqi_pm2_5 <= 39) ? "=" : "!";
        String pm10Str = (aqData.european_aqi_pm10 <= 1) ? "+" :
                         (aqData.european_aqi_pm10 <= 39) ? "=" : "!";

        writeTextReplaceBack(euStr, AQI_EU_CORD, SCBlack, SCWhite, true, 1);
        writeTextReplaceBack(usStr, AQI_US_CORD, SCBlack, SCWhite, true, 1);
        writeTextReplaceBack(pm25Str, AQI_EU_PM2_5_CORD, SCBlack, SCWhite, true, 1);
        writeTextReplaceBack(pm10Str, AQI_EU_PM10_CORD, SCBlack, SCWhite, true, 1);
    } else {
        // Only show "?" when there's no data
        writeTextReplaceBack("?", AQI_EU_CORD, SCBlack, SCWhite, true, 1);
        writeTextReplaceBack("?", AQI_US_CORD, SCBlack, SCWhite, true, 1);
        writeTextReplaceBack("?", AQI_EU_PM2_5_CORD, SCBlack, SCWhite, true, 1);
        writeTextReplaceBack("?", AQI_EU_PM10_CORD, SCBlack, SCWhite, true, 1);
    }
    */
}

String domainRemoveSpaces(String input)
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

String getWindDirectionIcon(uint16_t wind_deg)
{
    // Convert wind degree to one of 8 cardinal directions
    if (wind_deg >= 338 || wind_deg < 23)
        return "N";
    else if (wind_deg >= 23 && wind_deg < 68)
        return "NE";
    else if (wind_deg >= 68 && wind_deg < 113)
        return "E";
    else if (wind_deg >= 113 && wind_deg < 158)
        return "SE";
    else if (wind_deg >= 158 && wind_deg < 203)
        return "S";
    else if (wind_deg >= 203 && wind_deg < 248)
        return "SW";
    else if (wind_deg >= 248 && wind_deg < 293)
        return "W";
    else if (wind_deg >= 293 && wind_deg < 338)
        return "NW";
    else
        return "error"; // Default to North if something unexpected happens
}

void domainDrawWindCompass()
{
#if WEATHER_INFO
    OM_OneHourWeather wData = weatherGetDataHourly(WEATHER_WATCHFACE_HOUR_OFFSET);
    if (wData.fine == true)
    {
        // Get the appropriate wind direction icon
        String windDirection = getWindDirectionIcon(wData.wind_deg);

        // Draw the compass image
        writeImageN(COMPASS_IMG_CORD, getImg("domain/wind_" + windDirection));
    }
    else
    {
        // Show a question mark when no data is available
        writeImageN(COMPASS_IMG_CORD, getImg("domain/wind_error"));
    }
#else
    // Show a question mark when weather info is disabled
    writeImageN(COMPASS_IMG_CORD, getImg("domain/wind_error"));
#endif
}

void domainDrawMoon()
{
    // REMOVED: Moon statistics display (kept only moon image)
    MoonPhase mp;
    mp.calculate(getUnixTime(timeRTCLocal));
    writeImageN(MOON_IMG_CORD, getImg("domain/" + domainRemoveSpaces(String(mp.phaseName))));
}