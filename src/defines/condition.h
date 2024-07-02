#include "defines.h"

/*
Template for versioning
#if ATCHY_VER == WATCHY_2

#elif ATCHY_VER == WATCHY_3

#endif
*/
#if CONWAY && CONWAY_MODULE
#define CONWAY_MODULE_ENABLED 1
#endif

#if BOOK && BOOK_MODULE
#define BOOK_MODULE_ENABLED 1
#endif

#if SCOM_TASK == 1 && DEBUG == 1
#define SCOM_TASK_ENABLED 1
#endif
#if SCOM_TASK_ENABLED
#undef VIBRATION_BUTTON_TIME
#undef VIBRATION_ACTION_TIME
#define VIBRATION_BUTTON_TIME 800 // Time in ms to the motor to vibrate after clicking a button. 0 means none
#define VIBRATION_ACTION_TIME 800 // Time in ms to the motor to vibrate when the UI receives an action
#endif

#if FONT_PREVIEW_MENU == 1 && DEBUG == 1 && DEBUG_MENUS == 1
#define FONT_MENU_ENABLED 1
#endif

// Display init magic ;)
#if ATCHY_VER == WATCHY_2
#define EPD_CS 5
#define EPD_DC 10
#define EPD_RESET 9
#define EPD_BUSY 19
#elif ATCHY_VER == WATCHY_3
#define EPD_CS 33
#define EPD_DC 34
#define EPD_RESET 35
#define EPD_BUSY 36
#endif

// Button pins
#if ATCHY_VER == WATCHY_2
// Maybe I could od a !not but whatever
#define BUT_STATE LOW
#define BUT_CLICK_STATE HIGH
#define BUTTON_INTER_COND RISING
#define MENU_PIN 26
#define BACK_PIN 25
#define DOWN_PIN 4
#define UP_PIN 35 // Watchy 1.5 Has the pin 32 here
#elif ATCHY_VER == WATCHY_3
#define BUT_STATE HIGH
#define BUT_CLICK_STATE LOW
#define BUTTON_INTER_COND FALLING
#define MENU_PIN 7
#define BACK_PIN 6
#define DOWN_PIN 8
#define UP_PIN 0
#endif

// Other
#if ATCHY_VER == WATCHY_2
#define VIB_MOTOR_PIN 13
#define RTC_INT_PIN 27
#define EXT1_WAKEUP_STATE ESP_EXT1_WAKEUP_ANY_HIGH
#elif ATCHY_VER == WATCHY_3
#define VIB_MOTOR_PIN 17
#define RTC_INT_PIN -1
#define EXT1_WAKEUP_STATE ESP_EXT1_WAKEUP_ANY_LOW
#endif