#pragma once

#include "config.h"
#include "../hardware/temp/temperature.h"

// Language definitions
#define EN 1
#define PL 2
#define DE 3
#define SK 4
#define IT 5

// Ensure INKWATCHY_LANG is defined
#ifndef INKWATCHY_LANG
#define INKWATCHY_LANG EN
#endif

#if INKWATCHY_LANG == EN
#include "languages/localization_en.h"
#elif INKWATCHY_LANG == PL
#include "languages/localization_pl.h"
#elif INKWATCHY_LANG == DE
#include "languages/localization_de.h"
#elif INKWATCHY_LANG == SK
#include "languages/localization_sk.h"
#elif INKWATCHY_LANG == IT
#include "languages/localization_it.h"
#else // Default fallback to English
#include "languages/localization_en.h"
#warning "Unsupported language! Defaulting to English. Please define INKWATCHY_LANG as EN, PL, DE, or SK in config.h"
#endif

// Helper macro for building day name arrays at compile time
// Array starts with Sunday (index 0) to match standard Wday convention
#define LANGUAGE_DAY_NAMES {                                \
    WF_S_DAY_SUN, WF_S_DAY_MON, WF_S_DAY_TUE, WF_S_DAY_WED, \
    WF_S_DAY_THU, WF_S_DAY_FRI, WF_S_DAY_SAT}

// Helper function to get localized month name
inline String getLocalizedMonthName(int month)
{
    static const String monthNames[] = LANGUAGE_MONTH_NAMES;
    if (month >= 0 && month < 12)
    {
        return monthNames[month];
    }
    return "???";
}

// Helper function to get localized day name by day of week index
// dayOfWeek: 0 = Sunday, 1 = Monday, ..., 6 = Saturday
// offset: days to add/subtract from current day
inline String getLocalizedDayByIndex(int dayOfWeek, int offset = 0)
{
    static const String dayNames[] = LANGUAGE_DAY_NAMES;

    // Apply offset and wrap around
    int dayIndex = (dayOfWeek + offset + 6) % 7;

    if (dayIndex >= 0 && dayIndex < 7)
    {
        return dayNames[dayIndex];
    }
    return "???";
}

// Convenience function for getting current day name with offset
// This should be used where timeRTCLocal is available
inline String getCurrentLocalizedDayName(int offset = 0)
{
    extern tmElements_t timeRTCLocal; // Forward declaration
    return getLocalizedDayByIndex(timeRTCLocal.Wday, offset);
}
