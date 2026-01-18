#include "bleMain.h"

#if BLE_ENABLED

static BLEUUID bleKeyboardUUID((uint16_t)0x1812);
static BLEUUID bleKeyboardCharUUID((uint16_t)0x2a4d);
static BLEAdvertisedDevice* myBluetoothKeyboard = NULL;
bool BleKeyboardDoConnect = false;

void BleKeyboardCallbacks::onResult(BLEAdvertisedDevice advertisedDevice)
{
        debugLog("BLE Advertised Device found: " + advertisedDevice.toString());

        // We have found a device, let us now see if it contains the service we are looking for.
        if (advertisedDevice.haveServiceUUID() && advertisedDevice.isAdvertisingService(bleKeyboardUUID))
        {

            BLEDevice::getScan()->stop();
            myBluetoothKeyboard = new BLEAdvertisedDevice(advertisedDevice);
            BleKeyboardDoConnect = true;
            debugLog("Ble keyboard found?");
        } // Found our server
};

#endif
