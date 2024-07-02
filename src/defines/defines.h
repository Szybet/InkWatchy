#ifndef DEFINES_H
#define DEFINES_H

#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <GxEPD2_BW.h>
#include <SmallRTC.h>
#include "Fonts/FreeSansBold9pt7b.h"
#include <WiFi.h>
#include <WiFiMulti.h>
#include <NTPClient.h>
#include <JSON_Decoder.h>
#include <OpenWeather.h>
#include <StableBMA.h>
#include "FS.h"
#include <LittleFS.h>
#include <mutex>
#include <esp_core_dump.h>
#include "esp_wifi.h"
#include "hal/gpio_types.h"
#include <nvs_flash.h> 

#define PARTIAL_UPDATE true
#define FULL_UPDATE false

#define RTC_WAKEUP_REASON ESP_SLEEP_WAKEUP_EXT0
#define BUTTON_WAKEUP_REASON ESP_SLEEP_WAKEUP_EXT1

#define WATCHY_2 2
#define WATCHY_3 3

struct ImageDef {
    int16_t bw;
    int16_t bh;
    uint8_t *bitmap;
};

extern ImageDef emptyImgPack;

typedef enum 
{
    None,
    Back,
    Menu,
    Up,
    Down,
    LongBack,
    LongMenu,
    LongUp,
    LongDown,
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

#include "config.h" // Needs to be first!
#include "condition.h"
#include "confidential.h"
#include "macros.h"

#include "../hardware/hardware.h"
#include "../hardware/battery.h"
#include "../hardware/RTC.h"
#include "../hardware/sleep.h"
#include "../hardware/buttons.h"
#include "../hardware/display.h"
#include "../hardware/axc.h"
#include "../hardware/fs/littlefs.h"
#include "../functions.h"
#include "../network/wifi/wifiLogic.h"
#include "../network/wifi/wifiTask.h"
#include "../network/ntp.h"
#include "../network/weather.h"
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
#include "../ui/pinInput/pinInput.h"
#include "../ui/settings/nvsSettings.h"
#include "../ui/watchfaceModules/netMod/netMod.h"
#include "../ui/watchfaceModules/emptyMod/emptyMod.h"
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

#endif
