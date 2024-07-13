#ifndef CONFIG_H
#define CONFIG_H

// Basics
#define GSR_MINIMUM_BATTERY_VOLTAGE 0 // Watchy_GSR uses higher, RTC something something based minimum voltage levels. Change this to 1 if you want some more restrictive battery measurments
#define DEBUG_MENUS 1                 // Includes debug menus for various things
// Those vibrations settings are dependent on motor task priority
#define VIBRATION_BUTTON_TIME 35 // Time in ms to the motor to vibrate after clicking a button. 0 means none
#define VIBRATION_ACTION_TIME 60 // Time in ms to the motor to vibrate when the UI receives an action
// maybe a TLDR: if you don't feel any vibrations, turn the diviner smaller and in reverse. It kind of controls the power of the vibrations while the TIME variables the time its vibrating
#define VIBRATION_DIVINE 7 // Divider for the time above for it to not vibrate and not just run. Imits PWM... hard to explain, just look at vibrateMotorTaskFun in hardware.cpp

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
// From the Posix timezone string table column
// This function overwrites the TIMEZONE_OLSON variable if it's set
#define TIMEZONE_POSIX "" // Example value: "CEST-1CET,M3.2.0/2:00:00,M11.1.0/2:00:00"

#define MENU_LINES false             // Option to show lines between buttons in menus
#define BUTTON_LONG_PRESS_MS 500     // Duration until long press registers in miliseconds
#define FULL_DISPLAY_UPDATE_QUEUE 60 // Make a full display update after x of partial ones
#define LONG_BACK_FULL_REFRESH 1     // Make a full refresh at long back button clicked
#define WATCHDOG_TASK 1              // Wastes resources but tries to detect hangups and you can reset the watch with clicking all buttons too

// Drift & NTP & Syncing
#define SYNC_ON_CHARGING 1                 // Keep wifi connected and sync NTP every few minutes because the RTC fucks up
#define SYNC_WIFI 1                        // Sync wifi - only if it's being charger and after the delay below
#define SYNC_WIFI_SINCE_SUCC 72000         // 20h
#define SYNC_WIFI_SINCE_FAIL 600           // 10m
#define SYNC_NTP_ON_CHARGING_DELAY 90000   // Sync NTP when charging every, in Ms. Default 1.5 minute
#define VALID_PREVIOUS_SYNC_DELAY 300      // Valid minimum delay to calculate drift, below that it will be ignored. Keep in mind to keep it higher then the delay between SYNC_WIFI_SINCE_FAIL and SYNC_NTP_ON_CHARGING_DELAY
#define WIFI_CONNECTION_TRIES 3            // Regular sync, number of tries
#define WIFI_CONNECTION_TRIES_PERSISTENT 1 // Persistent sync (SYNC_ON_CHARGING), number of tries
#define WIFI_MULTI_SYNC_TIME 20000         // Ms, time for waiting to connect to wifi
#define WIFI_MULTI_ERROR_TIME 10000        // Time in MS to try to connect next time (WIFI_CONNECTION_TRIES)
#define TIME_DRIFT_CORRECTION 1            // The RTC may drift, this should repair it
#define FORCE_INTERNAL_RTC 0               // If your device has an external RTC but it sucks, set this to 1
#define FORCED_INTERNAL_RTC_QUARTZ 0       // If you forced the internal RTC and if you have an external quartz crystal, set this to 1
#define AVOID_SLEEPING_ON_FULL_MINUTE 4    // This value shouldn't be really touched, it checks if in x seconds a full minute passess, if yes, it waits to x / 2 seconds

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

// Conway
#define CONWAY 1
#define CONWAY_CPU_SPEED 0    // When calculating conway, increase cpu speed. This obviously increases battery usage but makes the watch faster with the conway module AND is a "maybe" fix for some resets related to watchdog
#define CONWAY_MODULE_DEBUG 0 // speed up the module. Don't

// Watchface modules
#define MODULE_PERSISTENT 1         // Makes modules, like bitcoin not dissmissable, they will always appear and be choosen
#define UPDATE_MODULE_IF_CHARGING 0 // Update modules very fast if its charging, not only every minute
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

// Power savings
#define NIGHT_SLEEP_FOR_M 45 // If it's 1 it doesn't apply, In minutes
#define NIGHT_SLEEP_AFTER_HOUR 23
#define NIGHT_SLEEP_BEFORE_HOUR 5
#define SLEEP_EVERY_MS 10000              // Goes to sleep timer, is resetted by button presses and other things
#define POWER_SAVING_AFTER 60             // Turn on power saving features after a certain battery percantage.
#define POWER_SAVING_OFF_AFTER 20         // Difference in POWER_SAVING_AFTER after which it will be turned off. Make sure POWER_SAVING_AFTER + POWER_SAVING_OFF_AFTER is not above 100
#define LOOP_NO_SCREEN_WRITE_DELAY_MS 225 // Go to "sleep" for x ms if the device is woken up (in a menu for example) and the screen didn't update. This value is fine tuned, but if you want a faster menu, feel free to make it smaller
#define HARDWARE_POWER_SAVINGS 1          // Like wifi modem in power saving mode
#define CPU_SPEED minimalSpeed            // Possible values: minimalSpeed, normalSpeed, maxSpeed. Obviously higher speeds decrease battery life - but it's only when you interact with the watch. This is mostly for the people who "ugh this watch is slow!". Well first, it's a watch, it should show time, any other features are battery wasters in some way and secondly you should first remove some features you don't use, most noticibly unused watchface moduls. In debug mode, this feature is overritten by DEBUG_CPU_SPEED when DEBUG
#define SOFT_START_THINGS 1               // Slowly starts things, like wifi to prevent brownout reset.
#define SOFT_START_DELAY_MS 2500

// For now those features are:
// DISABLE_BUTTON_VIBRATION
// DISABLE_WAKE_UP - don't wake up - at all
// Those are also editable via vns storage

// Vault
#define VAULT 0

// Now external tools, so they are configured in platformio.ini
// Wifi tool
// In platformio!

// Apple joke
#define APPLE_JOKE_DEVICE_NAME "Airpods"
#define APPLE_JOKE_DELAY 800 // The default one, it's changeable via up/down buttons

// Advanced
#define BUTTON_TASK_DELAY 60 // In ms, lower means faster button detection but more cpu usage
#define TIME_FOR_WATCHFACE_TO_SHOW_MS 2000
#define ADD_BUTTON_DELAY 1.5
#define SMALL_BUTTON_DELAY_MS 15
#define BAT_MINIMAL_DIFFERENCE 0.02

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
#define DISABLE_SLEEP 0             // Disable sleep, so it will never go to sleep. Good for SCOM_TASK
#define PUT_LOGS_TO_SERIAL 1        // Puts logs to serial. Turn off if you want debug on the go
#define PUT_LOGS_TO_FS 1            // Puts logs into littlefs
#define WAIT_FOR_MONITOR 0          // If debug is enabled, waits for monitor in setup for x ms
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
#define MINIMAL_LOGS 1      // Don't put full file paths in logs
#define SCOM_TASK 0         // Edit this to enable scom task. Requires DEBUG to be enabled too to be applied
#define FONT_PREVIEW_MENU 0 // Edit this to enable font preview menu. Requires DEBUG and DEBUG_MENUS to be enabled too
#define NO_CHARGING 1       // Disable detection of charging, only in debug

// Voltage reading average
#define VOLTAGE_AVG_COUNT 20
#define VOLTAGE_AVG_DELAY 5

// Battery
#define BATTERY_MIN_VOLTAGE 3.3
#define BATTERY_CRIT_VOLTAGE 3.15
#define BATTERY_CHARGE_VOLTAGE 4.14
#define BATTERY_MAX_VOLTAGE 4.18 // For calculating percentages, upper limit
#define BAD_BATTERY 0            // This true makes it use the values below, for when your battery doesn't hold the upper voltages anymore
#define BAD_BATTERY_MAX_VOLTAGE 4.00
#define BAD_BATTERY_CHARGE_VOLTAGE 4.05
#define BATTERY_CHARGE_DETECTION_DIFFERENCE 0.10 // The minimum difference to detect that the battery is charging

// Other
#define MAX_MENU_ITEMS 20

// Other other, resources thing
#define RESOURCES_NAME_LENGTH 50
#define IMG_COUNT 30
#define FONT_COUNT 30
// So it's 50*30 bytes

// Task memory size
#define TASK_STACK_DUMP_BATTERY 2000
#define TASK_STACK_BUTTON 4700
#define TASK_STACK_VIBRATION 3500
#define TASK_STACK_WIFI 33500
#define TASK_STACK_SCOM 10000
#define TASK_STACK_WATCHDOG 3500

// Priorities - max is 24
#define MAIN_LOOP_PRIORITY 20
#define WIFI_PRIORITY_REGULAR 10
#define BUTTONS_PRIORITY 12
#define WIFI_PRIORITY_PERSISTENT 4
#define SCOM_PRIORITY 3
#define MOTOR_PRIORITY 1
#define WATCHDOG_PRIORITY 1

// Config
#define CONF_BOOK_CURRENT_PAGE "current_page_"
#define CONF_BOOK_CURRENT_BOOK "current_book"
#define CONF_DISABLE_ALL_VIBRATION "disable_all_vibrations"
#define CONF_DISABLE_WAKE_UP "disable_wake_up"
#define CONF_WEATHER "weather"
#define CONF_BITCOIN "bitcoin"
#define CONF_DRIFT "drift"
#define CONF_DRIFT_FAST "drift_fast"

#endif
