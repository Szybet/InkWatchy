#include "bleMain.h"

#if BLE_ENABLED

BLEServer *pServer = NULL;
BLEService *bleService = NULL;
BLECharacteristic *pCharacteristic = NULL;
BLEAdvertising *pAdvertising = NULL;

class bleServerCallbacks : public BLEServerCallbacks
{
    void onConnect(BLEServer *pServer)
    {
        debugLog("Client connected");
    }
    void onDisconnect(BLEServer *pServer)
    {
        debugLog("Client disconnected");
        pAdvertising->start();
    }
};

void initBle()
{
    BLEDevice::init("InkWatchy");
    pServer = BLEDevice::createServer();
    pServer->setCallbacks(new bleServerCallbacks());
}

void startBle()
{
    bleService->start();
    pAdvertising = BLEDevice::getAdvertising();
    pAdvertising->start();
}

void exitBle()
{
    pAdvertising->stop();
    BLEDevice::deinit(true);
}

#endif