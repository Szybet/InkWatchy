#pragma once

#include "defines.h"

bool connectWifiQuick(wifiQuickCred creds, int maxTimeMs = WIFI_QUICK_MAX_MS);
void disconnectWifiQuick();

