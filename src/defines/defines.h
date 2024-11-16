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

// It's before the libraries to be able to affect them (until it doesn't work)
#include "config.h" // Needs to be first!
#include "condition.h"

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
#define ARDUINOJSON_ENABLE_PROGMEM 0 // Needed
#include <ArduinoJson.h>

// Order important
#include "macros.h"
#include "definesVars.h"
#include "confidential.h"

// Needs to be after confidential.h and definesVars.h
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

#if WEATHER_INFO
#include <OpenMeteo.h>
#endif

#if DEBUG
#include "../other/debugMain/debugMain.h"
#endif
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
#include "../ui/watchface/watchFaceLogic.h"
#include "../ui/watchface/watchfaceManagers/wManageOne.h"
#include "../ui/watchface/watchfaceManagers/wManageAll.h"
#include "../ui/watchface/watchfaceManagers/wFunctions.h"
#include "../ui/watchface/watchfaceSelector/watchfaceSel.h"
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
#if PONG
#include "../ui/pong/pong.h"
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
#if WATCHFACE_INKFIELD_SZYBET
#include "../ui/watchface/watchfaces/inkField_Szybet/inkField.h"
#endif
#if WATCHFACE_SHADES_SZYBET
#include "../ui/watchface/watchfaces/shades_szybet/shades.h"
#endif

#endif
