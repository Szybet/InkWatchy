#ifndef CONFIG_H
#define CONFIG_H

// Basics
#define GSR_MINIMUM_BATTERY_VOLTAGE 0 // GSR uses higher, RTC based minimum voltage levels. Change this to 1 if you have problems at lower battery levels
#define LOOP_DELAY 10                 // Speed of the device basically

// Wifi
#define WIFI_SSID1 "dragonn"
#define WIFI_PASS1 "ca9hi6HX"

#define WIFI_SSID2 "dragonn2"
#define WIFI_PASS2 "Twb3MRYd"

#define WIFI_SSID3 ""
#define WIFI_PASS3 ""

#define WIFI_SSID4 ""
#define WIFI_PASS4 ""

#define WIFI_SSID5 ""
#define WIFI_PASS5 ""

#define WIFI_SSID6 ""
#define WIFI_PASS6 ""

#define WIFI_SSID7 ""
#define WIFI_PASS7 ""

#define WIFI_SSID8 ""
#define WIFI_PASS8 ""

#define WIFI_SSID9 ""
#define WIFI_PASS9 ""

#define WIFI_SSID10 ""
#define WIFI_PASS10 ""

// Advanced
#define DEBUG 1

// Display init magic ;)
#define EPD_CS 5
#define EPD_DC 10
#define EPD_RESET 9
#define EPD_BUSY 19

#define PARTIAL_UPDATE true
#define FULL_UPDATE false

// Button pins
#define MENU_PIN 26
#define BACK_PIN 25
#define DOWN_PIN 4
#define DOWN_PIN 4

// Voltage reading average
#define VOLTAGE_AVG_COUNT 20
#define VOLTAGE_AVG_DELAY 5

// Battery
#define BATTERY_MIN_VOLTAGE 3.3
#define BATTERY_CRIT_VOLTAGE 3.15
#define BATTERY_MAX_VOLTAGE 4.25 // For calculating percentages, upper limit

#endif
