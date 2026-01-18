#include "bleMain.h"

#if BLE_ENABLED

void initBleHost(BLEAdvertisedDeviceCallbacks *pAdvertisedDeviceCallbacks)
{
    debugLog("Init ble host called");
    BLEDevice::init(BLE_NAME);
    BLEScan *pBLEScan = BLEDevice::getScan();
    pBLEScan->setAdvertisedDeviceCallbacks(pAdvertisedDeviceCallbacks);
    pBLEScan->setActiveScan(true);
    pBLEScan->start(5, false);
}

#endif
