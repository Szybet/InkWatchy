#ifndef CONFIG_H
#define CONFIG_H

// Basics
#define GSR_MINIMUM_BATTERY_VOLTAGE 0 // GSR uses higher, RTC something something based minimum voltage levels. Change this to 1 if you have problems at lower battery levels
#define DEBUG_MENUS 1                 // Includes debug menus for various things
// Those vibrations settings are dependent on motor task priority
#define VIBRATION_BUTTON_TIME 35 // Time in ms to the motor to vibrate after clicking a button. 0 means none
#define VIBRATION_ACTION_TIME 60 // Time in ms to the motor to vibrate when the UI receives an action
// maybe a TLDR: if you don't feel any vibrations, turn the diviner smaller and in reverse. It kind of controls the power of the vibrations while the TIME variables the time its vibrating
#define VIBRATION_DIVINE 7           // Divider for the time above for it to not vibrate and not just run. Imits PWM... hard to explain, just look at vibrateMotorTaskFun in hardware.cpp
#define TIME_OFFSET_S 7200           // Time offset in seconds, use this as timezones
#define MENU_LINES false             // Option to show lines between buttons in menus
#define BUTTON_LONG_PRESS_MS 500     // Duration until long press registers in miliseconds
#define FULL_DISPLAY_UPDATE_QUEUE 30 // Make a full display update after x of partial ones
#define LONG_BACK_FULL_REFRESH 1     // Make a full refresh at long back button clicked
#define WATCHDOG_TASK 1              // Wastes resources but tries to detect hangups and you can reset the watch with clicking all buttons too

// Drift & NTP & Syncing
#define REPAIR_TIME_S 600                  // 10 minutes, makes the time drift repair the time every 10 minutes no matter the delay between syncs
#define SYNC_ON_CHARGING 1                 // Keep wifi connected and sync NTP every few minutes because the RTC fucks up
#define SYNC_WIFI 1                        // Sync wifi - only if it's being charger and after the delay below
#define SYNC_WIFI_SINCE_SUCC 72000         // 20h
#define SYNC_WIFI_SINCE_FAIL 60            // 30 s
#define SYNC_NTP_ON_CHARGING_DELAY 90000   // Sync NTP when charging every, in Ms. Default 1.5 minute
#define VALID_PREVIOUS_SYNC_DELAY 300      // Valid minimum delay to calculate drift, below that it will be ignored. Keep in mind to keep it higher then the delay between SYNC_WIFI_SINCE_FAIL and SYNC_NTP_ON_CHARGING_DELAY
#define WIFI_CONNECTION_TRIES 3            // Regular sync, number of tries
#define WIFI_CONNECTION_TRIES_PERSISTENT 1 // Persistent sync (SYNC_ON_CHARGING), number of tries
#define WIFI_MULTI_SYNC_TIME 8000          // Ms, time for waiting to connect to wifi
#define WIFI_MULTI_ERROR_TIME 1000         // Time in MS to try to connect next time (WIFI_CONNECTION_TRIES)

// Watchface modules
#define MODULE_PERSISTENT 1         // Makes modules, like bitcoin not dissmissable, they will always appear and be choosen
#define UPDATE_MODULE_IF_CHARGING 0 // Update modules very fast if its charging, not only every minute
#define WIFI_MODULE 1
#define BITCOIN_MODULE 1   // Remember to define the api key for it in confidential.h
#define SMALL_BTC_MODULE 1 // At default, show the small btc module mode, if 0 show the bigger one

// Priorities - max is 24
#define MAIN_LOOP_PRIORITY 16
#define WIFI_PRIORITY_REGULAR 12
#define BUTTONS_PRIORITY 6
#define WIFI_PRIORITY_PERSISTENT 5
#define SCOM_PRIORITY 3
#define MOTOR_PRIORITY 0
#define WATCHDOG_PRIORITY 0

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

// Power savings
#define NIGHT_SLEEP_FOR_M 45 // If it's 1 it doesn't apply, In minutes
#define NIGHT_SLEEP_AFTER_HOUR 23
#define NIGHT_SLEEP_BEFORE_HOUR 5
#define SLEEP_EVERY_MS 6000               // 180000 // Goes to sleep timer, is resetted by button presses and other things
#define POWER_SAVING_AFTER 60             // Turn on power saving features after a certain battery percantage.
#define POWER_SAVING_OFF_AFTER 20         // Difference in POWER_SAVING_AFTER after which it will be turned off. Make sure POWER_SAVING_AFTER + POWER_SAVING_OFF_AFTER is not above 100
#define LOOP_NO_SCREEN_WRITE_DELAY_MS 125 // Go to "sleep" for 200 ms if the device is woken up ( in a menu for example )
#define HARDWARE_POWER_SAVINGS 1          // Like wifi modem in power saving mode

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
#define TIME_FOR_WATCHFACE_TO_SHOW_MS 1500
#define ADD_BUTTON_DELAY 1.5
#define SMALL_BUTTON_DELAY_MS 15
#define BAT_MINIMAL_DIFFERENCE 0.02

// Debugging help
#define DEBUG 0
#define DEBUG_SLOWER 0             // Makes debug as slow as the regular build
#define EINK_COUNTER 0             // Shows a counter somewhere on screen
#define DUMP_INIT_DEBUG 0          // If debug, shows at init init information
#define DUMP_LOOP_DEBUG 0          // If debug, at every loop iteration dump hardware values that can change
#define DUMP_LOOP_SOFTWARE_DEBUG 0 // If debug, at every loop iteration dump software values that can change - no sense in using it with DUMP_LOOP_DEBUG
#define SCREEN_SLEEP_INFO 0        // Shows information on screen when the device goes to screen
#define DRAW_DEBUG_RECT 0          // Shows rectangles where custom functions write bitmaps
#define DUMP_LOOP_DELAY 5000       // delay in ms of dumping loop data
#define SPEED_THROUGH_TIME 0       // Speeds up time for watchface programming
#define NO_SYNC 0                  // If debug and this is both true, it will not try to sync up automatically
#define VOLTAGE_PRINT_ON 0         // Prints voltage on the screen, really fast

#define SCOM_TASK 0 // Edit this to enable scom task. Requires DEBUG to be enabled too to be applied
#if SCOM_TASK == 1 && DEBUG == 1
#define SCOM_TASK_ENABLED 1
#endif
#define SCOM_SLEEP_DISABLE 1 // Disable sleep when scom is enabled
#if SCOM_TASK_ENABLED
#undef VIBRATION_BUTTON_TIME
#undef VIBRATION_ACTION_TIME
#define VIBRATION_BUTTON_TIME 800 // Time in ms to the motor to vibrate after clicking a button. 0 means none
#define VIBRATION_ACTION_TIME 800 // Time in ms to the motor to vibrate when the UI receives an action
#endif

#define FONT_PREVIEW_MENU 1 // Edit this to enable font preview menu. Requires DEBUG and DEBUG_MENUS to be enabled too
#if FONT_PREVIEW_MENU == 1 && DEBUG == 1 && DEBUG_MENUS == 1
#define FONT_MENU_ENABLED 1
#endif

// Display init magic ;)
#define EPD_CS 5
#define EPD_DC 10
#define EPD_RESET 9
#define EPD_BUSY 19

// Button pins
#define MENU_PIN 26
#define BACK_PIN 25
#define DOWN_PIN 4

// Button masks (for wake up)
#define MENU_MASK GPIO_SEL_26
#define BACK_MASK GPIO_SEL_25
#define DOWN_MASK GPIO_SEL_4

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
#define VIB_MOTOR_PIN 13 // Vibration motor
#define MAX_MENU_ITEMS 20
#define RTC_INT_PIN 27

// NVS
#define NVS_BOOK_CURRENT_PAGE "crp"
#define NVS_DISABLE_ALL_VIBRATION "dbv"
#define NVS_DISABLE_WAKE_UP "dwu"
#define NVS_BOOK_HASH "bhs"
#define NVS_WATCHDOG_DEBUG_DATA "wdd"
#define NVS_WEATHER "wtr"
#define NVS_BITCOIN "btc"

#endif
