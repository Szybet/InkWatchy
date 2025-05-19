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
// COMPILE-TIME VALIDATION
// ==============================================================================

// Ensure all required macros are defined for the selected language
#if !defined(WF_S_WEATHER_CLEAR_SKY) || !defined(WF_S_DAY_MON) || !defined(MENU_MAIN)
    #error "Localization file is incomplete or missing required definitions!"
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
 */