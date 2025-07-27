#pragma once

#include "defines.h"

#if BLE_ENABLED

extern BLEServer *pServer;
extern BLEService *bleService;
extern bool bleClientConnected;

void initBle();
void startBle();
void exitBle();

#endif