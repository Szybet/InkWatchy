#pragma once

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
#if ATCHY_VER == WATCHY_2 || ATCHY_VER == WATCHY_1 || ATCHY_VER == WATCHY_1_5
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
#if ATCHY_VER == WATCHY_2 || ATCHY_VER == WATCHY_1 || ATCHY_VER == WATCHY_1_5
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
#if ATCHY_VER == WATCHY_2 || ATCHY_VER == WATCHY_1 || ATCHY_VER == WATCHY_1_5
// Maybe I could od a !not but whatever
#define BUT_STATE LOW
#define BUT_CLICK_STATE HIGH
#define BUTTON_INTER_COND RISING
#define MENU_PIN 26
#define BACK_PIN 25
#define DOWN_PIN 4
#if ATCHY_VER == WATCHY_2
#define UP_PIN 35
#elif ATCHY_VER == WATCHY_1_5 || ATCHY_VER == WATCHY_1
#define UP_PIN 32
#endif
#elif ATCHY_VER == WATCHY_3
#define BUT_STATE HIGH
#define BUT_CLICK_STATE LOW
#define BUTTON_INTER_COND FALLING
#define MENU_PIN 7
#define BACK_PIN 6
#define DOWN_PIN 8
#define UP_PIN 0
#elif ATCHY_VER == YATCHY
// Don't define it, to show errors
#define BUT_STATE LOW
#define BUT_CLICK_STATE HIGH
#endif

// Other
#if ATCHY_VER == WATCHY_2 || ATCHY_VER == WATCHY_1 || ATCHY_VER == WATCHY_1_5
#define VIB_MOTOR_PIN 13
#define RTC_INT_PIN 27
#define EXT1_WAKEUP_STATE ESP_EXT1_WAKEUP_ANY_HIGH
#define ADC_VOLTAGE_DIVIDER 500.0f // Shortened version, by GuruSR
#if ATCHY_VER == WATCHY_2
#define BATT_ADC_PIN 34
#elif ATCHY_VER == WATCHY_1_5
#define BATT_ADC_PIN 35
#elif ATCHY_VER == WATCHY_1
#define BATT_ADC_PIN 33
#endif
#elif ATCHY_VER == WATCHY_3
#define VIB_MOTOR_PIN 17
#define RTC_INT_PIN -1
#define EXT1_WAKEUP_STATE ESP_EXT1_WAKEUP_ANY_LOW
#define BATT_ADC_PIN 9
#define ADC_VOLTAGE_DIVIDER 739.750f // Voltage divider at battery ADC, original values were ((360.0f + 100.0f) / 360.0f), this math is done by GuruSR
#define CHRG_STATUS_PIN 10 // If HIGH, usb is connected. Someone with a v3 is free to use interrupts for that and enable wakeups for this pin too
#elif ATCHY_VER == YATCHY
#define VIB_MOTOR_PIN 14
#define RTC_INT_PIN -1
#define EXT1_WAKEUP_STATE ESP_EXT1_WAKEUP_ANY_LOW
#define BATT_ADC_PIN 6
#define ADC_VOLTAGE_DIVIDER 710.094f // 300K and 806K
#define CHRG_STATUS_PIN -1
#endif

// Motor power multiplier
#if VIBRATION_MULTIPLIER_OVERWRITE == 0
#undef VIBRATION_MULTIPLIER
#if ATCHY_VER == YATCHY
#define VIBRATION_MULTIPLIER 0.75
#else
#define VIBRATION_MULTIPLIER 0.8
#endif
#endif

// RTC type
#if FORCE_INTERNAL_RTC
#define RTC_TYPE INTERNAL_RTC
#if FORCED_INTERNAL_RTC_QUARTZ
#define RTC_32KHZ_CRYSTAL 1
#endif
#else
#if ATCHY_VER == WATCHY_2 || ATCHY_VER == WATCHY_1 || ATCHY_VER == WATCHY_1_5
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

// CPU
#if ATCHY_VER == WATCHY_2 || ATCHY_VER == WATCHY_1 || ATCHY_VER == WATCHY_1_5 || ATCHY_VER == WATCHY_3
#define CPU_MINIMAL_SPEED 80
#define CPU_NORMAL_SPEED 160
#define CPU_FAST_SPEED 240
#elif ATCHY_VER == YATCHY
#define CPU_MINIMAL_SPEED 80
#define CPU_NORMAL_SPEED 120
#define CPU_FAST_SPEED 160
#endif

// AXC
#if ATCHY_VER == WATCHY_2 || ATCHY_VER == WATCHY_1 || ATCHY_VER == WATCHY_1_5
#define ACC_ENABLED 1
#elif ATCHY_VER == WATCHY_3
#define ACC_ENABLED 1
#elif ATCHY_VER == YATCHY
#define ACC_ENABLED 1
#endif

// USB JTAG
#if ATCHY_VER == YATCHY
#define USB_JTAG 1
#else
#define USB_JTAG 0 // I don't have a watchy v3
#endif

// Yatchy specific things
#if ATCHY_VER == YATCHY
#define LP_CORE 1

#define MENU_PIN 0 // A0
#define DOWN_PIN 1 // A1
#define UP_PIN 2 // A2

#define YATCHY_DISPLAY_CS 8 // B0

// Module definitions
#define YATCHY_NO_MODULE 0
#define YATCHY_DEFAULT_MODULE 1

// Select the module
#define YATCHY_MODULE YATCHY_DEFAULT_MODULE
// Always define YATCHY_BACK_BTN, maybe just as 255 otherwise compiler errors, done because it simplified code

#if YATCHY_MODULE == YATCHY_DEFAULT_MODULE
// Everything here is optional, or should be, if something doesn't work let me know
#define BACK_PIN 3 // A3
#define YATCHY_BACK_BTN
#define RGB_DIODE 1
#define RGB_DIODE_RED_PIN 12
#define RGB_DIODE_GREEN_PIN 11
#define RGB_DIODE_BLUE_PIN 10
#define BATTERY_RGB_DIODE 1 // If to show battery charge / fully charge with rgb diode

// For colors, look up to src/hardware/rgb/rgb.h
#define BATTERY_CHARGING_COLOR IwYellow
#define BATTERY_CHARGED_COLOR IwGreen
#define BATTERY_DISCHARGING_COLOR IwRed // What color to show for a second if its starting to discharge

#endif
#endif

// Yatchy mic mod / module
#define YATCHY_MIC 1
#if YATCHY_MIC
// Only GND can be a multiplexer pin
#define MIC_PIN_SCK 20
#define MIC_PIN_SM 18
#define MIC_PIN_SD 15
#define MIC_PIN_GND 14 // Multiplexer B6
#define MIC_PIN_VDD 19
#endif

// Lp core
#ifndef LP_CORE
#define LP_CORE 0
#endif
#if DEBUG && LP_CORE_TEST_RUN && LP_CORE
#define LP_CORE_TEST_ENABLED 1
#else
#define LP_CORE_TEST_ENABLED 0
#endif

#if FORCE_DISABLE_ACC == 1
#undef ACC_ENABLED
#define ACC_ENABLED 0
#endif

// Bma version
#if ATCHY_VER == WATCHY_3 || ATCHY_VER == WATCHY_2 || ATCHY_VER == WATCHY_1 || ATCHY_VER == WATCHY_1_5
#define BMA_VERSION 423
#elif ATCHY_VER == YATCHY
#define BMA_VERSION 456 // Needs 100Khz I2C
// Uncomment this, disable the previous line if you have the bma 530
// #define BMA_VERSION 530
#endif

#if BMA_VERSION < 500
#define BMAM_4 1 // BMA major
#define BMAM_5 0
#else
#define BMAM_5 1
#define BMAM_4 0
#endif
