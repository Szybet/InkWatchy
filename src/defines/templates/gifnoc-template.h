#pragma once

// Watchfaces!
// Keep at least one enabled. If you bother me with a problem and the problem will be that you disabled all watchfaces, then I will credit you when implementing a future counter measure.
#define WATCHFACE_12H 1 // Show time in 12 hour format. Supported watchfaces: Taychron
#define WATCHFACE_INKFIELD_SZYBET 1
#define WATCHFACE_SHADES_SZYBET 1
#define WATCHFACE_ANALOG_SHARP_SZYBET 1
#define WATCHFACE_TAYCHRON 1


// Basics
#define DEBUG_MENUS 1                 // Includes debug menus for various things
// Those vibrations settings are dependent on motor task priority
#define VIBRATION_BUTTON_TIME_OVERWRITE 0 // This needs to be to 1 if you want the values below to be listened to. Otherwise it's device dependent in condition.h
// Those are example values
#define VIBRATION_BUTTON_TIME 90      // Time in ms to the motor to vibrate after clicking a button. 0 means none
#define VIBRATION_BUTTON_LONG_TIME 60 // This is just an addition to VIBRATION_BUTTON_TIME
#define VIBRATION_ACTION_TIME 200     // Time in ms to the motor to vibrate when the UI receives an action
#define VIBRATION_POWER 170           // From 1 to 255, PWM duty cycle. Too low and it can not even vibrate a little
#define STEPS_GOAL 7000

// Timezone! So:
// - You don't set anything, it will try to ques based on IP, it can fail sometimes
// - You set olson, which still can be incorrect (probably won't). If this is set, there will be no quesing
// - You set posix, it will overwrite everything above, no quesing or anything
// Set this as a string of olson timezone time
// List here: https://en.wikipedia.org/wiki/List_of_tz_database_time_zones
// The "TZ" identifier in the table
#define TIMEZONE_OLSON "" // Example value: "Europe/Warsaw"
// Here is an example table:
// https://support.cyberdata.net/portal/en/kb/articles/010d63c0cfce3676151e1f2d5442e311
// Better one probably: https://github.com/yuan910715/Esp8266_Wifi_Matrix_Clock/blob/master/posix.md
// Better one probably: https://github.com/yuan910715/Esp8266_Wifi_Matrix_Clock/blob/master/posix.md
// From the Posix timezone string table column
// This function overwrites the TIMEZONE_OLSON variable if it's set
#define TIMEZONE_POSIX "" // Example value for poland: "CET-1CEST,M3.5.0,M10.5.0/3"

#define MENU_LINES false             // Option to show lines between buttons in menus
#define BUTTON_LONG_PRESS_MS 500     // Duration until long press registers in miliseconds
#define FULL_DISPLAY_UPDATE_QUEUE 60 // Make a full display update after x of partial ones
#define LONG_BACK_FULL_REFRESH 1     // Make a full refresh at long back button clicked
#define WATCHDOG_TASK 1              // Wastes resources but tries to detect hangups and you can reset the watch with clicking all buttons too

// Drift & NTP & Syncing
// For now, SYNC_WIFI is not advised to be used, thank you
#define SYNC_WIFI 0                        // Autimatically sync wifi - only if it's being charger and after the delay below
#define SYNC_WIFI_SINCE_SUCC 72000         // 20h
#define SYNC_WIFI_SINCE_FAIL 600           // 10m
#define MANUAL_NTP_OFFSET 0                // In seconds, When your ntp server is wrong
#define SYNC_NTP_ON_CHARGING_DELAY 90000   // Sync NTP when charging every, in Ms. Default 1.5 minute
#define VALID_PREVIOUS_SYNC_DELAY 300      // Valid minimum delay to calculate drift, below that it will be ignored. Keep in mind to keep it higher then the delay between SYNC_WIFI_SINCE_FAIL and SYNC_NTP_ON_CHARGING_DELAY
#define WIFI_CONNECTION_TRIES 3            // Regular sync, number of tries
#define WIFI_CONNECTION_TRIES_PERSISTENT 1 // Persistent sync (SYNC_WIFI), number of tries
#define WIFI_SYNC_TIME 10000                // Ms, time for waiting to connect to wifi
#define WIFI_ERROR_TIME 3000               // Ms, time for waiting when wifi failed before next try
#define TIME_DRIFT_CORRECTION 0            // The RTC may drift, this should repair it. It basically on first ntp sync starts the measurment and on the second it ends it. I suggest disabling SYNC_WIFI because if it's triggered, it will cancel the drift calculation because its a persistent sync, it happens every few minutes. If you have a watchy V2, you should first try FORCE_INTERNAL_RTC instead of this option.
#define TIME_DRIFT_MINIMUM_TIME 24         // Minimum amount of hours a drift calculation can go on, because measuring drift between 5 minutes will help you experience the back to the future movie. It's suggested to be minimum 24 hours.
#define FORCE_INTERNAL_RTC 0               // If your device has an external RTC but it sucks, set this to 1
#define FORCED_INTERNAL_RTC_QUARTZ 0       // If you forced the internal RTC and if you have an external quartz crystal, set this to 1
#define AVOID_SLEEPING_ON_FULL_MINUTE 4    // This value shouldn't be really touched, it checks if in x seconds a full minute passess, if yes, it waits to x / 2 seconds
#define CONNECT_TO_HIDDEN_NETWORKS 0       // This makes the wifi manager connect to hidden networks too. For some reason it takes more time and power to do so, that's why it's not on default, otherwise it should work
#define NTP_SERVER_URL "ntp.ubuntu.com" // Change it to something else if it doesn't work as a first step to troubleshoot. pool.ntp.org has a rate limiter apparently

// Weather
#define WEATHER_INFO 1                     // Enable weather
#define WEATHER_TRIES 3                    // Ammount of times to try to sync weather
#define WEATHER_WATCHFACE_HOUR_OFFSET 1
#define PRESSURE_HPA_MAX 1100 // 1013 is 50%
#define PRESSURE_HPA_MIN 850
#define VISIBILITY_M_MAX 150
#define VISIBILITY_M_MIN 0
#define WIND_SPEED_MAX 30 // Km/h
#define WIND_SPEED_MIN 0
// Moon
#define MOON_MIN_RADIUS 57.0
#define MOON_MAX_RADIUS 63.6

// Book things
#define BOOK 0
#define BOOK_ADD_SLEEP_DELAY_MS 60000
#define BOOK_AX_X_PAGE_CHANGE_VALUE_UP 300
#define BOOK_AX_X_PAGE_CHANGE_VALUE_DOWN 250
#define BOOK_AX_Z_BACK_VALUE 500
#define BOOK_AX_Y_BACK_VALUE 500
#define BOOK_AX_X_TOLERANCE 150
#define BOOK_ON_EXIT_GO_PAGE_BACK 1
#define BOOK_AXC_LINE 1
#define BOOK_AXC_LINE_WIDTH 3
#define BOOK_AXC_DIFFERENCE_CHANGE 15
#define BOOK_CHARS_PER_PAGE 200 // This is dependent on the font. It must be Mono
#define BOOK_FONT getFont("UbuntuMono-Regular10")

// Conway, It will drain your battery a lot
#define CONWAY 1
#define CONWAY_CPU_SPEED 0    // When calculating conway, increase cpu speed. This obviously increases battery usage but makes the watch faster with the conway module AND is a "maybe" fix for some resets related to watchdog
#define CONWAY_MODULE_DEBUG 0 // speed up the module. Don't

// Calendar
#define CALENDAR 0
// Shorter names needed
#define CALENDAR_SPLIT_DESCRIPTION 1
#define CALENDAR_SPLIT_DESCRIPTION_STRING "\\n"
#define CALENDAR_SPLIT_DESCRIPTION_ARRAY_SIZE 2

#define VIDEO_PLAYER 0

// Watchface modules
#define MODULE_PERSISTENT 1       // Makes modules, like bitcoin not dissmissable, they will always appear and be choosen. The image module always will be shown, if you disable it will simply be an empty space
#define MODULE_UPDATE_LIMIT_S 600 // Every x seconds, update the modules. Not every minute, like it was for now. Change it to 60 for every minute. Change it to 0 to completly disable modules updating automatically
#define WIFI_MODULE 1
#define BITCOIN_MODULE 1   // Remember to define the api key for it in confidential.h
#define SMALL_BTC_MODULE 1 // At default, show the small btc module mode, if 0 show the bigger one
#define BITCOIN_SYNC_TRIES 3
#define CONWAY_MODULE 1                  // Conway module. to make it enabled CONWAY itself needs to be enabled
#define CONWAY_MODULE_GRID_PERCANTAGE 50 // Percentage of initial grid filling

#define BOOK_MODULE 1
#define BOOK_MODULE_CHARS_PER_PAGE 140
#define MODULES_OPERATING_FAST 1      // Great name for a feature... Well basically when you operate modules it won't wait for more operations, it goes to sleep. This makes it wait the value below:
#define MODULES_WAITING_DIVISION 0.75 // Divise SLEEP_EVERY_MS through that value

#define IMAGE_MODULE 1
#define IMG_MODULE_CHANGE_EVERY_HOUR 1 // Change the image every hour

#define API_MODULE 0 // With it, consider setting BUTTON_CPU_SPEED to something faster to make it faster

#define EVENT_MODULE 0 // Needs the CALENDAR feature enabled, otherwise will error out

// Power savings
#define NIGHT_SLEEP_FOR_M 45 // If it's 1 it doesn't apply, In minutes
#define NIGHT_SLEEP_AFTER_HOUR 23
#define NIGHT_SLEEP_BEFORE_HOUR 5
#define SLEEP_EVERY_MS 10000              // Goes to sleep timer, is resetted by button presses and other things
#define POWER_SAVING_AFTER 40             // Turn on power saving features after a certain battery percantage.
#define POWER_SAVING_OFF_AFTER 20         // Difference in POWER_SAVING_AFTER after which it will be turned off. Make sure POWER_SAVING_AFTER + POWER_SAVING_OFF_AFTER is not above 100
#define LOOP_NO_SCREEN_WRITE_DELAY_MS 112 // Go to "sleep" for x ms if the device is woken up (in a menu for example) and the screen didn't update. This value is fine tuned, but if you want a faster menu, feel free to make it smaller. 225 is good for me, but a very small percent of people say it's too much, let's do 112 and be happy. Power saving with this option will do really nothing, barely anything
#define HARDWARE_POWER_SAVINGS 1          // Like wifi modem in power saving mode
#define CPU_SPEED minimalSpeed            // Possible values: minimalSpeed, normalSpeed, maxSpeed. Obviously higher speeds decrease battery life - but it's only when you interact with the watch. This is mostly for the people who "ugh this watch is slow!". Well first, it's a watch, it should show time, any other features are battery wasters in some way and secondly you should first remove some features you don't use, most noticibly unused watchface moduls. In debug mode, this feature is overritten by DEBUG_CPU_SPEED when DEBUG
#define BUTTON_CPU_SPEED normalSpeed      // Cpu speed when it has woken up by a button
#define SOFT_START_THINGS 1               // Slowly starts things, like wifi to prevent brownout reset.
#define SOFT_START_DELAY_MS 4500
#define FORCE_DISABLE_ACC 0 // 1 Means ACC is disabled

// Vault
#define VAULT 1

// Pong
#define PONG 1

// Tetris
#define TETRIS 1

// Credits page
#define CREDITS 1 // Feel free to disable it for your own usage :D

// Advanced
#define BUTTON_TASK_DELAY 60 // In ms, lower means faster button detection but more cpu usage
#define TIME_FOR_WATCHFACE_TO_SHOW_MS 2000
#define ADD_BUTTON_DELAY 1.5
#define SMALL_BUTTON_DELAY_MS 15
#define BAT_MINIMAL_DIFFERENCE 0.02
#define AVOID_SLEEP_USB_JTAG 1 // If your device has USB JTAG (Yatchy, I don't have a Watchy v3 to test) it will avoid going to sleep if it detects it is being used. Useful when you want to reprogram the watch but it goes to sleep

// Wifi Quick settings
#define WIFI_QUICK_CHECK_MS 30
#define WIFI_QUICK_MAX_MS 1250 // And half of it for tcp

// Temperature readings
// This is highly experimental, dont enable this if you dont have a reason for it, possible reasons (but you still should read the code to understand if it will help you)
// - screen goes bad on bad temperatures
// - you live in a volcano and need to turn the esp32 off if its above a certain temperature, or below
// - you stream youtube and the cpu gets too hot
#define TEMP_CHECKS 0
// Those variables won't work for you, you need to edit them yourself after you see the temperature logs
// Also everything in Celsius, americans need to metric (Or add support for burgers per inch for inkwatchy)
// just joking :D
#define TEMP_REAL_OFFSET 0 // On first boot a temperature will be taken, this will be added to it and that's the main temperature
#define TEMP_HIGHER_LIMIT_RELATIVE 0
#define TEMP_LOWER_LIMIT_RELATIVE 0
#define TEMP_REBOOT_LIMIT_RELATIVE 0
#define TEMP_MAX_SCREEN_FIXES 5

// Wifi country
// If your wifi connects to ghost networks, freaks out in general, try setting the wifi country for your country
// https://docs.espressif.com/projects/esp-idf/en/stable/esp32/api-reference/network/esp_wifi.html#_CPPv425esp_wifi_set_country_codePKcb
#define WIFI_COUNTRY_FIX 0 // Enable this to 1 to enable the fix
/*
Supported country codes are "01"(world safe mode) "AT","AU","BE","BG","BR", "CA","CH","CN","CY","CZ","DE","DK","EE","ES","FI","FR","GB","GR","HK","HR","HU", "IE","IN","IS","IT","JP","KR","LI","LT","LU","LV","MT","MX","NL","NO","NZ","PL","PT", "RO","SE","SI","SK","TW","US"
*/
#define WIFI_COUNTRY_CODE ""
#define WIFI_COUNTRY_FORCE false // This should be false, you can set it to true to check if something starts working

// Debugging help
#define DEBUG 0
#define DEBUG_CPU_SPEED normalSpeed // Possible values: minimalSpeed, normalSpeed, maxSpeed
#define EINK_COUNTER 0              // Shows a counter somewhere on screen
#define DUMP_INIT_DEBUG 0           // If debug, shows at init init information
#define DUMP_LOOP_DEBUG 0           // If debug, at every loop iteration dump hardware values that can change
#define DUMP_LOOP_SOFTWARE_DEBUG 0  // If debug, at every loop iteration dump software values that can change - no sense in using it with DUMP_LOOP_DEBUG
#define SCREEN_SLEEP_INFO 0         // Shows information on screen when the device goes to screen
#define DRAW_DEBUG_RECT 0           // Shows rectangles where custom functions write bitmaps
#define DUMP_LOOP_DELAY 1000        // delay in ms of dumping loop data
#define SPEED_THROUGH_TIME 0        // Speeds up time for watchface programming
#define NO_SYNC 0                   // If debug and this is both true, it will not try to sync up automatically
#define VOLTAGE_PRINT_ON 0          // Prints voltage on the screen, really fast
#define DISABLE_SLEEP 0             // Disable sleep, so it will never go to sleep. Good for SCOM_TASK. It disables the sleep logic fully
#define DISABLE_SLEEP_PARTIAL 0     // Disable sleep, but remain the logic of it so it will return to the watchface for example
#define PUT_LOGS_TO_SERIAL 1        // Puts logs to serial. Turn off if you want debug on the go
#define PUT_LOGS_TO_FS 0            // Puts logs into littlefs
#define WAIT_FOR_MONITOR 0          // If debug is enabled, waits for monitor in setup for x ms
#define WAIT_FOR_INPUT 0            // If debug is enabled, waits for monitor until you click a button while serial monitor is opened
#define WAIT_FOR_INPUT_MOTOR 0      // Vibrate lightly in WAIT_FOR_INPUT
// Ah... too much allocated memory... 10 hours of my life...
#define LOG_SERIAL_BUFFER_SIZE 1000
#define LOG_FILE_BUFFER_SIZE 3000
// Max file size for littlefs: 2Gb
// https://github.com/littlefs-project/littlefs/issues/738
#define MAX_LOG_FILE_SIZE_BYTES 100000 // 100 Kb - this means logs will maximally be stored x2 by that, because 2 files switching by each other to preserve 100 Kb of last logs, 400000 is the max in my opinion
#define STOP_ON_RESET 1                // Stop the device until the reset is cleared, it doesn't do that if it's sure that it was a forced reset (esptool one)
#define SERIAL_LOG_DELAY 0             // If 1, use the delay below to ensure good looking logs
#define SERIAL_LOG_DELAY_MS 23
#define SERIAL_BAUDRATE 115200
#define MINIMAL_LOGS 1                 // Don't put full file paths in logs. Scom task requires this to be 0
#define SCOM_TASK 0                    // Edit this to enable scom task. Requires DEBUG to be enabled too to be applied. It forces DEBUG_CPU_SPEED to maxSpeed
#define FONT_PREVIEW_MENU 0            // Edit this to enable font preview menu. Requires DEBUG and DEBUG_MENUS to be enabled too
#define NO_CHARGING 0                  // Disable detection of charging, only in debug. Also locks in the voltage (for lp core to check if the time is changed bla bla)
#define RESET_RTC_ON_BOOT 0            // Experimental.
#define BATTERY_TIME_DROP 0            // If enabled, if battery drops below BATTERY_TIME_DROP_VOLTAGE. Doesn't need DEBUG
#define BATTERY_TIME_DROP_VOLTAGE 3.50 // Needs to be float
#define LP_CORE_TEST_RUN 0             // Yatchy only, why would anyone use it aside from me?
#define LP_CORE_SERIOUS_TEST 0
#define DISABLE_WAKEUP_INTERRUPTS 0            // As in buttons (or yatchy gpio expander)
#define MCP_GPIO_EXPANDER_DISABLE 0            // Bad idea, may increase power consumption
#define MCP_GPIO_EXPANDER_DISABLE_INTERRUPTS 0 // Yatchy with no battery, huh

// Voltage reading average
#define VOLTAGE_AVG_COUNT 20
#define VOLTAGE_AVG_DELAY 1

// Battery
#define BATTERY_MIN_VOLTAGE 3.3
#define BATTERY_CRIT_VOLTAGE 3.15
#define BATTERY_CHARGE_VOLTAGE 4.14
#define BATTERY_MAX_VOLTAGE 4.18 // For calculating percentages, upper limit
// #define BAD_BATTERY_MAX_VOLTAGE 4.00
// #define BAD_BATTERY_CHARGE_VOLTAGE 4.05
#define BATTERY_CHARGE_DETECTION_DIFFERENCE 0.10 // The minimum difference to detect that the battery is charging

// Other
#define MAX_MENU_ITEMS 35

// Other other, resources thing
#define RESOURCES_NAME_LENGTH 50
#define IMG_COUNT 30
#define FONT_COUNT 30
// So it's 50*30 bytes

// Task memory size
#define TASK_STACK_DUMP_BATTERY 2000
#define TASK_STACK_BUTTON 4700
#define TASK_STACK_VIBRATION 3500
#define TASK_STACK_WIFI 60000
#define TASK_STACK_SCOM 10000
#define TASK_STACK_WATCHDOG 3500
#define TASK_STACK_RGB 2000

// Priorities - max is 24
#define MAIN_LOOP_PRIORITY 20
#define WIFI_PRIORITY_REGULAR 10
#define BUTTONS_PRIORITY 12
#define WIFI_PRIORITY_PERSISTENT 4
#define SCOM_PRIORITY 3
#define MOTOR_PRIORITY 22
#define WATCHDOG_PRIORITY 1
#define RGB_PRIORITY 22

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

#define WEATHER_HOURLY_DIR "/weather/hourly"
#define WEATHER_DIR "/weather"

#define IMAGE_MODULE_PATH "watchfaceImages/"

