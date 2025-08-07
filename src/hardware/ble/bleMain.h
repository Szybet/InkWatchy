#pragma once

#include "defines.h"

#if BLE_ENABLED

extern BLEServer *pServer;
extern BLEService *bleService;
extern bool bleClientConnected;

void initBle();
void initBle(String name);
void startBle();
void exitBle();

#endif
