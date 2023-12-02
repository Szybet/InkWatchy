#ifndef WIFI_H
#define WIFI_H

#include "defines/defines.h"

void initWifi();
void turnOnWifiTask();
void turnOnWifi(void *parameter);
void turnOffWifi();

#if DEBUG
String wifiStatus();
#endif

bool isWifiConnected();

#endif