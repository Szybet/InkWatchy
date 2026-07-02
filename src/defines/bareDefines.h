#pragma once

#define PARTIAL_UPDATE true
#define FULL_UPDATE false

#define WATCHY_1 1
#define WATCHY_1_5 15
#define WATCHY_2 2
#define WATCHY_3 3
#define YATCHY 4

#define EXTERNAL_RTC 1
#define INTERNAL_RTC 2

#define BTN_REMAP_VAL_MENU 1
#define BTN_REMAP_VAL_BACK 2
#define BTN_REMAP_VAL_UP 3
#define BTN_REMAP_VAL_DOWN 4

// Voltage reading average
#define VOLTAGE_AVG_COUNT 8
#define VOLTAGE_AVG_DELAY 0

// Other
#define FULL_MODE_AFTER_S 10 // Entering full mode after x seconds.

// Task priorities - max is 24
#define MAIN_LOOP_PRIORITY 20
#define WIFI_PRIORITY_REGULAR 10
#define BUTTONS_PRIORITY 12
#define WIFI_PRIORITY_PERSISTENT 4
#define SCOM_PRIORITY 3
#define MOTOR_PRIORITY 22
#define WATCHDOG_PRIORITY 1
#define RGB_PRIORITY 22

// Resources
#define RESOURCES_NAME_LENGTH 50 // Max

// Config
#define CONF_BOOK_CURRENT_PAGE "current_page_"
#define CONF_BOOK_CURRENT_BOOK "current_book"
#define CONF_DISABLE_ALL_VIBRATION "disable_all_vibrations"
#define CONF_DISABLE_WAKE_UP "disable_wake_up"
#define CONF_BITCOIN "bitcoin"
#define CONF_DRIFT "drift"
#define CONF_DRIFT_FAST "drift_fast"
#define CONF_UNIX_LAST_SYNC "last_sync_unix"
#define CONF_UNIX_PREVIOUS_SYNC "previous_sync_unix"
#define CONF_SECONDS_DRIFT "drift_sync_seconds"
#define CONF_UNIX_LAST_CHARGE "last_charge_unix"
#define CONF_RTC_BACKUP "rtc_memory_backup"

#define WEATHER_HOURLY_DIR "/weather/hourly"
#define WEATHER_DIR "/weather"
#define AIR_QUALITY_HOURLY_DIR "/air_quality/hourly"
#define AIR_QUALITY_DIR "/air_quality"

#define IMAGE_MODULE_PATH "watchfaceImages/"
#define PRECISE_STEP_COUNTING_DIR "/other/steps/"
#define NOTES_DIR "/notes"

#define LP_CORE_FILE_DEFAULT "default"
#define LP_CORE_FILE_TERRAIN "terrain"
