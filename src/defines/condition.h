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

// More display magic but SPI
#if ATCHY_VER == WATCHY_2
// Unused really as defaults are good
#define EPD_SPI_SCK 18
#define EPD_SPI_MISO 19
#define EPD_SPI_MOSI 23
#define EPD_SPI_SS 5
#elif ATCHY_VER == WATCHY_3
// But those are used
#define EPD_SPI_SCK 47
#define EPD_SPI_MISO 46
#define EPD_SPI_MOSI 48
#define EPD_SPI_SS 33
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
#define RTC_WAKEUP_REASON ESP_SLEEP_WAKEUP_EXT0
#define BATT_ADC_PIN 34
#elif ATCHY_VER == WATCHY_3
#define VIB_MOTOR_PIN 17
#define RTC_INT_PIN -1
#define EXT1_WAKEUP_STATE ESP_EXT1_WAKEUP_ANY_LOW
#define RTC_WAKEUP_REASON ESP_SLEEP_WAKEUP_TIMER
#define BATT_ADC_PIN 9
#define ADC_VOLTAGE_DIVIDER ((360.0f + 100.0f) / 360.0f) // Voltage divider at battery ADC
#endif