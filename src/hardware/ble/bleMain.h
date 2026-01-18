#pragma once

#include "defines.h"

#if BLE_ENABLED

extern BLEServer *pServer;
extern BLEService *bleService;
extern bool bleClientConnected;

void initBle();
void startBle();
void exitBle();
// As in, we want a keyboard to connect to us
void initBleHost(BLEAdvertisedDeviceCallbacks *pAdvertisedDeviceCallbacks);

class BleKeyboardCallbacks : public BLEAdvertisedDeviceCallbacks
{
public:
    void onResult(BLEAdvertisedDevice advertisedDevice) override;
};

#endif
