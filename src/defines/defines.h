#ifndef DEFINES_H
#define DEFINES_H

#define PARTIAL_UPDATE true
#define FULL_UPDATE false

#define WATCHY_1 1
#define WATCHY_1_5 15
#define WATCHY_2 2
#define WATCHY_3 3
#define YATCHY 4

#define EXTERNAL_RTC 1
#define INTERNAL_RTC 2

typedef struct {
    const char* ssid;
    const char* password;
} WiFiCred;

#define STATIC_WIFI_CRED static const WiFiCred

// It's before the libraries to be able to affect them (until it doesn't work)
#include "config.h" // Needs to be first!
#include "condition.h"

#define ARDUINOJSON_ENABLE_PROGMEM 0

#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <GxEPD2_BW.h>
#include <SmallRTC.h>
#include "Fonts/FreeSansBold9pt7b.h"
#include <WiFi.h>
#include <WiFiMulti.h>
#include <NTPClient.h>
#include <StableBMA.h>
#include "FS.h"
#include <LittleFS.h>
#include <mutex>
#include <esp_core_dump.h>
#include "esp_wifi.h"
#include "hal/gpio_types.h"
#include <nvs_flash.h> 
#include "driver/rtc_io.h"
#include <Olson2POSIX.h>
#include <ArduinoJson.h>

#include "confidential.h"

static const WiFiCred* wifiCredStatic[] = {
    &wifi_credential1,
    &wifi_credential2,
    &wifi_credential3,
    &wifi_credential4,
    &wifi_credential5,
    &wifi_credential6,
    &wifi_credential7,
    &wifi_credential8,
    &wifi_credential9,
    &wifi_credential10
};
#define SIZE_WIFI_CRED_STAT sizeof(wifiCredStatic) / sizeof(wifiCredStatic[0])

#include "macros.h"

#if WEATHER_INFO
#include <OpenMeteo.h>
#endif

struct ImageDef {
    int16_t bw;
    int16_t bh;
    uint8_t *bitmap;
};

extern ImageDef emptyImgPack;

// It's converted to int by api mod
typedef enum 
{
    Unknown = 0, // For the task to look for answers, used on the yatchy
    None = 1,
    Back = 2,
    Menu = 3,
    Up = 4,
    Down = 5,
    LongBack = 6,
    LongMenu = 7,
    LongUp = 8,
    LongDown = 9,
} buttonState; // This needs to be here because watchface modules use it too and idk why it doesn't work if its in buttons.h :( send help

typedef enum {
    WifiOff,
    WifiOn,
    WifiConnected,
} wifiStatusSimple; // This too

struct wfModule {
    bool show;
    void (*checkShow)(bool* showBool, bool* redrawBool);
    void (*requestShow)(buttonState button, bool* showBool);
}; // Madness -,-

struct bufSize {
    uint8_t *buf;
    int size;
};

extern bufSize emptyBuff;

#include "../hardware/hardware.h"
#include "../hardware/battery.h"
#include "../hardware/RTC.h"
#include "../hardware/sleep.h"
#include "../hardware/buttons.h"
#include "../hardware/display.h"
#include "../hardware/axc.h"
#include "../hardware/fs/littlefs.h"
#if ATCHY_VER == YATCHY
#include "../hardware/i2c.h"
#endif
#if TEMP_CHECKS_ENABLED
#include "../hardware/temp/temp.h"
#endif
#include "../hardware/motor/motor.h"
#include "../hardware/rgb/rgb.h"
#if LP_CORE
#include "../hardware/lpCore/lpCore.h" // Always include it as there are some variables we want outside
#include "../hardware/lpCore/export/lp_logs.h"
#include "../hardware/lpCore/export/lp_rust.h" // Maybe don't call those functions from there
#include "esp_sleep.h"
#include "ulp_lp_core.h"
#include <bootloader_common.h>
#include <ulp_lp_core_memory_shared.h>
#endif
#include "../functions.h"
#include "../network/wifi/wifiLogic.h"
#include "../network/wifi/wifiTask.h"
#include "../network/wifi/wifiQuick.h"
#include "../network/ntp.h"
#include "../network/getTimezone.h"
#if WEATHER_INFO
#include "../network/weather.h"
#endif
#include "../ui/functionsUi.h"
#include "../ui/debug.h"
#include "../ui/batteryDebug.h"
#include "../ui/wifiDebug.h"
#include "../ui/generalDebug.h"
#include "../ui/menu.h"
#include "../ui/chart.h"
#include "../ui/manager.h"
#include "../ui/watchface/watchface.h"
#if BOOK
#include "../ui/book/bookUi.h"
#include "../ui/book/bookSelector.h"
#endif
#if CALENDAR
#include "../ui/calendar/calendar.h"
#endif
#if VAULT
#include "../ui/vault/vaultUi.h"
#endif
#if WIFI_TOOL
#include "../other/wifiTool/wifiTool.h"
#endif
#include "../other/watchdogTask/watchdogTask.h"
#if SCOM_TASK_ENABLED
#include "../other/scomTask/scomTask.h"
#endif
#if APPLE_JOKE
#include "../ui/appleJoke/appleJoke.h"
#include "../ui/appleJoke/appleSour.h"
#endif
#if FONT_MENU_ENABLED
#include "../other/fontPreview/fontPreview.h"
#endif
#if CONWAY
#include "../other/conway/conway.h"
#endif
#if ATCHY_VER == YATCHY
#include "../other/mcp23018/mcp23018.h"
#endif
#include "../ui/pinInput/pinInput.h"
#include "../ui/settings/nvsSettings.h"
#include "../ui/watchfaceModules/netMod/netMod.h"
#include "../ui/watchfaceModules/watchFaceModule.h"
#if BITCOIN_MODULE
#include "../ui/watchfaceModules/bitMod/bitMod.h"
#endif
#if CONWAY_MODULE_ENABLED
#include "../ui/watchfaceModules/conwayMod/conwayMod.h"
#endif
#if BOOK_MODULE_ENABLED
#include "../ui/watchfaceModules/bookMod/bookMod.h"
#endif
#if IMAGE_MODULE
#include "../ui/watchfaceModules/imgMod/imgMod.h"
#endif
#if API_MODULE
#include "../ui/watchfaceModules/apiMod/apiMod.h"
#endif
#if EVENT_MODULE
#include "../ui/watchfaceModules/eventMod/eventMod.h"
#endif

#endif
