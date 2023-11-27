#ifndef WIFI_H
#define WIFI_H

#include "defines/defines.h"

void initWifi();
void turnOnWifi();
void turnOffWifi();

#if DEBUG
String wifiStatus();
#endif

#endif