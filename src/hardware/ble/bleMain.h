#pragma once

#include "defines.h"

#if BLE_ENABLED || BLE_HOST_ENABLED
extern bool bleClientConnected;
#endif

#if BLE_ENABLED

// Server functions (X chooses to connect to inkwatchy)
extern BLEServer *pServer;
extern BLEService *bleService;

void initBle();
void startBle();
void exitBle();
#endif

#if BLE_HOST_ENABLED
// Client (Host) functions (Inkwatchy chooses to connect to X)
void hostBleDeInitEverything();
void hostBleInitClient();
void hostBleStartScan(uint32_t durationSeconds);
int hostBleGetScannedDevicesCount();
String hostBleGetScannedDeviceName(int index);
bool hostBleConnectToDevice(int index);
void hostBleDisconnectDevice();
extern String hostBleClientName;
extern notify_callback hostBleNotifyCallback;
#endif
