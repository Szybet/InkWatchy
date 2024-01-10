#include "config.h"

#if WIFI_TOOL

#ifndef WIFI_TOOL_H
#define WIFI_TOOL_H

#include "../../defines/defines.h"
extern bool wifiToolRunning;

String wifiToolStatus();
void initWifiTool();
void stopWifiTool();

#endif

#endif
