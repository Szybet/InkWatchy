#ifndef WIFI_H
#define WIFI_H

#include "defines/defines.h"

void initWifi();
void turnOnWifi();
void turnOnWifiPersistent();
void turnOffWifiMinimal();
void turnOffWifi();
int getSignalStrength();
void regularSync();
bool isWifiTaskCheck();

#if DEBUG || DEBUG_MENUS == 1
String wifiStatus();
#endif

typedef enum {
    WifiOff,
    WifiOn,
    WifiConnected,
} wifiStatusSimple;

wifiStatusSimple wifiStatusWrap();

#endif
