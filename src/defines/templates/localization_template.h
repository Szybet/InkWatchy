#pragma once

// ==============================================================================
// TRANSLATION TEMPLATE - Replace XX with your language code (e.g., FR, ES, IT)
// ==============================================================================
//
// INSTRUCTIONS:
// 1. Replace all empty strings "" with translations in your language
// 2. Use the English text in comments as reference for what to translate
// 3. Keep translations concise due to limited screen space on watch
// 4. Avoid special characters/diacritics if your language supports it
// 5. Test on actual device to ensure text fits properly
// 6. Rename this file to: localization_XX.h (where XX is your language code)
// 7. Don't use periods within Weather/Air Quality Menu names (ie. "Temp.")
//
// IMPORTANT NOTES:
// - Keep all #define names exactly as they are
// - Only translate the text between quotes ""
// - Some abbreviations (like "AM/PM") might stay the same
// - Day/month abbreviations should be 2-4 characters max
// - Error messages should be very short (2-3 characters)
//
// ==============================================================================

// Slate watchface weather conditions
#define WF_S_WEATHER_CLEAR_SKY ""      // "Clear Sky"
#define WF_S_WEATHER_MOSTLY_CLEAR ""   // "Mostly Clear"
#define WF_S_WEATHER_PARTLY_CLOUDY ""  // "Partly Cloudy"
#define WF_S_WEATHER_OVERCAST ""       // "Overcast"
#define WF_S_WEATHER_FOG ""            // "Foggy"
#define WF_S_WEATHER_HEAVY_FOG ""      // "Heavy Fog"
#define WF_S_WEATHER_LIGHT_DRIZZLE ""  // "Light Drizzle"
#define WF_S_WEATHER_DRIZZLE ""        // "Drizzling"
#define WF_S_WEATHER_HEAVY_DRIZZLE ""  // "Heavy Drizzle"
#define WF_S_WEATHER_LIGHT_FREEZING "" // "Light Freezing"
#define WF_S_WEATHER_HEAVY_FREEZING "" // "Heavy Freezing"
#define WF_S_WEATHER_LIGHT_RAIN ""     // "Light Rain"
#define WF_S_WEATHER_RAIN ""           // "Raining"
#define WF_S_WEATHER_HEAVY_RAIN ""     // "Heavy Rain"
#define WF_S_WEATHER_FREEZING_RAIN ""  // "Freezing Rain"
#define WF_S_WEATHER_FREEZING_HARD ""  // "Freezing Hard"
#define WF_S_WEATHER_LIGHT_SNOW ""     // "Light Snow"
#define WF_S_WEATHER_SNOW ""           // "Snowing"
#define WF_S_WEATHER_HEAVY_SNOW ""     // "Heavy Snow"
#define WF_S_WEATHER_SNOW_GRAINS ""    // "Snow Grains"
#define WF_S_WEATHER_LIGHT_SHOWERS ""  // "Light Showers"
#define WF_S_WEATHER_SHOWERS ""        // "Showering"
#define WF_S_WEATHER_HEAVY_SHOWERS ""  // "Heavy Showers"
#define WF_S_WEATHER_SNOW_SHOWERS ""   // "Snow Showers"
#define WF_S_WEATHER_THUNDERSTORM ""   // "Thunderstorm"
#define WF_S_WEATHER_LIGHT_HAIL ""     // "Light Hail"
#define WF_S_WEATHER_HEAVY_HAIL ""     // "Heavy Hail"
#define WF_S_WEATHER_UNKNOWN ""        // "Unknown"

// Day names (abbreviated) - Keep these SHORT (2-4 characters max)
#define WF_S_DAY_MON "" // "MON"
#define WF_S_DAY_TUE "" // "TUE"
#define WF_S_DAY_WED "" // "WED"
#define WF_S_DAY_THU "" // "THU"
#define WF_S_DAY_FRI "" // "FRI"
#define WF_S_DAY_SAT "" // "SAT"
#define WF_S_DAY_SUN "" // "SUN"

// Error messages - Keep these VERY SHORT (MAX 3 characters for InkField)
#define WF_T_ERROR ""       // "ERR" - Taychron error
#define WF_I_ERROR ""       // "ERR" - InkField error (MAX 3 CHARACTERS!)
#define WF_I_ERROR_SHORT "" // "ER" - InkField short error

// Time format - These might stay the same in many languages
#define WF_TIME_AM "" // "AM"
#define WF_TIME_PM "" // "PM"

// Weather/Air Quality Selection items
#define WEATHER_AQ_MENU_TITLE       // "Data Type"
#define WEATHER_AQ_MENU_WEATHER     // "Weather"
#define WEATHER_AQ_MENU_AIR_QUALITY // "Air Quality"
#define WEATHER_AQ_NOT_AVAILABLE    // "Weather/AQ both not available"

// Weather submenu items
#define WEATHER_MENU_TEMPERATURE ""           // "Temperature"
#define WEATHER_MENU_PRESSURE ""              // "Pressure"
#define WEATHER_MENU_HUMIDITY ""              // "Humidity"
#define WEATHER_MENU_CONDITIONS ""            // "Weather conditions"
#define WEATHER_MENU_CLOUDINESS ""            // "Cloudiness"
#define WEATHER_MENU_WIND_SPEED ""            // "Wind speed"
#define WEATHER_MENU_WIND_GUSTS ""            // "Wind gusts"
#define WEATHER_MENU_VISIBILITY ""            // "Visibility"
#define WEATHER_MENU_PRECIPITATION ""         // "% of precipitation"
#define WEATHER_MENU_UV_INDEX ""              // "UV Index"
#define WEATHER_MENU_UV_INDEX_CLEAR_SKY ""    // "UV Clear Sky"
#define WEATHER_MENU_WET_BULB ""              // "Wet Bulb Temp"
#define WEATHER_MENU_CAPE ""                  // "CAPE"
#define WEATHER_MENU_DEW_POINT ""             // "Dew Point"
#define WEATHER_MENU_PRECIPITATION_AMOUNT ""  // "Precipitation"
#define WEATHER_MENU_SNOW_DEPTH ""            // "Snow Depth"
#define WEATHER_MENU_LIFTED_INDEX ""          // "Lifted Index"
#define WEATHER_MENU_CONVECTIVE_INHIBITION "" // "Convective Inhibition"
#define WEATHER_MENU_TERRESTRIAL_RADIATION "" // "Terrestrial Rad"
#define WEATHER_MENU_DIFFUSE_RADIATION ""     // "Diffuse Rad"
#define WEATHER_MENU_DIRECT_RADIATION ""      // "Direct Rad"
#define WEATHER_MENU_TOTAL_WATER_VAPOUR ""    // "Total Water Vapour"

// Weather menu titles and messages
#define WEATHER_SELECT_DATE ""   // "Select date"
#define WEATHER_STAT_TITLE ""    // "Weather stat"
#define WEATHER_NOT_AVAILABLE "" // "Weather not available"
#define WEATHER_DATE_WRONG ""    // "Date is wrong?"
#define WEATHER_CORRUPTED ""     // "Weather corrupted"

// Chart titles with time notation - Keep format similar with units
#define WEATHER_CHART_TEMP ""                  // "Temp / C (24h)"
#define WEATHER_CHART_PRESSURE ""              // "Pressure / hPa (24h)"
#define WEATHER_CHART_HUMIDITY ""              // "Humidity / % (24h)"
#define WEATHER_CHART_CLOUDS ""                // "Clouds / % (24h)"
#define WEATHER_CHART_WIND_SPEED ""            // "Wind speed / km/h (24h)"
#define WEATHER_CHART_WIND_GUSTS ""            // "Wind gusts / km/h (24h)"
#define WEATHER_CHART_VISIBILITY ""            // "Visib. / m (24h)"
#define WEATHER_CHART_PRECIPITATION ""         // "Prep. / % (24h)"
#define WEATHER_CONDITIONS_TITLE ""            // "Weather conditions (24h)"
#define WEATHER_CHART_UV_INDEX ""              // "UV Index (24h)"
#define WEATHER_CHART_UV_INDEX_CLEAR_SKY ""    // "UV Clear Sky (24h)"
#define WEATHER_CHART_WET_BULB ""              // "Wet Bulb / C (24h)"
#define WEATHER_CHART_CAPE ""                  // "CAPE / J/kg (24h)"
#define WEATHER_CHART_DEW_POINT ""             // "Dew Point / C (24h)"
#define WEATHER_CHART_PRECIPITATION_AMOUNT ""  // "Total Prep. / (24h)"
#define WEATHER_CHART_SNOW_DEPTH ""            // "Snow Depth / (24h)"
#define WEATHER_CHART_LIFTED_INDEX ""          // "Lifted Index / (24h)"
#define WEATHER_CHART_CONVECTIVE_INHIBITION "" // Con. Inhib. / (24hr)"
#define WEATHER_CHART_TERRESTRIAL_RADIATION "" // "Terr. Rad. / (24hr)"
#define WEATHER_CHART_DIFFUSE_RADIATION ""     // "Diff. Rad. / (24hr)"
#define WEATHER_CHART_DIRECT_RADIATION ""      // "Direct Rad. / (24hr)"
#define WEATHER_CHART_TOTAL_WATER_VAPOUR ""    // "Total Water Vap. (24hr)"

// Air quality submenu items
#define AIR_MENU_EUAQI ""                 // "European AQI"
#define AIR_MENU_USAQI ""                 // "US AQI"
#define AIR_MENU_EU_AQI_PM2_5 ""          // "EU AQI PM 2,5"
#define AIR_MENU_EU_AQI_PM10 ""           // "EU AQI PM 10"
#define AIR_MENU_EU_AQI_NO2 ""            // "EU AQI NO2"
#define AIR_MENU_EU_AQI_O3 ""             // "EU AQI O3"
#define AIR_MENU_EU_AQI_SO2 ""            // "EU AQI SO2"
#define AIR_MENU_PM2_5 ""                 // "Part Matter 2,5"
#define AIR_MENU_PM10 ""                  // "Part Matter 10"
#define AIR_MENU_CARBON_MONOXIDE ""       // "Carbon Monoxide"
#define AIR_MENU_CARBON_DIOXIDE ""        // "Carbon Dioxide"
#define AIR_MENU_NITROGEN_DIOXIDE ""      // "Nitrogen Dioxide"
#define AIR_MENU_SULPHUR_DIOXIDE ""       // "Sulphur Dioxide"
#define AIR_MENU_OZONE ""                 // "Ozone"
#define AIR_MENU_AEROSOL_OPTICAL_DEPTH "" // "Aerosol Depth"
#define AIR_MENU_DUST ""                  // "Dust"
#define AIR_MENU_METHANE ""               // "Methane"
#define AIR_MENU_FORMALDEHYDE ""          // "Formaldehyde"
#define AIR_MENU_GLYOXAL ""               // "Glyoxal"
#define AIR_MENU_SEA_SALT_AEROSOL ""      // "Sea Salt Aerosol"
#define AIR_MENU_NITROGEN_MONOXIDE ""     // "Nitrogen Monoxide"
#define AIR_MENU_PEROXYACYL_NITRATES ""   // "Peroxyacyl Nitrates"

// Air quality meni titles and messages
#define AIR_QUALITY_SELECT_DATE ""   // "Select date"
#define AIR_QUALITY_STAT_TITLE ""    // "Air quality stat"
#define AIR_QUALITY_NOT_AVAILABLE "" // "Air quality not available"
#define AIR_QUALITY_DATE_WRONG ""    // "Date is wrong?"
#define AIR_QUALITY_CORRUPTED ""     // "Air quality corrupted"

// Month names - Keep these SHORT (3-4 characters max), use format: {"JAN","FEB",...}
#define LANGUAGE_MONTH_NAMES {"", "", "", "", "", "", "", "", "", "", "", ""} // {"JAN", "FEB", "MAR", "APR", "MAY", "JUN", "JUL", "AUG", "SEP", "OCT", "NOV", "DEC"}

// Menu translations
#define MENU_MAIN ""             // "Main menu"
#define MENU_CALENDAR ""         // "Calendar"
#define MENU_SELECT_BOOK ""      // "Select book"
#define MENU_ALARMS ""           // "Alarms"
#define MENU_WEATHER ""          // "Weather"
#define MENU_SETTINGS ""         // "Settings"
#define MENU_HEART_MONITOR ""    // "Heart monitor"
#define MENU_VAULT ""            // "Vault"
#define MENU_WIFI_TOOL ""        // "Wifi tool"
#define MENU_EATING_APPLES ""    // "Eating apples"
#define MENU_SMASHING_APPLES ""  // "Smashing apples"
#define MENU_GAMES ""            // "Games"
#define MENU_CREDITS ""          // "Credits"
#define MENU_WIFI ""             // "Wifi"
#define MENU_DEBUG ""            // "Debug"
#define MENU_POWER_SETTINGS ""   // "Power settings"
#define MENU_CHANGE_WATCHFACE "" // "Change watchface"
#define MENU_TETRIS ""           // "Blockchy"
#define MENU_PONG ""             // "Pong"
#define MENU_VIDEO_PLAYER ""     // "Video player"
#define MENU_CONWAY ""           // "Conway"
#define MENU_PARTY ""            // "Party"
#define MENU_VIBRATIONS_DIS ""   // "Vibrations disabled"
#define MENU_WAKE_UP_DIS ""      // "Wake up disabled"
#define MENU_POWER ""            // "Power menu"

// ==============================================================================
// APP AND BOOK TRANSLATIONS
// ==============================================================================

// Book reader messages
#define BOOK_NO_SELECTED "" // "No book selected"

// Calendar messages
#define CALENDAR_NO_DATA "" // "No calendar data"

// ==============================================================================
// HEART MONITOR TRANSLATIONS
// ==============================================================================

#define HEART_MONITOR_TITLE ""          // "Heart rate monitor"
#define HEART_MONITOR_NOT_MEDICAL ""    // "Not for medical use"
#define HEART_MONITOR_PLACE_DEVICE ""   // "Place device on heart"
#define HEART_MONITOR_CLICK_MENU ""     // "Then click menu"
#define HEART_MONITOR_START_3S ""       // "It will start in 3s"
#define HEART_MONITOR_TAKES_15S ""      // "It takes 15s"
#define HEART_MONITOR_BREATHE_SLOWLY "" // "Try to breath slowly"
#define HEART_MONITOR_TIME_LEFT ""      // "Time left: "
#define HEART_MONITOR_BPM ""            // "bpm"

// ==============================================================================
// VIDEO PLAYER TRANSLATIONS
// ==============================================================================

#define VIDEO_PLAYER_CHOOSE "" // "Choose video"

// ==============================================================================
// WATCHFACE SELECTION TRANSLATIONS
// ==============================================================================

#define WATCHFACE_SELECT_TITLE "" // "Select watchface"

// ==============================================================================
// TETRIS GAME TRANSLATIONS
// ==============================================================================

// Tetris UI labels - Keep TETRIS_LINE_CLEARS short to avoid text cutoff
#define TETRIS_LINE_CLEARS ""   // "Lines clr:" (Keep this short!)
#define TETRIS_SCORE ""         // "Score:"
#define TETRIS_LEVEL ""         // "Level:"
#define TETRIS_CONTROLS_UP ""   // "Up:Right"
#define TETRIS_CONTROLS_DOWN "" // "Down:Left"
#define TETRIS_GAME_OVER ""     // "GAMEOVER"

// ==============================================================================
// DEBUG MENU TRANSLATIONS
// ==============================================================================

// Debug menu titles
#define DEBUG_MENU_ACC ""      // "Debug Menu: Acc"
#define DEBUG_MENU_BATTERY ""  // "Debug Menu: Batt"
#define DEBUG_MENU_CLOCK ""    // "Debug Menu: Clock"
#define DEBUG_MENU_GIT ""      // "Debug Menu: Git"
#define DEBUG_MENU_HARDWARE "" // "Debug Menu: Hardware"
#define DEBUG_MENU_WIFI ""     // "Debug Menu: Wifi"
#define DEBUG_MENU_MOTOR ""

// Debug menu items (short names for main debug menu)
#define DEBUG_ITEM_CLOCK ""        // "Clock"
#define DEBUG_ITEM_HARDWARE ""     // "Hardware"
#define DEBUG_ITEM_BATTERY ""      // "Battery"
#define DEBUG_ITEM_GIT ""          // "Git"
#define DEBUG_ITEM_ACC ""          // "Acc"
#define DEBUG_ITEM_FONT_PREVIEW "" // "Font preview"

// Accelerometer debug
#define DEBUG_ACC_DISABLED ""    // "disabled"
#define DEBUG_ACC_FAILED_INIT "" // "Failed to init Acc"
#define DEBUG_ACC_DAMAGED ""     // "Acc is damaged"
#define DEBUG_ACC_WORKING ""     // "Acc is working"
#define DEBUG_ACC_CLICK_3D ""    // "Click menu for 3D"
#define DEBUG_ACC_X ""           // "Accel X: "
#define DEBUG_ACC_Y ""           // "Accel Y: "
#define DEBUG_ACC_Z ""           // "Accel Z: "
#define DEBUG_ACC_PURE_X ""      // "Accel pure X: "
#define DEBUG_ACC_PURE_Y ""      // "Accel pure Y: "
#define DEBUG_ACC_PURE_Z ""      // "Accel pure Z: "
#define DEBUG_ACC_STEPS ""       // "Steps: "
#define DEBUG_ACC_IC ""          // "IC: "

// Battery debug
#define DEBUG_BATTERY_CURRENT_V ""  // "Current V: "
#define DEBUG_BATTERY_MINIMUM_V ""  // "Minimum V: "
#define DEBUG_BATTERY_MAXIMUM_V ""  // "Maximum V: "
#define DEBUG_BATTERY_CRITICAL_V "" // "Critical V: "
#define DEBUG_BATTERY_LEVEL ""      // "Level %: "
#define DEBUG_BATTERY_CHARGING ""   // "Charging: "
#define DEBUG_BATTERY_FULLY ""      // "Fully: "

// Clock debug
#define DEBUG_CLOCK_DRIFT_SYNCS ""   // "Drift between synces:"
#define DEBUG_CLOCK_NOT_AVAILABLE "" // "Not available"
#define DEBUG_CLOCK_LAST_SYNC ""     // "Last sync:"
#define DEBUG_CLOCK_PREVIOUS_SYNC "" // "Previous sync:"
#define DEBUG_CLOCK_LAST_CHARGE ""   // "Last charge:"

// Git debug
#define DEBUG_GIT_COMMIT_HASH "" // "Commit hash:"
#define DEBUG_GIT_BRANCH ""      // "Branch:"
#define DEBUG_GIT_BUILD_TIME ""  // "Build time:"

// Hardware debug
#define DEBUG_HW_CHIP_MODEL "" // "Chip Model:"
#define DEBUG_HW_RTC_TYPE ""   // "RTC type: "
#define DEBUG_HW_USED_HEAP ""  // "Used Heap KB: "
#define DEBUG_HW_CPU_TEMP ""   // "CPU temp: "
#define DEBUG_HW_INIT_TEMP ""  // "Init temp: "

// RTC Types - Keep DEBUG_RTC_INTERNAL short to avoid cutoff
#define DEBUG_RTC_UNKNOWN ""  // "Unknown"
#define DEBUG_RTC_DS3231 ""   // "DS3231"
#define DEBUG_RTC_PCF8563 ""  // "PCF8563"
#define DEBUG_RTC_INTERNAL "" // "INT" (Keep this short! e.g. "INT", "VN", "WEWN")
#define DEBUG_RTC_INVALID ""  // "Invalid Type"

// WiFi debug
#define DEBUG_WIFI_MAC_ADDRESS "" // "MAC address:"
#define DEBUG_WIFI_STATUS ""      // "Wifi status: "
#define DEBUG_WIFI_IP ""          // "IP: "
#define DEBUG_WIFI_SSID ""        // "SSID: "
#define DEBUG_WIFI_SIGNAL ""      // "Wifi signal: "
#define DEBUG_WIFI_CONNECTING ""  // "Connecting: "
#define DEBUG_WIFI_ON ""          // "ON"
#define DEBUG_WIFI_OFF ""         // "OFF"

// Common debug terms
#define DEBUG_COMMON_PERCENT "" // "%"
#define DEBUG_COMMON_COLON ""   // ": "

// ==============================================================================
// GAME AND APP TRANSLATIONS
// ==============================================================================

// Pong game
#define PONG_YOU_LOST "" // "You lost!"

// Party app
#define PARTY_MESSAGE "" // "Party party"

// Vault app
#define VAULT_KEY_INCORRECT "" // "Key is incorrect"
#define VAULT_EMPTY ""         // "Vault is empty?"

// ==============================================================================
// ALARM TRANSLATIONS
// ==============================================================================

// Alarm menu titles
#define ALARM_MENU_EDIT_DAYS ""        // "Edit days"
#define ALARM_MENU_EDIT_ALARM ""       // "Edit alarm"
#define ALARM_MENU_ALARMS ""           // "Alarms"
#define ALARM_MENU_BROWSE_ALARMS ""    // "Browse alarms"
#define ALARM_MENU_QUICK_ALARMS_SET "" // "Quick alarms set"
#define ALARM_MENU_QUICK_ALARM ""      // "Quick alarm"
#define ALARM_MENU_POMODORO ""         // "Pomodoro"

// Alarm status and settings
#define ALARM_STATUS_ENABLED ""       // "Enabled"
#define ALARM_STATUS_DISABLED ""      // "Disabled"
#define ALARM_LABEL_TIME ""           // "Time: "
#define ALARM_LABEL_ONE_TIME_ALARM "" // "One time alarm"
#define ALARM_LABEL_DAYS ""           // "Days: "
#define ALARM_LABEL_REQUIRE_WIFI ""   // "Require wifi"

// Quick alarm messages
#define ALARM_QUICK_ALERT_TITLE ""   // "Quick Alarm alert"
#define ALARM_QUICK_ALARM_SET_FOR "" // "Alarm set for:"
#define ALARM_QUICK_WILL_RING_AT ""  // "Will ring at:"

// Day names (full) - Full day names for alarm settings
#define ALARM_DAY_MONDAY ""    // "Monday"
#define ALARM_DAY_TUESDAY ""   // "Tuesday"
#define ALARM_DAY_WEDNESDAY "" // "Wednesday"
#define ALARM_DAY_THURSDAY ""  // "Thursday"
#define ALARM_DAY_FRIDAY ""    // "Friday"
#define ALARM_DAY_SATURDAY ""  // "Saturday"
#define ALARM_DAY_SUNDAY ""    // "Sunday"

// Alarm info strings
#define ALARM_INFO_ALL_DAYS "" // "All days"
#define ALARM_INFO_NEVER ""    // "No days"
#define ALARM_INFO_ONCE ""     // ", once"
#define ALARM_INFO_WIFI ""     // ", wifi"
#define ALARM_INFO_QUICK ""    // ", quick"
#define ALARM_INFO_POMODORO "" // ", pomodoro"

// Pomodoro strings
#define POMODORO_STATUS ""         // "Status: "
#define POMODORO_RUNNING ""        // "running, "
#define POMODORO_WORKING ""        // "working, "
#define POMODORO_PAUSE ""          // "pause, "
#define POMODORO_ITER ""           // "iter: "
#define POMODORO_DISABLED ""       // "disabled"
#define POMODORO_RESET_TURN_OFF "" // "Reset & Turn off"
#define POMODORO_START ""          // "Start"

// Set Clock GUI
#define SETCLOCK_SET_TIME ""            // "Set time"
#define SETCLOCK_SET_DATE ""            // "Set date"
#define SETCLOCK_TIMEZONE_MENU ""       // "Timezone menu"
#define SETCLOCK_CLOCK_SETTINGS ""      // "Clock settings"
#define SETCLOCK_CURRENT_TIMEZONE ""    // "Current timezone: "
#define SETCLOCK_CLEAR_TIMEZONE ""      // "Clear timezone"
#define SETCLOCK_SET_TIMEZONE ""        // "Set timezone"
#define SETCLOCK_TIMEZONE_SETTINGS ""   // "Timezone settings"
#define SETCLOCK_TIMEZONE_WARNING ""    // "This menu ignores daylight/standard time and will overwrite your timezone, which may already have it.\nClick back button to confirm"
#define SETCLOCK_WARNING_TITLE ""       // "Warning"
#define SETCLOCK_SELECT_TIMEZONE ""     // "Select Timezone"

// Time unit translations (for automatic formatting)
#define TIME_UNIT_MINUTE ""  // "minute"
#define TIME_UNIT_MINUTES "" // "minutes"
#define TIME_UNIT_HOUR ""    // "hour"
#define TIME_UNIT_HOURS ""   // "hours"
#define TIME_UNIT_DAY ""     // "day"
#define TIME_UNIT_DAYS ""    // "days"
#define TIME_UNIT_AND ""     // "and"


// ==============================================================================
// TRANSLATION CHECKLIST
// ==============================================================================
//
// Before submitting your translation, please verify:
// □ All empty strings "" have been replaced with translations
// □ Day abbreviations are 2-4 characters max
// □ Month abbreviations are 3-4 characters max
// □ Error messages are 2-3 characters max
// □ WF_I_ERROR is MAX 3 characters (InkField limitation)
// □ TETRIS_LINE_CLEARS is short enough to avoid cutoff
// □ DEBUG_RTC_INTERNAL is abbreviated (e.g., "INT", "VN", "WEWN")
// □ All translations fit on watch screen (test if possible)
// □ No special characters that might cause display issues
// □ LANGUAGE_MONTH_NAMES array has exactly 12 entries
// □ File renamed to localization_XX.h (where XX is your language code)
//
// ==============================================================================