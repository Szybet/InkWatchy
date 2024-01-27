#ifndef WIFI_H
#define WIFI_H

#include "defines/defines.h"

extern bool isWifiTaskRunning;

void initWifi();
void turnOnWifi();
void turnOffWifiMinimal();
void turnOffWifi();
int getSignalStrength();
void regularSync();

#if DEBUG || DEBUG_MENUS == 1
String wifiStatus();
#endif

bool isWifiConnected();

#endif
