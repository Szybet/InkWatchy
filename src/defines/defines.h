#pragma once

#include "bareDefines.h"

// It's before the libraries to be able to affect them (until it doesn't work)
#include "config.h" // Needs to be first!
#include "condition.h"

#include <Arduino.h>

// Here add BT things
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>

#include <Adafruit_GFX.h>
#include <GxEPD2_BW.h>
#include <SmallRTC.h>
#include "Fonts/FreeSansBold9pt7b.h"
#include <WiFi.h>
#include <NTPClient.h>
#include <StableBMA.h>
#include "FS.h"
#include <LittleFS.h>
#include <mutex>
#include <esp_core_dump.h>
#include "esp_wifi.h"
#include "hal/gpio_types.h"
#include <nvs_flash.h>
#include <atomic>
#include "driver/rtc_io.h"
#include <Olson2POSIX.h>
#undef ARDUINOJSON_ENABLE_PROGMEM
#define ARDUINOJSON_ENABLE_PROGMEM 0 // Needed
#include <ArduinoJson.h>
#include <mbedtls/md5.h>
#if (DEBUG && SCREENSHOT_ENDPOINT)
#include <ESPAsyncWebServer.h>
#endif
#include "rusty.h"

// Order important
#include "macros.h"
#include "definesVars.h"
#include "confidential.h"

// Order important
#include "macros.h"
#include "definesVars.h"
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

#if WEATHER_INFO
#include <OpenMeteo.h>
#endif
#if LP_CORE
#include "esp_sleep.h"
#include "ulp_lp_core.h"
#include <bootloader_common.h>
#include <ulp_lp_core_memory_shared.h>
#endif

#if !INITIAL_SCREEN_INVERT
#define SC_BLACK_INIT GxEPD_BLACK;
#define SC_WHITE_INIT GxEPD_WHITE;
#else
#define SC_BLACK_INIT GxEPD_WHITE;
#define SC_WHITE_INIT GxEPD_BLACK;
#endif

// Screen Colors
extern uint16_t SCBlack;
extern uint16_t SCWhite;

#include "../functions.h"
#include "../other/rustExport/rEGeneric.h"

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
#if ATCHY_VER == YATCHY || ATCHY_VER == WATCHY_3
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
#if BLE_ENABLED
#include "../hardware/ble/bleMain.h"
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

// Alarms
#if INK_ALARMS
#include "../ui/places/inkAlarms/inkAlarm.h"
#include "../ui/places/inkAlarms/alarmEdit.h"
#include "../ui/places/inkAlarms/alarmEditMenu.h"
#include "../ui/places/inkAlarms/alarmSelector.h"
#include "../ui/places/inkAlarms/alarmRinging.h"
#include "../ui/places/inkAlarms/alarmQuickSet.h"
#if POMODORO_ALARM
#include "../ui/places/inkAlarms/pomodoro.h"
#endif
#endif

// Ui
#include "../ui/functionsUi.h"
#include "../ui/reUse/menu/menu.h"
#include "../ui/reUse/chart/chart.h"
#include "../ui/reUse/setTime/setTime.h"
#include "../ui/manager/manager.h"
#include "../ui/watchface/watchFaceLogic.h"
#include "../ui/watchface/watchfaceManagers/wManageOne.h"
#include "../ui/watchface/watchfaceManagers/wManageAll.h"
#include "../ui/watchface/watchfaceManagers/wFunctions.h"
// Places
#include "../ui/places/wifiDebug/wifiDebug.h"
#if DEBUG == 1 || DEBUG_MENUS == 1
#include "../ui/places/batteryDebug/batteryDebug.h"
#include "../ui/places/hardwareDebug/hardwareDebug.h"
#include "../ui/places/clockDebug/clockDebug.h"
#include "../ui/places/gitDebug/gitDebug.h"
#include "../ui/places/accDebug/accDebug.h"
#include "../ui/places/motorDebug/motorDebug.h"
#endif
#include "../ui/places/watchfaceSelector/watchfaceSel.h"
#include "../ui/places/powerSettings/powerSettings.h"
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
#if JUMPER
#include "../ui/places/jumper/jumper.h"
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
#if HEART_MONITOR
#include "../ui/places/heartMonitor/heartMonitor.h"
#endif
#if FONT_MENU_ENABLED
#include "../ui/places/fontPreview/fontPreview.h"
#endif
#if CONWAY
#include "../ui/places/conway/conway.h"
#endif
#include "../ui/reUse/pinInput/pinInput.h"
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
#if ALARM_MODULE && INK_ALARMS
#include "../ui/watchfaceModules/alarmMod/alarmMod.h"
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
#if WATCHFACE_SLATE
#include "../ui/watchface/watchfaces/slate_Suddoku/slate.h"
#endif
#if GSR_WATCHFACES
#include "../ui/watchface/watchfaceManagers/wManageGSR/wManageGSR.h"
#endif
#if BAIKY
#include "../ui/places/baiky/baiky.h"
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

// Include localization system after all other includes
#include "localization.h"