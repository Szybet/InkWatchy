#pragma once

#include "config.h"
#include "../hardware/temp/temperature.h"

// ==============================================================================
// LANGUAGE CONFIGURATION
// ==============================================================================

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

// ==============================================================================
// INCLUDE THE APPROPRIATE LANGUAGE FILE
// ==============================================================================

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

// ==============================================================================
// HELPER MACROS AND FUNCTIONS
// ==============================================================================

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

// ==============================================================================
// COMPILE-TIME VALIDATION
// ==============================================================================

// Check basic watchface weather conditions
#if !defined(WF_S_WEATHER_CLEAR_SKY) || !defined(WF_S_WEATHER_MOSTLY_CLEAR) || !defined(WF_S_WEATHER_PARTLY_CLOUDY) ||     \
    !defined(WF_S_WEATHER_OVERCAST) || !defined(WF_S_WEATHER_FOG) || !defined(WF_S_WEATHER_HEAVY_FOG) ||                   \
    !defined(WF_S_WEATHER_LIGHT_DRIZZLE) || !defined(WF_S_WEATHER_DRIZZLE) || !defined(WF_S_WEATHER_HEAVY_DRIZZLE) ||      \
    !defined(WF_S_WEATHER_LIGHT_FREEZING) || !defined(WF_S_WEATHER_HEAVY_FREEZING) || !defined(WF_S_WEATHER_LIGHT_RAIN) || \
    !defined(WF_S_WEATHER_RAIN) || !defined(WF_S_WEATHER_HEAVY_RAIN) || !defined(WF_S_WEATHER_FREEZING_RAIN) ||            \
    !defined(WF_S_WEATHER_FREEZING_HARD) || !defined(WF_S_WEATHER_LIGHT_SNOW) || !defined(WF_S_WEATHER_SNOW) ||            \
    !defined(WF_S_WEATHER_HEAVY_SNOW) || !defined(WF_S_WEATHER_SNOW_GRAINS) || !defined(WF_S_WEATHER_LIGHT_SHOWERS) ||     \
    !defined(WF_S_WEATHER_SHOWERS) || !defined(WF_S_WEATHER_HEAVY_SHOWERS) || !defined(WF_S_WEATHER_SNOW_SHOWERS) ||       \
    !defined(WF_S_WEATHER_THUNDERSTORM) || !defined(WF_S_WEATHER_LIGHT_HAIL) || !defined(WF_S_WEATHER_HEAVY_HAIL) ||       \
    !defined(WF_S_WEATHER_UNKNOWN)
#error "Localization file is missing weather condition definitions."
#endif

// Check day names (abbreviated)
#if !defined(WF_S_DAY_MON) || !defined(WF_S_DAY_TUE) || !defined(WF_S_DAY_WED) || !defined(WF_S_DAY_THU) || \
    !defined(WF_S_DAY_FRI) || !defined(WF_S_DAY_SAT) || !defined(WF_S_DAY_SUN)
#error "Localization file is missing day name definitions."
#endif

// Check error messages
#if !defined(WF_T_ERROR) || !defined(WF_I_ERROR) || !defined(WF_I_ERROR_SHORT)
#error "Localization file is missing error message definitions."
#endif

// Check time format
#if !defined(WF_TIME_AM) || !defined(WF_TIME_PM)
#error "Localization file is missing time format definitions."
#endif

// Check weather/air quality menu items
#if !defined(WEATHER_AQ_MENU_TITLE) || !defined(WEATHER_AQ_MENU_WEATHER) || !defined(WEATHER_AQ_MENU_AIR_QUALITY) || \
    !defined(WEATHER_AQ_NOT_AVAILABLE)
#error "Localization file is missing weather/air quality selection menu definitions."
#endif

// Check weather submenu items
#if !defined(WEATHER_MENU_TEMPERATURE) || !defined(WEATHER_MENU_PRESSURE) || !defined(WEATHER_MENU_HUMIDITY) ||                            \
    !defined(WEATHER_MENU_CONDITIONS) || !defined(WEATHER_MENU_CLOUDINESS) || !defined(WEATHER_MENU_WIND_SPEED) ||                         \
    !defined(WEATHER_MENU_WIND_GUSTS) || !defined(WEATHER_MENU_VISIBILITY) || !defined(WEATHER_MENU_PRECIPITATION) ||                      \
    !defined(WEATHER_MENU_UV_INDEX) || !defined(WEATHER_MENU_UV_INDEX_CLEAR_SKY) || !defined(WEATHER_MENU_WET_BULB) ||                     \
    !defined(WEATHER_MENU_CAPE) || !defined(WEATHER_MENU_DEW_POINT) || !defined(WEATHER_MENU_PRECIPITATION_AMOUNT) ||                      \
    !defined(WEATHER_MENU_SNOW_DEPTH) || !defined(WEATHER_MENU_LIFTED_INDEX) || !defined(WEATHER_MENU_CONVECTIVE_INHIBITION) ||            \
    !defined(WEATHER_MENU_TERRESTRIAL_RADIATION) || !defined(WEATHER_MENU_DIFFUSE_RADIATION) || !defined(WEATHER_MENU_DIRECT_RADIATION) || \
    !defined(WEATHER_MENU_TOTAL_WATER_VAPOUR)
#error "Localization file is missing weather submenu definitions."
#endif

// Check air quality submenu items
#if !defined(AIR_MENU_EUAQI) || !defined(AIR_MENU_USAQI) || !defined(AIR_MENU_EU_AQI_PM2_5) ||                        \
    !defined(AIR_MENU_EU_AQI_PM10) || !defined(AIR_MENU_EU_AQI_NO2) || !defined(AIR_MENU_EU_AQI_O3) ||                \
    !defined(AIR_MENU_EU_AQI_SO2) || !defined(AIR_MENU_PM2_5) || !defined(AIR_MENU_PM10) ||                           \
    !defined(AIR_MENU_CARBON_MONOXIDE) || !defined(AIR_MENU_CARBON_DIOXIDE) || !defined(AIR_MENU_NITROGEN_DIOXIDE) || \
    !defined(AIR_MENU_SULPHUR_DIOXIDE) || !defined(AIR_MENU_OZONE) || !defined(AIR_MENU_AEROSOL_OPTICAL_DEPTH) ||     \
    !defined(AIR_MENU_DUST) || !defined(AIR_MENU_METHANE) || !defined(AIR_MENU_FORMALDEHYDE) ||                       \
    !defined(AIR_MENU_GLYOXAL) || !defined(AIR_MENU_SEA_SALT_AEROSOL) || !defined(AIR_MENU_NITROGEN_MONOXIDE) ||      \
    !defined(AIR_MENU_PEROXYACYL_NITRATES)
#error "Localization file is missing air quality submenu definitions."
#endif

// Check weather menu titles and messages
#if !defined(WEATHER_SELECT_DATE) || !defined(WEATHER_STAT_TITLE) || !defined(WEATHER_NOT_AVAILABLE) || \
    !defined(WEATHER_DATE_WRONG) || !defined(WEATHER_CORRUPTED)
#error "Localization file is missing weather menu title definitions."
#endif

// Check air quality menu titles and messages
#if !defined(AIR_QUALITY_SELECT_DATE) || !defined(AIR_QUALITY_STAT_TITLE) || !defined(AIR_QUALITY_NOT_AVAILABLE) || \
    !defined(AIR_QUALITY_DATE_WRONG) || !defined(AIR_QUALITY_CORRUPTED)
#error "Localization file is missing air quality menu title definitions."
#endif

// Check weather chart titles
#if !defined(WEATHER_CHART_TEMP) || !defined(WEATHER_CHART_PRESSURE) || !defined(WEATHER_CHART_HUMIDITY) ||                                   \
    !defined(WEATHER_CHART_CLOUDS) || !defined(WEATHER_CHART_WIND_SPEED) || !defined(WEATHER_CHART_WIND_GUSTS) ||                             \
    !defined(WEATHER_CHART_VISIBILITY) || !defined(WEATHER_CHART_PRECIPITATION) || !defined(WEATHER_CONDITIONS_TITLE) ||                      \
    !defined(WEATHER_CHART_UV_INDEX) || !defined(WEATHER_CHART_UV_INDEX_CLEAR_SKY) || !defined(WEATHER_CHART_WET_BULB) ||                     \
    !defined(WEATHER_CHART_CAPE) || !defined(WEATHER_CHART_DEW_POINT) || !defined(WEATHER_CHART_PRECIPITATION_AMOUNT) ||                      \
    !defined(WEATHER_CHART_SNOW_DEPTH) || !defined(WEATHER_CHART_LIFTED_INDEX) || !defined(WEATHER_CHART_CONVECTIVE_INHIBITION) ||            \
    !defined(WEATHER_CHART_TERRESTRIAL_RADIATION) || !defined(WEATHER_CHART_DIFFUSE_RADIATION) || !defined(WEATHER_CHART_DIRECT_RADIATION) || \
    !defined(WEATHER_CHART_TOTAL_WATER_VAPOUR)
#error "Localization file is missing weather chart definitions."
#endif

// Check air quality chart titles
#if !defined(AIR_CHART_EUAQI) || !defined(AIR_CHART_USAQI) || !defined(AIR_CHART_EU_AQI_PM2_5) ||                        \
    !defined(AIR_CHART_EU_AQI_PM10) || !defined(AIR_CHART_EU_AQI_NO2) || !defined(AIR_CHART_EU_AQI_O3) ||                \
    !defined(AIR_CHART_EU_AQI_SO2) || !defined(AIR_CHART_PM2_5) || !defined(AIR_CHART_PM10) ||                           \
    !defined(AIR_CHART_CARBON_MONOXIDE) || !defined(AIR_CHART_CARBON_DIOXIDE) || !defined(AIR_CHART_NITROGEN_DIOXIDE) || \
    !defined(AIR_CHART_SULPHUR_DIOXIDE) || !defined(AIR_CHART_OZONE) || !defined(AIR_CHART_AEROSOL_OPTICAL_DEPTH) ||     \
    !defined(AIR_CHART_DUST) || !defined(AIR_CHART_METHANE) || !defined(AIR_CHART_FORMALDEHYDE) ||                       \
    !defined(AIR_CHART_GLYOXAL) || !defined(AIR_CHART_SEA_SALT_AEROSOL) || !defined(AIR_CHART_NITROGEN_MONOXIDE) ||      \
    !defined(AIR_CHART_PEROXYACYL_NITRATES)
#error "Localization file is missing air quaity chart definitions."
#endif

// Check month names
#if !defined(LANGUAGE_MONTH_NAMES)
#error "Localization file is missing month name definitions."
#endif

// Check main menu translations
#if !defined(MENU_MAIN) || !defined(MENU_CALENDAR) || !defined(MENU_SELECT_BOOK) || !defined(MENU_ALARMS) ||              \
    !defined(MENU_WEATHER) || !defined(MENU_SETTINGS) || !defined(MENU_HEART_MONITOR) || !defined(MENU_HEALTH) || !defined(MENU_STEPS_PER_DAY) || !defined(MENU_VAULT) ||          \
    !defined(MENU_WIFI_TOOL) || !defined(MENU_EATING_APPLES) || !defined(MENU_SMASHING_APPLES) || !defined(MENU_GAMES) || \
    !defined(MENU_CREDITS) || !defined(MENU_WIFI) || !defined(MENU_DEBUG) || !defined(MENU_POWER_SETTINGS) ||             \
    !defined(MENU_CHANGE_WATCHFACE) || !defined(MENU_TETRIS) || !defined(MENU_PONG) || !defined(MENU_VIDEO_PLAYER) ||     \
    !defined(MENU_CONWAY) || !defined(MENU_PARTY) || !defined(MENU_VIBRATIONS_DIS) || !defined(MENU_WAKE_UP_DIS) ||       \
    !defined(MENU_POWER)
#error "Localization file is missing main menu definitions."
#endif

// Check app and book translations
#if !defined(BOOK_NO_SELECTED) || !defined(CALENDAR_NO_DATA)
#error "Localization file is missing app/book definitions."
#endif

// Check heart monitor translations
#if !defined(HEART_MONITOR_TITLE) || !defined(HEART_MONITOR_NOT_MEDICAL) || !defined(HEART_MONITOR_PLACE_DEVICE) || \
    !defined(HEART_MONITOR_CLICK_MENU) || !defined(HEART_MONITOR_START_3S) || !defined(HEART_MONITOR_TAKES_15S) ||  \
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

// Check Tetris game translations
#if !defined(TETRIS_LINE_CLEARS) || !defined(TETRIS_SCORE) || !defined(TETRIS_LEVEL) || \
    !defined(TETRIS_CONTROLS_UP) || !defined(TETRIS_CONTROLS_DOWN) || !defined(TETRIS_GAME_OVER)
#error "Localization file is missing Tetris game definitions."
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
#if !defined(DEBUG_ACC_DISABLED) || !defined(DEBUG_ACC_FAILED_INIT) || !defined(DEBUG_ACC_DAMAGED) ||                  \
    !defined(DEBUG_ACC_WORKING) || !defined(DEBUG_ACC_CLICK_3D) || !defined(DEBUG_ACC_X) || !defined(DEBUG_ACC_Y) ||   \
    !defined(DEBUG_ACC_Z) || !defined(DEBUG_ACC_PURE_X) || !defined(DEBUG_ACC_PURE_Y) || !defined(DEBUG_ACC_PURE_Z) || \
    !defined(DEBUG_ACC_STEPS) || !defined(DEBUG_ACC_IC) || !defined(DEBUG_ACC_ERROR_CODE) || !defined(DEBUG_ACC_STATUS_CODE)
#error "Localization file is missing accelerometer debug definitions."
#endif

// Check battery debug translations
#if !defined(DEBUG_BATTERY_CURRENT_V) || !defined(DEBUG_BATTERY_MINIMUM_V) || !defined(DEBUG_BATTERY_MAXIMUM_V) || \
    !defined(DEBUG_BATTERY_CRITICAL_V) || !defined(DEBUG_BATTERY_LEVEL) || !defined(DEBUG_BATTERY_CHARGING) ||     \
    !defined(DEBUG_BATTERY_FULLY)
#error "Localization file is missing battery debug definitions."
#endif

// Check clock debug translations
#if !defined(DEBUG_CLOCK_DRIFT_SYNCS) || !defined(DEBUG_CLOCK_NOT_AVAILABLE) || !defined(DEBUG_CLOCK_LAST_SYNC) || \
    !defined(DEBUG_CLOCK_PREVIOUS_SYNC) || !defined(DEBUG_CLOCK_LAST_CHARGE)
#error "Localization file is missing clock debug definitions."
#endif

// Check git debug translations
#if !defined(DEBUG_GIT_COMMIT_HASH) || !defined(DEBUG_GIT_BRANCH) || !defined(DEBUG_GIT_BUILD_TIME)
#error "Localization file is missing git debug definitions."
#endif

// Check hardware debug translations
#if !defined(DEBUG_HW_CHIP_MODEL) || !defined(DEBUG_HW_RTC_TYPE) || !defined(DEBUG_HW_USED_HEAP) || \
    !defined(DEBUG_HW_CPU_TEMP) || !defined(DEBUG_HW_INIT_TEMP)
#error "Localization file is missing hardware debug definitions."
#endif

// Check RTC type translations
#if !defined(DEBUG_RTC_UNKNOWN) || !defined(DEBUG_RTC_DS3231) || !defined(DEBUG_RTC_PCF8563) || \
    !defined(DEBUG_RTC_INTERNAL) || !defined(DEBUG_RTC_INVALID)
#error "Localization file is missing RTC type definitions."
#endif

// Check WiFi debug translations
#if !defined(DEBUG_WIFI_MAC_ADDRESS) || !defined(DEBUG_WIFI_STATUS) || !defined(DEBUG_WIFI_IP) ||  \
    !defined(DEBUG_WIFI_SSID) || !defined(DEBUG_WIFI_SIGNAL) || !defined(DEBUG_WIFI_CONNECTING) || \
    !defined(DEBUG_WIFI_ON) || !defined(DEBUG_WIFI_OFF)
#error "Localization file is missing WiFi debug definitions."
#endif

// Check common debug terms
#if !defined(DEBUG_COMMON_PERCENT) || !defined(DEBUG_COMMON_COLON)
#error "Localization file is missing common debug term definitions."
#endif

// Check game and app translations
#if !defined(PONG_YOU_LOST) || !defined(PARTY_MESSAGE) || !defined(VAULT_KEY_INCORRECT) || !defined(VAULT_EMPTY)
#error "Localization file is missing game/app definitions."
#endif

// Check alarm menu titles
#if !defined(ALARM_MENU_EDIT_DAYS) || !defined(ALARM_MENU_EDIT_ALARM) || !defined(ALARM_MENU_ALARMS) ||                \
    !defined(ALARM_MENU_BROWSE_ALARMS) || !defined(ALARM_MENU_QUICK_ALARMS_SET) || !defined(ALARM_MENU_QUICK_ALARM) || \
    !defined(ALARM_MENU_POMODORO)
#error "Localization file is missing alarm menu definitions."
#endif

// Check alarm status and settings
#if !defined(ALARM_STATUS_ENABLED) || !defined(ALARM_STATUS_DISABLED) || !defined(ALARM_LABEL_TIME) || \
    !defined(ALARM_LABEL_ONE_TIME_ALARM) || !defined(ALARM_LABEL_DAYS) || !defined(ALARM_LABEL_REQUIRE_WIFI)
#error "Localization file is missing alarm status/settings definitions."
#endif

// Check quick alarm messages
#if !defined(ALARM_QUICK_ALERT_TITLE) || !defined(ALARM_QUICK_ALARM_SET_FOR) || !defined(ALARM_QUICK_WILL_RING_AT)
#error "Localization file is missing quick alarm message definitions."
#endif

// Check day names (full)
#if !defined(ALARM_DAY_MONDAY) || !defined(ALARM_DAY_TUESDAY) || !defined(ALARM_DAY_WEDNESDAY) || \
    !defined(ALARM_DAY_THURSDAY) || !defined(ALARM_DAY_FRIDAY) || !defined(ALARM_DAY_SATURDAY) || \
    !defined(ALARM_DAY_SUNDAY)
#error "Localization file is missing full day name definitions."
#endif

// Check alarm info strings
#if !defined(ALARM_INFO_ALL_DAYS) || !defined(ALARM_INFO_NEVER) || !defined(ALARM_INFO_ONCE) || \
    !defined(ALARM_INFO_WIFI) || !defined(ALARM_INFO_QUICK) || !defined(ALARM_INFO_POMODORO)
#error "Localization file is missing alarm info string definitions."
#endif

// Check Pomodoro strings
#if !defined(POMODORO_STATUS) || !defined(POMODORO_RUNNING) || !defined(POMODORO_WORKING) || \
    !defined(POMODORO_PAUSE) || !defined(POMODORO_ITER) || !defined(POMODORO_DISABLED) ||    \
    !defined(POMODORO_RESET_TURN_OFF) || !defined(POMODORO_START)
#error "Localization file is missing Pomodoro definitions."
#endif

// Check Set Clock GUI translations
#if !defined(SETCLOCK_SET_TIME) || !defined(SETCLOCK_SET_DATE) || !defined(SETCLOCK_TIMEZONE_MENU) || \
    !defined(SETCLOCK_CLOCK_SETTINGS) || !defined(SETCLOCK_CURRENT_TIMEZONE) || !defined(SETCLOCK_CLEAR_TIMEZONE) || \
    !defined(SETCLOCK_SET_UTC_TIMEZONE) || !defined(SETCLOCK_TIMEZONE_SETTINGS) || !defined(SETCLOCK_TIMEZONE_WARNING) || \
    !defined(SETCLOCK_WARNING_TITLE) || !defined(SETCLOCK_SELECT_TIMEZONE)
#error "Localization file is missing Set Clock GUI definitions."
#endif


// Check time unit translations
#if !defined(TIME_UNIT_MINUTE) || !defined(TIME_UNIT_MINUTES) || !defined(TIME_UNIT_HOUR) || \
    !defined(TIME_UNIT_HOURS) || !defined(TIME_UNIT_DAY) || !defined(TIME_UNIT_DAYS) || !defined(TIME_UNIT_AND)
#error "Localization file is missing time unit definitions."
#endif

// ==============================================================================
// NOTES FOR DEVELOPERS
// ==============================================================================

/*
 * ADDING A NEW LANGUAGE:
 *
 * STEP 1: Use the Translation Template
 * 1. Copy the file: src/templates/localization_template.h
 * 2. Rename it to: languages/localization_XX.h (where XX is your language code, e.g., FR, ES, IT)
 * 3. Fill in all empty strings "" using the English references in comments
 * 4. Follow the guidelines in the template (character limits, special characters, etc.)
 * 5. Use the checklist at the end of the template to verify completeness
 *
 * STEP 2: Register the Language in System
 * 6. Add a new language constant above: #define XX 5 (next available number)
 * 7. Add a new #elif case in the include section: #include "languages/localization_XX.h"
 * 8. Update the warning message to include your new language code
 * 9. Test compilation - the validation system will catch any missing translations
 * 10. Test functionality on actual device to ensure text fits properly
 *
 * TRANSLATION GUIDELINES:
 * - Keep abbreviations SHORT: Day names (2-4 chars), Month names (3-4 chars), Errors (2-3 chars)
 * - Test on actual watch hardware - text must fit on small screen
 * - Avoid diacritics/special characters if possible for better compatibility
 * - Weather chart titles should maintain format: "Parameter / Unit (24h)"
 * - Time units may need special handling for languages with complex plural rules
 * - Technical terms (like RTC types) often stay in English
 *
 * CURRENTLY SUPPORTED LANGUAGES:
 * - EN (English) - Reference implementation
 * - PL (Polish) - Full support
 * - DE (German) - Full support
 * - SK (Slovak) - Full support
 * - Template available for new languages
 *
 * MODIFYING EXISTING TRANSLATIONS:
 * - Only edit individual language files (localization_en.h, localization_pl.h, etc.)
 * - Do NOT modify this main localization.h file unless adding new language support
 * - Always test compilation after changes - validation will catch errors
 * - Consider impact on screen space when changing text length
 *
 * ADDING NEW TRANSLATABLE STRINGS:
 * 1. Add the new #define to ALL existing language files (EN, PL, DE, SK, template)
 * 2. Add validation check in the appropriate category above
 * 3. Document the new string in the template with English reference
 * 4. Test compilation with all languages to ensure completeness
 * 5. Update this documentation if creating new categories
 *
 * TECHNICAL DETAILS:
 *
 * Temperature Formatting:
 * - Temperature functions are in temperature.h
 * - Use formatTemperature() for consistent display across languages
 *
 * Compile-Time Validation System:
 * - Automatically checks 120+ required macro definitions during compilation
 * - Organized into logical categories (weather, menus, debug, alarms, etc.)
 * - Fails compilation with descriptive error if any translation is missing
 * - Ensures 100% translation completeness across all language files
 *
 * Validation Categories:
 * - Weather conditions: All WF_S_WEATHER_* definitions (28 variables)
 * - Day/time format: WF_S_DAY_*, WF_TIME_*, error messages (12 variables)
 * - Weather UI: WEATHER_MENU_*, WEATHER_CHART_*, WEATHER_* (18 variables)
 * - Main menu: All MENU_* definitions (25 variables)
 * - Applications: BOOK_*, CALENDAR_*, HEART_MONITOR_*, etc. (11 variables)
 * - Games: TETRIS_*, PONG_*, PARTY_*, VAULT_* (10 variables)
 * - Debug system: DEBUG_MENU_*, DEBUG_ITEM_*, DEBUG_*_* (45+ variables)
 * - Alarms & Pomodoro: ALARM_*, POMODORO_* (30+ variables)
 * - Time units: TIME_UNIT_* including plural forms (10 variables)
 *
 * TROUBLESHOOTING:
 * - "missing definitions" error -> Use template, check variable names match exactly
 * - Text doesn't fit on screen -> Shorten translations, use abbreviations
 * - Special characters display wrong -> Avoid diacritics or test on device
 * - Compilation fails -> Check all validation categories, ensure no empty strings
 */
