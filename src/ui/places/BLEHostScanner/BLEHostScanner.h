#pragma once

#include "defines.h"

#if BLE_HOST_ENABLED

void initBluetoothHostScanner();
void exitBluetoothHostScanner();
void loopBluetoothHostScanner();

#endif
