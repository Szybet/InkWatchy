#pragma once

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
// INCLUDE THE APPROPRIATE LANGUAGE FILE
// ==============================================================================

#if INKWATCHY_LANG == EN
    #include "languages/localization_en.h"
#elif INKWATCHY_LANG == PL
    #include "languages/localization_pl.h"
#elif INKWATCHY_LANG == DE
    #include "languages/localization_de.h"
#else
    // Default fallback to English
    #include "languages/localization_en.h"
    #warning "Unsupported language! Defaulting to English. Please define INKWATCHY_LANG as EN, PL, or DE in config.h"
#endif

// ==============================================================================
// HELPER MACROS AND FUNCTIONS
// ==============================================================================

// Helper macro for building day name arrays at compile time
// Array starts with Sunday (index 0) to match standard Wday convention
#define LANGUAGE_DAY_NAMES { \
    WF_S_DAY_SUN, WF_S_DAY_MON, WF_S_DAY_TUE, WF_S_DAY_WED, \
    WF_S_DAY_THU, WF_S_DAY_FRI, WF_S_DAY_SAT \
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
    int dayIndex = (dayOfWeek + offset + 6) % 7;
    
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
// COMPILE-TIME VALIDATION
// ==============================================================================

// Check basic watchface and menu translations
#if !defined(WF_S_WEATHER_CLEAR_SKY) || !defined(WF_S_DAY_MON) || !defined(MENU_MAIN)
    #error "Localization file is incomplete! Missing basic watchface or menu definitions."
#endif

// Check weather translations
#if !defined(WEATHER_MENU_TEMPERATURE) || !defined(WEATHER_NOT_AVAILABLE) || !defined(WEATHER_CHART_TEMP)
    #error "Localization file is missing weather-related definitions."
#endif

// Check main menu translations
#if !defined(MENU_SETTINGS) || !defined(MENU_DEBUG) || !defined(MENU_GAMES) || !defined(MENU_POWER)
    #error "Localization file is missing main menu definitions."
#endif

// Check debug menu titles
#if !defined(DEBUG_MENU_ACC) || !defined(DEBUG_MENU_BATTERY) || !defined(DEBUG_MENU_CLOCK) || \
    !defined(DEBUG_MENU_GIT) || !defined(DEBUG_MENU_HARDWARE) || !defined(DEBUG_MENU_WIFI)
    #error "Localization file is missing debug menu title definitions."
#endif

// Check debug menu items
#if !defined(DEBUG_ITEM_CLOCK) || !defined(DEBUG_ITEM_HARDWARE) || !defined(DEBUG_ITEM_BATTERY) || \
    !defined(DEBUG_ITEM_GIT) || !defined(DEBUG_ITEM_ACC) || !defined(DEBUG_ITEM_FONT_PREVIEW)
    #error "Localization file is missing debug menu item definitions."
#endif

// Check accelerometer debug translations
#if !defined(DEBUG_ACC_DISABLED) || !defined(DEBUG_ACC_WORKING) || !defined(DEBUG_ACC_X) || \
    !defined(DEBUG_ACC_STEPS) || !defined(DEBUG_ACC_IC)
    #error "Localization file is missing accelerometer debug definitions."
#endif

// Check battery debug translations
#if !defined(DEBUG_BATTERY_CURRENT_V) || !defined(DEBUG_BATTERY_CHARGING) || !defined(DEBUG_BATTERY_LEVEL)
    #error "Localization file is missing battery debug definitions."
#endif

// Check clock debug translations
#if !defined(DEBUG_CLOCK_DRIFT_SYNCS) || !defined(DEBUG_CLOCK_NOT_AVAILABLE) || !defined(DEBUG_CLOCK_LAST_SYNC)
    #error "Localization file is missing clock debug definitions."
#endif

// Check git debug translations
#if !defined(DEBUG_GIT_COMMIT_HASH) || !defined(DEBUG_GIT_BRANCH) || !defined(DEBUG_GIT_BUILD_TIME)
    #error "Localization file is missing git debug definitions."
#endif

// Check hardware debug translations
#if !defined(DEBUG_HW_CHIP_MODEL) || !defined(DEBUG_HW_RTC_TYPE) || !defined(DEBUG_HW_USED_HEAP) || \
    !defined(DEBUG_RTC_UNKNOWN) || !defined(DEBUG_RTC_DS3231)
    #error "Localization file is missing hardware debug definitions."
#endif

// Check WiFi debug translations
#if !defined(DEBUG_WIFI_MAC_ADDRESS) || !defined(DEBUG_WIFI_STATUS) || !defined(DEBUG_WIFI_IP) || \
    !defined(DEBUG_WIFI_ON) || !defined(DEBUG_WIFI_OFF)
    #error "Localization file is missing WiFi debug definitions."
#endif

// Check game and app translations
#if !defined(PONG_YOU_LOST) || !defined(PARTY_MESSAGE) || !defined(VAULT_KEY_INCORRECT) || !defined(VAULT_EMPTY)
    #error "Localization file is missing game/app definitions."
#endif

// Check common debug terms
#if !defined(DEBUG_COMMON_PERCENT) || !defined(DEBUG_COMMON_COLON)
    #error "Localization file is missing common debug term definitions."
#endif

// Check heart monitor translations
#if !defined(HEART_MONITOR_TITLE) || !defined(HEART_MONITOR_NOT_MEDICAL) || !defined(HEART_MONITOR_PLACE_DEVICE) || \
    !defined(HEART_MONITOR_CLICK_MENU) || !defined(HEART_MONITOR_START_3S) || !defined(HEART_MONITOR_TAKES_15S) || \
    !defined(HEART_MONITOR_BREATHE_SLOWLY) || !defined(HEART_MONITOR_TIME_LEFT) || !defined(HEART_MONITOR_BPM)
    #error "Localization file is missing heart monitor definitions."
#endif

// Check video player translations
#if !defined(VIDEO_PLAYER_CHOOSE)
    #error "Localization file is missing video player definitions."
#endif

// Check watchface selection translations
#if !defined(WATCHFACE_SELECT_TITLE)
    #error "Localization file is missing watchface selection definitions."
#endif

// ==============================================================================
// NOTES FOR DEVELOPERS
// ==============================================================================

/*
 * ADDING A NEW LANGUAGE:
 * 1. Create a new file: languages/localization_XX.h (where XX is the language code)
 * 2. Copy the structure from languages/localization_en.h
 * 3. Translate all the string definitions
 * 4. Add a new language constant: #define XX 4 (next available number)
 * 5. Add a new #elif case in the include section above: #include "languages/localization_XX.h"
 * 6. Test compilation and functionality
 * 
 * MODIFYING TRANSLATIONS:
 * - Only edit the individual language files (languages/localization_en.h, languages/localization_pl.h, etc.)
 * - Do NOT modify this main localization.h file unless adding support for new languages
 * 
 * TEMPERATURE FORMATTING:
 * - Temperature formatting functions are defined in temperature.h
 * - Use formatTemperature() function for consistent temperature display
 * 
 * COMPILE-TIME VALIDATION:
 * - The system automatically checks if all required macros are defined during compilation
 * - If a translation is missing, compilation will fail with a descriptive error message
 * - When adding new translatable strings, add them to the validation checks above
 */