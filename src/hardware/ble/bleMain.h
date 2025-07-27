#pragma once

#include "defines.h"

#if BLE_ENABLED

extern BLEServer *pServer;
extern BLEService *bleService;
extern BLECharacteristic *pCharacteristic;

void initBle();
void startBle();
void exitBle();

#endif