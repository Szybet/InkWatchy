#ifndef CONFIG_H
#define CONFIG_H

// Basics
#define GSR_MINIMUM_BATTERY_VOLTAGE 0 // GSR uses higher, RTC based minimum voltage levels. Change this to 1 if you have problems at lower battery levels
#define DEBUG_MENUS 1                 // Includes debug menus for various things
#define VIBRATION_BUTTON_TIME 35      // Time in ms to the motor to vibrate after clicking a button. 0 means none
#define VIBRATION_ACTION_TIME 60      // Time in ms to the motor to vibrate when the UI receives an action
#define TIME_OFFSET_S 3600            // Time offset in seconds, use this as timezones
#define MENU_LINES false // Option to show lines between buttons in menus
#define BUTTON_LONG_PRESS_MS 500 // Duration until long press registers in miliseconds
#define FULL_DISPLAY_UPDATE_QUEUE 30 // Make a full display update after x of partial ones

// Book things
#define BOOK 1
#define BOOK_ADD_SLEEP_DELAY_MS 60000
#define BOOK_AX_X_PAGE_CHANGE_VALUE_UP 300
#define BOOK_AX_X_PAGE_CHANGE_VALUE_DOWN 250
#define BOOK_AX_Z_BACK_VALUE 500
#define BOOK_AX_Y_BACK_VALUE 500
#define BOOK_AX_X_TOLERANCE 150

// Power savings
#define NIGHT_SLEEP_FOR_M 15 // If it's 1 it doesn't apply, In minutes
#define NIGHT_SLEEP_AFTER_HOUR 23
#define NIGHT_SLEEP_BEFORE_HOUR 5
#define SLEEP_EVERY_MS 6000 //180000 // Goes to sleep timer, is resetted by button presses and other things

// Vault
#define VAULT 1

// Wifi tool
// Set via platformio env
// #define WIFI_TOOL 0

// Advanced
#define BUTTON_TASK_DELAY 140 // In ms, lower means faster button detection but more cpu usage
#define TIME_FOR_WATCHFACE_TO_SHOW_MS 1500

// Debugging help
#define DEBUG 0
#define EINK_COUNTER 0      // Shows a counter somewhere on screen
#define DUMP_INIT_DEBUG 0   // If debug, shows at init init information
#define DUMP_LOOP_DEBUG 0   // If debug, at every loop iteration dump hardware values that can change
#define DUMP_LOOP_SOFTWARE_DEBUG 0 // If debug, at every loop iteration dump software values that can change - no sense in using it with DUMP_LOOP_DEBUG
#define SCREEN_SLEEP_INFO 0 // Shows information on screen when the device goes to screen
#define DRAW_DEBUG_RECT 0   // Shows rectangles where custom functions write bitmaps
#define DUMP_LOOP_DELAY 5000 // delay in ms of dumping loop data

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
#define BATTERY_CHARGE_VOLTAGE 4.27
#define BATTERY_MAX_VOLTAGE 4.18 // For calculating percentages, upper limit

// Other
#define VIB_MOTOR_PIN 13 // Vibration motor
#define MAX_MENU_ITEMS 20
#define RTC_INT_PIN 27

// NVS
#define NVS_CURRENT_PAGE "crp"

#endif
