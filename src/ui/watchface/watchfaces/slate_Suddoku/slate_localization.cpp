#include "slate_localization.h"
#include "rtcMem.h"

// Returns localized weather condition string based on WMO weather codes
// Polish characters have been removed to avoid display/encoding issues
String getLocalizedWeatherCondition(uint8_t weatherCode) {
    // Check language setting from config.h
    if (SLATE_LANG == PL) {
        // Polish weather conditions (without diacritical marks)
        switch(weatherCode) {
            case 0: return "Bezchmurnie";          // Clear sky
            case 1: return "Prawie czysto";        // Mostly clear
            case 2: return "Lekkie chmury";        // Partly cloudy
            case 3: return "Pochmurno";            // Overcast
            case 45: return "Mgla";                // Fog
            case 48: return "Gesta mgla";          // Dense fog
            case 51: return "Lekka mzawka";        // Light drizzle
            case 53: return "Mzawka";              // Moderate drizzle
            case 55: return "Mzawka";              // Dense drizzle
            case 56: return "Lekkie zamrz.";       // Light freezing drizzle
            case 57: return "Lodowato";            // Dense freezing drizzle
            case 61: return "Lekki deszcz";        // Slight rain
            case 63: return "Deszcz";              // Moderate rain
            case 65: return "Ulewny deszcz";       // Heavy rain
            case 66: return "Marznacy";            // Light freezing rain
            case 67: return "Zamiez";              // Heavy freezing rain
            case 71: return "Lekki snieg";         // Slight snow fall
            case 73: return "Snieg";               // Moderate snow fall
            case 75: return "Snieznica";           // Heavy snow fall
            case 77: return "Ziarna";              // Snow grains
            case 80: return "Przelotnie";          // Slight rain showers
            case 81: return "Deszczowo";           // Moderate rain showers
            case 82: return "Opady";               // Violent rain showers
            case 85: return "Sniezne";             // Slight snow showers
            case 86: return "Silnie sniezny";      // Heavy snow showers
            case 95: return "Burza";               // Thunderstorm
            case 96: return "Lekki grad";          // Thunderstorm with slight hail
            case 99: return "Gradowo";             // Thunderstorm with heavy hail
            default: return "Nieznane";            // Unknown weather code
        }
    } else {
        // English weather conditions (default for all non-Polish languages)
        switch(weatherCode) {
            case 0: return "Clear Sky";
            case 1: return "Mostly Clear";
            case 2: return "Partly Cloudy";
            case 3: return "Overcast";
            case 45: return "Foggy";
            case 48: return "Heavy Fog";
            case 51: return "Light Drizzle";
            case 53: return "Drizzling";
            case 55: return "Heavy Drizzle";
            case 56: return "Light Freezing";
            case 57: return "Heavy Freezing";
            case 61: return "Light Rain";
            case 63: return "Raining";
            case 65: return "Heavy Rain";
            case 66: return "Freezing Rain";
            case 67: return "Freezing Hard";
            case 71: return "Light Snow";
            case 73: return "Snowing";
            case 75: return "Heavy Snow";
            case 77: return "Snow Grains";
            case 80: return "Light Showers";
            case 81: return "Showering";
            case 82: return "Heavy Showers";
            case 85: return "Snow Showers";
            case 86: return "Heavy Snow";
            case 95: return "Thunderstorm";
            case 96: return "Light Hail";
            case 99: return "Heavy Hail";
            default: return "Unknown";
        }
    }
}

// Returns localized abbreviated day name (3 letters)
// Polish characters removed to avoid encoding issues on e-ink display
String getLocalizedDayName(int offset) {
    // Get current time in Unix format for day calculation
    long unixTime = rM.SRTC.doMakeTime(timeRTCLocal);
    int weekDay = weekday(unixTime) + offset;
    
    // Convert weekday() result (1=Sunday, 2=Monday...) to array index
    // Arrays start with Monday (index 0), Sunday is at the end (index 6)
    // weekday(): 1=Sun, 2=Mon, 3=Tue, 4=Wed, 5=Thu, 6=Fri, 7=Sat
    int arrayIndex = (weekDay == 1) ? 6 : weekDay - 2;
    
    // Handle wraparound for offsets
    arrayIndex = (arrayIndex + 7) % 7;
    
    if (SLATE_LANG == PL) {
        // Polish day abbreviations starting from Monday (without diacritical marks)
        const char* polishDays[] = {"PON", "WTO", "SRO", "CZW", "PIA", "SOB", "NIE"};
        return String(polishDays[arrayIndex]);
    } else {
        // English day abbreviations starting from Monday  
        const char* englishDays[] = {"MON", "TUE", "WED", "THU", "FRI", "SAT", "SUN"};
        return String(englishDays[arrayIndex]);
    }
}

// Formats time string according to 12H/24H setting from config.h
// Returns time in HH:MM format with proper zero padding
String getLocalizedTimeString(tmElements_t timeEl) {
#if WATCHFACE_12H
    // Convert 24-hour to 12-hour format
    int hour12 = timeEl.Hour;
    
    // Handle special cases for 12-hour format
    if (hour12 == 0) {
        hour12 = 12;        // Midnight (00:xx) becomes 12:xx AM
    } else if (hour12 > 12) {
        hour12 -= 12;       // Afternoon hours (13:xx-23:xx) become 1:xx-11:xx PM
    }
    
    // Format hour with leading zero if single digit
    String hourStr = String(hour12);
    if (hourStr.length() == 1) {
        hourStr = "0" + hourStr;
    }
    
    // Format minute with leading zero if single digit
    String minute = String(timeEl.Minute);
    if (minute.length() == 1) {
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
String getLocalizedAMPM(tmElements_t timeEl) {
#if WATCHFACE_12H
    // Determine if it's afternoon/evening (PM) or morning (AM)
    bool isPM = (timeEl.Hour >= 12);
    
    // AM/PM is standard across all languages
    return isPM ? "PM" : "AM";
#else
    // Return empty string for 24-hour format
    return "";
#endif
}