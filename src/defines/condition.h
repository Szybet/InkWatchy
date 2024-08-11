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
#define VIBRATION_BUTTON_TIME 150 // Time in ms to the motor to vibrate after clicking a button. 0 means none
#define VIBRATION_ACTION_TIME 150 // Time in ms to the motor to vibrate when the UI receives an action
#undef DEBUG_CPU_SPEED
#define DEBUG_CPU_SPEED maxSpeed
#undef SERIAL_LOG_DELAY_MS
#define SERIAL_LOG_DELAY_MS 0
#undef DISABLE_SLEEP
#define DISABLE_SLEEP 1
#undef MINIMAL_LOGS
#define MINIMAL_LOGS 0
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
#elif ATCHY_VER == YATCHY
#define EPD_CS -1 // Chip select
#define EPD_DC 3
#define EPD_RESET 2
#define EPD_BUSY 4
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
#elif ATCHY_VER == YATCHY
#define EPD_SPI_SCK 7
#define EPD_SPI_MISO -1
#define EPD_SPI_MOSI 5
#define EPD_SPI_SS -1 // Chip select!
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
#elif ATCHY_VER == YATCHY
#define BUT_STATE LOW
#define BUT_CLICK_STATE HIGH
#define BUTTON_INTER_COND RISING
#define MENU_PIN -1
#define BACK_PIN -1
#define DOWN_PIN -1
#define UP_PIN -1
#endif

// Other
#if ATCHY_VER == WATCHY_2
#define VIB_MOTOR_PIN 13
#define RTC_INT_PIN 27
#define EXT1_WAKEUP_STATE ESP_EXT1_WAKEUP_ANY_HIGH
#define BATT_ADC_PIN 34
#elif ATCHY_VER == WATCHY_3
#define VIB_MOTOR_PIN 17
#define RTC_INT_PIN -1
#define EXT1_WAKEUP_STATE ESP_EXT1_WAKEUP_ANY_LOW
#define BATT_ADC_PIN 9
#define ADC_VOLTAGE_DIVIDER ((360.0f + 100.0f) / 360.0f) // Voltage divider at battery ADC
#define CHRG_STATUS_PIN 10 // If HIGH, usb is connected. Someone with a v3 is free to use interrupts for that and enable wakeups for this pin too
#elif ATCHY_VER == YATCHY
#define VIB_MOTOR_PIN -1
#define RTC_INT_PIN -1
#define EXT1_WAKEUP_STATE ESP_EXT1_WAKEUP_ANY_LOW
#define BATT_ADC_PIN -1
#define ADC_VOLTAGE_DIVIDER -1
#define CHRG_STATUS_PIN -1
#endif

// RTC type
#if FORCE_INTERNAL_RTC
#define RTC_TYPE INTERNAL_RTC
#if FORCED_INTERNAL_RTC_QUARTZ
#define RTC_32KHZ_CRYSTAL 1
#endif
#else
#if ATCHY_VER == WATCHY_2
#define RTC_TYPE EXTERNAL_RTC
#elif ATCHY_VER == WATCHY_3
#define RTC_TYPE INTERNAL_RTC
#define RTC_32KHZ_CRYSTAL 1
// This won't work, needs to be in platformio.ini too
#define SMALL_RTC_NO_DS3232 1
#define SMALL_RTC_NO_PCF8563 1
#elif ATCHY_VER == YATCHY
#define RTC_TYPE INTERNAL_RTC
#define RTC_32KHZ_CRYSTAL 0 // TODO: not yet!
// This won't work, needs to be in platformio.ini too
#define SMALL_RTC_NO_DS3232 1
#define SMALL_RTC_NO_PCF8563 1
#define SMALL_RTC_NO_EXT0 1 // esp32c6 simply doesn't have it, as a defined function
#endif
#endif
#if RTC_32KHZ_CRYSTAL != 1
#define RTC_32KHZ_CRYSTAL 0
#endif

// Temps
#if TEMP_CHECKS != 0 && TEMP_HIGHER_LIMIT_RELATIVE != 0 && TEMP_LOWER_LIMIT_RELATIVE != 0 && TEMP_REBOOT_LIMIT_RELATIVE != 0
#define TEMP_CHECKS_ENABLED 1
#else
#define TEMP_CHECKS_ENABLED 0
#endif