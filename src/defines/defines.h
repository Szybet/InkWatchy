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
#include <ArduinoNvs.h>
#include <StableBMA.h>

#define PARTIAL_UPDATE true
#define FULL_UPDATE false

#define RTC_WAKEUP_REASON ESP_SLEEP_WAKEUP_EXT0
#define BUTTON_WAKEUP_REASON ESP_SLEEP_WAKEUP_EXT1

struct ImageDef {
    const uint8_t *bitmap;
    int16_t bw;
    int16_t bh;
};

const ImageDef emptyImgPack = {NULL, 0, 0};

#include "config.h" // Needs to be first!
#include "confidential.h"
#include "macros.h"
#include "images.h"
#include "fonts.h"

#include "../hardware/hardware.h"
#include "../hardware/battery.h"
#include "../hardware/RTC.h"
#include "../hardware/sleep.h"
#include "../hardware/buttons.h"
#include "../hardware/display.h"
#include "../hardware/axc.h"
#include "../functions.h"
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
#endif
#if VAULT
#include "../ui/vault/vaultUi.h"
#endif
#if WIFI_TOOL
#include "../other/wifiTool/wifiTool.h"
#endif
#if APPLE_JOKE
#include "../ui/appleJoke/appleJoke.h"
#endif
#include "../ui/pinInput/pinInput.h"
#include "../network/wifi.h"
#include "../network/ntp.h"
#include "../network/weather.h"

#endif
