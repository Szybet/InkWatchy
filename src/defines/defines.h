#pragma once

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
#if LP_CORE
#include "esp_sleep.h"
#include "ulp_lp_core.h"
#include <bootloader_common.h>
#include <ulp_lp_core_memory_shared.h>
#endif

#include "../functions.h"

// Hardware
#include "../hardware/hardware.h"
#include "../hardware/battery/battery.h"
#include "../hardware/rtc/rtc.h"
#include "../hardware/sleep/sleep.h"
#include "../hardware/input/buttons.h"
#include "../hardware/input/combinations.h"
#include "../hardware/display/display.h"
#include "../hardware/acc/acc.h"
#include "../hardware/fs/littlefs.h"
#if ATCHY_VER == YATCHY
#include "../hardware/i2c/i2c.h"
#endif
#if TEMP_CHECKS_ENABLED
#include "../hardware/temp/temp.h"
#endif
#include "../hardware/motor/motor.h"
#include "../hardware/rgb/rgb.h"
#if ATCHY_VER == YATCHY
#include "../hardware/mcp23018/mcp23018.h"
#include "../hardware/lpCore/rtcPins.h"
#endif
#if LP_CORE
#include "../hardware/lpCore/lpCore.h" // Always include it as there are some variables we want outside
#include "../hardware/lpCore/export/lp_logs.h"
#include "../hardware/lpCore/export/lp_rust.h" // Maybe don't call those functions from there
#endif

// Network
#include "../network/wifi/wifiLogic.h"
#include "../network/wifi/wifiTask.h"
#include "../network/wifi/wifiQuick.h"
#include "../network/ntp.h"
#include "../network/getTimezone.h"

// Weather
#if WEATHER_INFO
#include "../network/weather.h"
#include "../ui/places/weather/weatherSwitches.h"
#endif

// Ui
#include "../ui/functionsUi.h"
#include "../ui/reUse/menu/menu.h"
#include "../ui/reUse/chart/chart.h"
#include "../ui/manager/manager.h"
#include "../ui/watchface/watchFaceLogic.h"
#include "../ui/watchface/watchfaceManagers/wManageOne.h"
#include "../ui/watchface/watchfaceManagers/wManageAll.h"
#include "../ui/watchface/watchfaceManagers/wFunctions.h"
// Places
#include "../ui/places/wifiDebug/wifiDebug.h"
#if DEBUG_MENUS
#include "../ui/places/batteryDebug/batteryDebug.h"
#include "../ui/places/hardwareDebug/hardwareDebug.h"
#include "../ui/places/clockDebug/clockDebug.h"
#include "../ui/places/gitDebug/gitDebug.h"
#endif
#include "../ui/places/watchfaceSelector/watchfaceSel.h"
#if BOOK
#include "../ui/places/book/bookUi.h"
#include "../ui/places/book/bookSelector.h"
#endif
#if CALENDAR
#include "../ui/places/calendar/calendar.h"
#endif
#if VIDEO_PLAYER
#include "../ui/places/video/video.h"
#endif
#if VAULT
#include "../ui/places/vault/vaultUi.h"
#endif
#if PONG
#include "../ui/places/pong/pong.h"
#endif
#if TETRIS
#include "../ui/places/tetris/tetrisPlace.h"
#endif
#if CREDITS
#include "../ui/places/credits/credits.h"
#endif
#if APPLE_JOKE
#include "../ui/appleJoke/appleJoke.h"
#include "../ui/appleJoke/appleSour.h"
#endif
#if RGB_DIODE
#include "../ui/places/party/party.h"
#endif
#if FONT_MENU_ENABLED
#include "../ui/places/fontPreview/fontPreview.h"
#endif
#if CONWAY
#include "../ui/places/conway/conway.h"
#endif
#include "../ui/reUse/pinInput/pinInput.h"
#include "../ui/places/settings/powerSettings.h"
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
#include "../ui/watchface/watchfaces/shades_Szybet/shades.h"
#endif
#if WATCHFACE_ANALOG_SHARP_SZYBET
#include "../ui/watchface/watchfaces/analogSharp_Szybet/analogSharp.h"
#endif
#if WATCHFACE_TAYCHRON
#include "../ui/watchface/watchfaces/taychron_tay/taychron.h"
#endif

// Other
#if DEBUG
#include "../other/debug/debugMain.h"
#include "../other/debug/debug.h"
#endif
#if WIFI_TOOL
#include "../other/wifiTool/wifiTool.h"
#endif
#include "../other/watchdogTask/watchdogTask.h"
#if SCOM_TASK_ENABLED
#include "../other/scomTask/scomTask.h"
#endif