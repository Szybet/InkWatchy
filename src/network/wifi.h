#ifndef WIFI_H
#define WIFI_H

#include "defines/defines.h"

void initWifi();
void turnOnWifi();
void turnOffWifi();
int getSignalStrength();
extern bool isWifiTaskRunning;

#if DEBUG
String wifiStatus();
#endif

bool isWifiConnected();

#endif
