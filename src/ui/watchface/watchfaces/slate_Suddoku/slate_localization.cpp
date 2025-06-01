#include "slate_localization.h"
#include "rtcMem.h"

// Returns localized weather condition string based on WMO weather codes
// Now uses compile-time language macros instead of runtime checks
String getLocalizedWeatherCondition(uint8_t weatherCode)
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
        return WF_S_WEATHER_HEAVY_FOG;
    case 51:
        return WF_S_WEATHER_LIGHT_DRIZZLE;
    case 53:
        return WF_S_WEATHER_DRIZZLE;
    case 55:
        return WF_S_WEATHER_HEAVY_DRIZZLE;
    case 56:
        return WF_S_WEATHER_LIGHT_FREEZING;
    case 57:
        return WF_S_WEATHER_HEAVY_FREEZING;
    case 61:
        return WF_S_WEATHER_LIGHT_RAIN;
    case 63:
        return WF_S_WEATHER_RAIN;
    case 65:
        return WF_S_WEATHER_HEAVY_RAIN;
    case 66:
        return WF_S_WEATHER_FREEZING_RAIN;
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
        return WF_S_WEATHER_HEAVY_SNOW;
    case 95:
        return WF_S_WEATHER_THUNDERSTORM;
    case 96:
        return WF_S_WEATHER_LIGHT_HAIL;
    case 99:
        return WF_S_WEATHER_HEAVY_HAIL;
    default:
        return WF_S_WEATHER_UNKNOWN;
    }
}

// Returns localized abbreviated day name (3 letters)
// Now uses compile-time language macros
String getLocalizedDayName(int offset)
{
    // Get current time in Unix format for day calculation
    long unixTime = rM.SRTC.doMakeTime(timeRTCLocal);
    int weekDay = weekday(unixTime) + offset;

    // Convert weekday() result (1=Sunday, 2=Monday...) to array index
    // Arrays start with Monday (index 0), Sunday is at the end (index 6)
    // weekday(): 1=Sun, 2=Mon, 3=Tue, 4=Wed, 5=Thu, 6=Fri, 7=Sat
    int arrayIndex = (weekDay == 1) ? 6 : weekDay - 2;

    // Handle wraparound for offsets
    arrayIndex = (arrayIndex + 7) % 7;

    // Use compile-time language macros
    const char *dayNames[] = LANGUAGE_DAY_NAMES;
    return String(dayNames[arrayIndex]);
}

// Formats time string according to 12H/24H setting from config.h
// Returns time in HH:MM format with proper zero padding
String getLocalizedTimeString(tmElements_t timeEl)
{
#if WATCHFACE_12H
    // Convert 24-hour to 12-hour format
    int hour12 = timeEl.Hour;

    // Handle special cases for 12-hour format
    if (hour12 == 0)
    {
        hour12 = 12; // Midnight (00:xx) becomes 12:xx AM
    }
    else if (hour12 > 12)
    {
        hour12 -= 12; // Afternoon hours (13:xx-23:xx) become 1:xx-11:xx PM
    }

    // Format hour with leading zero if single digit
    String hourStr = String(hour12);
    if (hourStr.length() == 1)
    {
        hourStr = "0" + hourStr;
    }

    // Format minute with leading zero if single digit
    String minute = String(timeEl.Minute);
    if (minute.length() == 1)
    {
        minute = "0" + minute;
    }

    return hourStr + ":" + minute;
#else
    // Use standard 24-hour format (calls existing function)
    return getHourMinute(timeEl);
#endif
}

// Returns AM/PM indicator for 12-hour format
// Only used when WATCHFACE_12H is enabled in config.h
String getLocalizedAMPM(tmElements_t timeEl)
{
#if WATCHFACE_12H
    // Determine if it's afternoon/evening (PM) or morning (AM)
    bool isPM = (timeEl.Hour >= 12);

    // Use compile-time language macros
    return isPM ? WF_TIME_PM : WF_TIME_AM;
#else
    // Return empty string for 24-hour format
    return "";
#endif
}