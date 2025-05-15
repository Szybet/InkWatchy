#pragma once

#include <Arduino.h>  // For String type
#include "config.h"
#include "temperature.h"

// ==============================================================================
// LANGUAGE CONFIGURATION
// ==============================================================================

// Language definitions
#define EN 1
#define PL 2
#define DE 3

// Ensure INKWATCHY_LANG is defined
#ifndef INKWATCHY_LANG
#define INKWATCHY_LANG EN
#endif

// ==============================================================================
// SLATE WATCHFACE TRANSLATIONS
// ==============================================================================

#if INKWATCHY_LANG == EN
    // Weather conditions
    #define WF_S_WEATHER_CLEAR_SKY "Clear Sky"
    #define WF_S_WEATHER_MOSTLY_CLEAR "Mostly Clear"
    #define WF_S_WEATHER_PARTLY_CLOUDY "Partly Cloudy"
    #define WF_S_WEATHER_OVERCAST "Overcast"
    #define WF_S_WEATHER_FOG "Foggy"
    #define WF_S_WEATHER_HEAVY_FOG "Heavy Fog"
    #define WF_S_WEATHER_LIGHT_DRIZZLE "Light Drizzle"
    #define WF_S_WEATHER_DRIZZLE "Drizzling"
    #define WF_S_WEATHER_HEAVY_DRIZZLE "Heavy Drizzle"
    #define WF_S_WEATHER_LIGHT_FREEZING "Light Freezing"
    #define WF_S_WEATHER_HEAVY_FREEZING "Heavy Freezing"
    #define WF_S_WEATHER_LIGHT_RAIN "Light Rain"
    #define WF_S_WEATHER_RAIN "Raining"
    #define WF_S_WEATHER_HEAVY_RAIN "Heavy Rain"
    #define WF_S_WEATHER_FREEZING_RAIN "Freezing Rain"
    #define WF_S_WEATHER_FREEZING_HARD "Freezing Hard"
    #define WF_S_WEATHER_LIGHT_SNOW "Light Snow"
    #define WF_S_WEATHER_SNOW "Snowing"
    #define WF_S_WEATHER_HEAVY_SNOW "Heavy Snow"
    #define WF_S_WEATHER_SNOW_GRAINS "Snow Grains"
    #define WF_S_WEATHER_LIGHT_SHOWERS "Light Showers"
    #define WF_S_WEATHER_SHOWERS "Showering"
    #define WF_S_WEATHER_HEAVY_SHOWERS "Heavy Showers"
    #define WF_S_WEATHER_SNOW_SHOWERS "Snow Showers"
    #define WF_S_WEATHER_THUNDERSTORM "Thunderstorm"
    #define WF_S_WEATHER_LIGHT_HAIL "Light Hail"
    #define WF_S_WEATHER_HEAVY_HAIL "Heavy Hail"
    #define WF_S_WEATHER_UNKNOWN "Unknown"
    
    // Day names
    #define WF_S_DAY_MON "MON"
    #define WF_S_DAY_TUE "TUE"
    #define WF_S_DAY_WED "WED"
    #define WF_S_DAY_THU "THU"
    #define WF_S_DAY_FRI "FRI"
    #define WF_S_DAY_SAT "SAT"
    #define WF_S_DAY_SUN "SUN"

#elif INKWATCHY_LANG == PL
    // Weather conditions (Polish without diacritics)
    #define WF_S_WEATHER_CLEAR_SKY "Bezchmurnie"
    #define WF_S_WEATHER_MOSTLY_CLEAR "Prawie czysto"
    #define WF_S_WEATHER_PARTLY_CLOUDY "Lekkie chmury"
    #define WF_S_WEATHER_OVERCAST "Pochmurno"
    #define WF_S_WEATHER_FOG "Mgla"
    #define WF_S_WEATHER_HEAVY_FOG "Gesta mgla"
    #define WF_S_WEATHER_LIGHT_DRIZZLE "Lekka mzawka"
    #define WF_S_WEATHER_DRIZZLE "Mzawka"
    #define WF_S_WEATHER_HEAVY_DRIZZLE "Mzawka"
    #define WF_S_WEATHER_LIGHT_FREEZING "Lekkie zamrz."
    #define WF_S_WEATHER_HEAVY_FREEZING "Lodowato"
    #define WF_S_WEATHER_LIGHT_RAIN "Lekki deszcz"
    #define WF_S_WEATHER_RAIN "Deszcz"
    #define WF_S_WEATHER_HEAVY_RAIN "Ulewny deszcz"
    #define WF_S_WEATHER_FREEZING_RAIN "Marznacy"
    #define WF_S_WEATHER_FREEZING_HARD "Zamiez"
    #define WF_S_WEATHER_LIGHT_SNOW "Lekki snieg"
    #define WF_S_WEATHER_SNOW "Snieg"
    #define WF_S_WEATHER_HEAVY_SNOW "Snieznica"
    #define WF_S_WEATHER_SNOW_GRAINS "Ziarna"
    #define WF_S_WEATHER_LIGHT_SHOWERS "Przelotnie"
    #define WF_S_WEATHER_SHOWERS "Deszczowo"
    #define WF_S_WEATHER_HEAVY_SHOWERS "Opady"
    #define WF_S_WEATHER_SNOW_SHOWERS "Sniezne"
    #define WF_S_WEATHER_THUNDERSTORM "Burza"
    #define WF_S_WEATHER_LIGHT_HAIL "Lekki grad"
    #define WF_S_WEATHER_HEAVY_HAIL "Gradowo"
    #define WF_S_WEATHER_UNKNOWN "Nieznane"
    
    // Day names
    #define WF_S_DAY_MON "PON"
    #define WF_S_DAY_TUE "WTO"
    #define WF_S_DAY_WED "SRO"
    #define WF_S_DAY_THU "CZW"
    #define WF_S_DAY_FRI "PIA"
    #define WF_S_DAY_SAT "SOB"
    #define WF_S_DAY_SUN "NIE"

#elif INKWATCHY_LANG == DE
    // German weather conditions
    #define WF_S_WEATHER_CLEAR_SKY "Klarer Himmel"
    #define WF_S_WEATHER_MOSTLY_CLEAR "Meist klar"
    #define WF_S_WEATHER_PARTLY_CLOUDY "Teilw. bewolkt"
    #define WF_S_WEATHER_OVERCAST "Bedeckt"
    #define WF_S_WEATHER_FOG "Neblig"
    #define WF_S_WEATHER_HEAVY_FOG "Dichter Nebel"
    #define WF_S_WEATHER_LIGHT_DRIZZLE "Leichter Regen"
    #define WF_S_WEATHER_DRIZZLE "Nieselregen"
    #define WF_S_WEATHER_HEAVY_DRIZZLE "Starker Niesel"
    #define WF_S_WEATHER_LIGHT_FREEZING "Leicht gefrierend"
    #define WF_S_WEATHER_HEAVY_FREEZING "Stark gefrierend"
    #define WF_S_WEATHER_LIGHT_RAIN "Leichter Regen"
    #define WF_S_WEATHER_RAIN "Regen"
    #define WF_S_WEATHER_HEAVY_RAIN "Starker Regen"
    #define WF_S_WEATHER_FREEZING_RAIN "Eisregen"
    #define WF_S_WEATHER_FREEZING_HARD "Starker Eisregen"
    #define WF_S_WEATHER_LIGHT_SNOW "Leichter Schnee"
    #define WF_S_WEATHER_SNOW "Schnee"
    #define WF_S_WEATHER_HEAVY_SNOW "Starker Schnee"
    #define WF_S_WEATHER_SNOW_GRAINS "Schneekörner"
    #define WF_S_WEATHER_LIGHT_SHOWERS "Leichte Schauer"
    #define WF_S_WEATHER_SHOWERS "Schauer"
    #define WF_S_WEATHER_HEAVY_SHOWERS "Starke Schauer"
    #define WF_S_WEATHER_SNOW_SHOWERS "Schneeschauer"
    #define WF_S_WEATHER_THUNDERSTORM "Gewitter"
    #define WF_S_WEATHER_LIGHT_HAIL "Leichter Hagel"
    #define WF_S_WEATHER_HEAVY_HAIL "Starker Hagel"
    #define WF_S_WEATHER_UNKNOWN "Unbekannt"
    
    // Day names
    #define WF_S_DAY_MON "MO"
    #define WF_S_DAY_TUE "DI"
    #define WF_S_DAY_WED "MI"
    #define WF_S_DAY_THU "DO"
    #define WF_S_DAY_FRI "FR"
    #define WF_S_DAY_SAT "SA"
    #define WF_S_DAY_SUN "SO"

#else
    // Default to English if language not supported
    #define WF_S_WEATHER_CLEAR_SKY "Clear Sky"
    #define WF_S_WEATHER_MOSTLY_CLEAR "Mostly Clear"
    #define WF_S_WEATHER_PARTLY_CLOUDY "Partly Cloudy"
    #define WF_S_WEATHER_OVERCAST "Overcast"
    #define WF_S_WEATHER_FOG "Foggy"
    #define WF_S_WEATHER_HEAVY_FOG "Heavy Fog"
    #define WF_S_WEATHER_LIGHT_DRIZZLE "Light Drizzle"
    #define WF_S_WEATHER_DRIZZLE "Drizzling"
    #define WF_S_WEATHER_HEAVY_DRIZZLE "Heavy Drizzle"
    #define WF_S_WEATHER_LIGHT_FREEZING "Light Freezing"
    #define WF_S_WEATHER_HEAVY_FREEZING "Heavy Freezing"
    #define WF_S_WEATHER_LIGHT_RAIN "Light Rain"
    #define WF_S_WEATHER_RAIN "Raining"
    #define WF_S_WEATHER_HEAVY_RAIN "Heavy Rain"
    #define WF_S_WEATHER_FREEZING_RAIN "Freezing Rain"
    #define WF_S_WEATHER_FREEZING_HARD "Freezing Hard"
    #define WF_S_WEATHER_LIGHT_SNOW "Light Snow"
    #define WF_S_WEATHER_SNOW "Snowing"
    #define WF_S_WEATHER_HEAVY_SNOW "Heavy Snow"
    #define WF_S_WEATHER_SNOW_GRAINS "Snow Grains"
    #define WF_S_WEATHER_LIGHT_SHOWERS "Light Showers"
    #define WF_S_WEATHER_SHOWERS "Showering"
    #define WF_S_WEATHER_HEAVY_SHOWERS "Heavy Showers"
    #define WF_S_WEATHER_SNOW_SHOWERS "Snow Showers"
    #define WF_S_WEATHER_THUNDERSTORM "Thunderstorm"
    #define WF_S_WEATHER_LIGHT_HAIL "Light Hail"
    #define WF_S_WEATHER_HEAVY_HAIL "Heavy Hail"
    #define WF_S_WEATHER_UNKNOWN "Unknown"
    
    #define WF_S_DAY_MON "MON"
    #define WF_S_DAY_TUE "TUE"
    #define WF_S_DAY_WED "WED"
    #define WF_S_DAY_THU "THU"
    #define WF_S_DAY_FRI "FRI"
    #define WF_S_DAY_SAT "SAT"
    #define WF_S_DAY_SUN "SUN"
#endif

// ==============================================================================
// TAYCHRON WATCHFACE TRANSLATIONS
// ==============================================================================

#if INKWATCHY_LANG == EN
    #define WF_T_ERROR "ERR"
    
#elif INKWATCHY_LANG == PL
    #define WF_T_ERROR "BLAD"
    
#elif INKWATCHY_LANG == DE
    #define WF_T_ERROR "FEHLER"
    
#else
    #define WF_T_ERROR "ERR"
#endif

// ==============================================================================
// INKFIELD WATCHFACE TRANSLATIONS
// ==============================================================================

#if INKWATCHY_LANG == EN
    #define WF_I_ERROR "ERR"
    #define WF_I_ERROR_SHORT "ER"
    
#elif INKWATCHY_LANG == PL
    #define WF_I_ERROR "BLAD"
    #define WF_I_ERROR_SHORT "BL"
    
#elif INKWATCHY_LANG == DE
    #define WF_I_ERROR "FEHLER"
    #define WF_I_ERROR_SHORT "FE"
    
#else
    #define WF_I_ERROR "ERR"
    #define WF_I_ERROR_SHORT "ER"
#endif

// ==============================================================================
// COMMON TIME FORMAT TRANSLATIONS
// ==============================================================================

#if INKWATCHY_LANG == EN
    #define WF_TIME_AM "AM"
    #define WF_TIME_PM "PM"
    
#elif INKWATCHY_LANG == PL
    #define WF_TIME_AM "AM"
    #define WF_TIME_PM "PM"
    
#elif INKWATCHY_LANG == DE
    #define WF_TIME_AM "AM"
    #define WF_TIME_PM "PM"
    
#else
    #define WF_TIME_AM "AM"
    #define WF_TIME_PM "PM"
#endif

// ==============================================================================
// ADDITIONAL TRANSLATIONS (from original localization.h)
// ==============================================================================

#if INKWATCHY_LANG == EN
    // Month names
    #define LANGUAGE_MONTH_NAMES {"JAN", "FEB", "MAR", "APR", "MAY", "JUN", "JUL", "AUG", "SEP", "OCT", "NOV", "DEC"}
    
    // Menu translations
    #define MENU_MAIN "Main menu"
    #define MENU_CALENDAR "Calendar"
    #define MENU_SELECT_BOOK "Select book"
    #define MENU_ALARMS "Alarms"
    #define MENU_WEATHER "Weather"
    #define MENU_SETTINGS "Settings"
    #define MENU_HEART_MONITOR "Heart monitor"
    #define MENU_VAULT "Vault"
    #define MENU_WIFI_TOOL "Wifi tool"
    #define MENU_EATING_APPLES "Eating apples"
    #define MENU_SMASHING_APPLES "Smashing apples"
    #define MENU_GAMES "Games"
    #define MENU_CREDITS "Credits"
    #define MENU_WIFI "Wifi"
    #define MENU_DEBUG "Debug"
    #define MENU_POWER_SETTINGS "Power settings"
    #define MENU_CHANGE_WATCHFACE "Change watchface"
    #define MENU_TETRIS "Blockchy"
    #define MENU_PONG "Pong"
    #define MENU_VIDEO_PLAYER "Video player"
    #define MENU_CONWAY "Conway"
    #define MENU_PARTY "Party"
    #define MENU_VIBRATIONS_DIS "Vibrations disabled"
    #define MENU_WAKE_UP_DIS "Wake up disabled"

#elif INKWATCHY_LANG == PL
    // Month names
    #define LANGUAGE_MONTH_NAMES {"STY", "LUT", "MAR", "KWI", "MAJ", "CZE", "LIP", "SIE", "WRZ", "PAZ", "LIS", "GRU"}
    
    // Menu translations
    #define MENU_MAIN "Menu glowne"
    #define MENU_CALENDAR "Kalendarz"
    #define MENU_SELECT_BOOK "Wybierz ksiazke"
    #define MENU_ALARMS "Alarmy"
    #define MENU_WEATHER "Pogoda"
    #define MENU_SETTINGS "Ustawienia"
    #define MENU_HEART_MONITOR "Monitor serca"
    #define MENU_VAULT "Sejf"
    #define MENU_WIFI_TOOL "Narzedzie WiFi"
    #define MENU_EATING_APPLES "Jedzenie jablek"
    #define MENU_SMASHING_APPLES "Rozbijanie jablek"
    #define MENU_GAMES "Gry"
    #define MENU_CREDITS "Autorzy"
    #define MENU_WIFI "WiFi"
    #define MENU_DEBUG "Debugowanie"
    #define MENU_POWER_SETTINGS "Ustawienia zasilania"
    #define MENU_CHANGE_WATCHFACE "Zmien tarcze"
    #define MENU_TETRIS "Blockchy"
    #define MENU_PONG "Pong"
    #define MENU_VIDEO_PLAYER "Odtwarzacz video"
    #define MENU_CONWAY "Conway"
    #define MENU_PARTY "Impreza"
    #define MENU_VIBRATIONS_DIS "Wibracje wylaczone"
    #define MENU_WAKE_UP_DIS "Wybudzenie wylaczone"

#elif INKWATCHY_LANG == DE
    // Month names
    #define LANGUAGE_MONTH_NAMES {"JAN", "FEB", "MAR", "APR", "MAI", "JUN", "JUL", "AUG", "SEP", "OKT", "NOV", "DEZ"}
    
    // Menu translations
    #define MENU_MAIN "Hauptmenu"
    #define MENU_CALENDAR "Kalender"
    #define MENU_SELECT_BOOK "Buch wahlen"
    #define MENU_ALARMS "Alarme"
    #define MENU_WEATHER "Wetter"
    #define MENU_SETTINGS "Einstellungen"
    #define MENU_HEART_MONITOR "Herzmonitor"
    #define MENU_VAULT "Tresor"
    #define MENU_WIFI_TOOL "WiFi Tool"
    #define MENU_EATING_APPLES "Apfel essen"
    #define MENU_SMASHING_APPLES "Apfel zerquetschen"
    #define MENU_GAMES "Spiele"
    #define MENU_CREDITS "Credits"
    #define MENU_WIFI "WiFi"
    #define MENU_DEBUG "Debug"
    #define MENU_POWER_SETTINGS "Energie-Einstel."
    #define MENU_CHANGE_WATCHFACE "Zifferblatt and."
    #define MENU_TETRIS "Blockchy"
    #define MENU_PONG "Pong"
    #define MENU_VIDEO_PLAYER "Video Player"
    #define MENU_CONWAY "Conway"
    #define MENU_PARTY "Party"
    #define MENU_VIBRATIONS_DIS "Vibration aus"
    #define MENU_WAKE_UP_DIS "Aufwachen aus"

#else
    #error "Unsupported language! Please define INKWATCHY_LANG as EN, PL, or DE in config.h"
#endif

// ==============================================================================
// HELPER MACROS
// ==============================================================================

// Helper macro for building day name arrays at compile time
#define LANGUAGE_DAY_NAMES { \
    WF_S_DAY_MON, WF_S_DAY_TUE, WF_S_DAY_WED, WF_S_DAY_THU, \
    WF_S_DAY_FRI, WF_S_DAY_SAT, WF_S_DAY_SUN \
}

// Helper function to get localized month name
inline String getLocalizedMonthName(int month) {
    static const String monthNames[] = LANGUAGE_MONTH_NAMES;
    if (month >= 0 && month < 12) {
        return monthNames[month];
    }
    return "???";
}

// Helper function to get localized day name by day of week index
// dayOfWeek: 0 = Sunday, 1 = Monday, ..., 6 = Saturday
// offset: days to add/subtract from current day
inline String getLocalizedDayByIndex(int dayOfWeek, int offset = 0) {
    static const String dayNames[] = LANGUAGE_DAY_NAMES;
    
    // Apply offset and wrap around
    int dayIndex = (dayOfWeek + offset + 7) % 7;
    
    if (dayIndex >= 0 && dayIndex < 7) {
        return dayNames[dayIndex];
    }
    return "???";
}

// Convenience function for getting current day name with offset
// This should be used where timeRTCLocal is available
inline String getCurrentLocalizedDayName(int offset = 0) {
    extern tmElements_t timeRTCLocal;  // Forward declaration
    return getLocalizedDayByIndex(timeRTCLocal.Wday, offset);
}

// ==============================================================================
// TEMPERATURE FORMATTING FUNCTIONS
// ==============================================================================

// Temperature formatting functions are defined in temperature.h
// No need to redeclare them here