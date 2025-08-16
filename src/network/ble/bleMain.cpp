#include "bleMain.h"

#if BLE_ENABLED
#include "rtcMem.h"

BLEServer *pServer = NULL;
BLEService *bleService = NULL;
BLEAdvertising *pAdvertising = NULL;
bool bleClientConnected = false;

class bleServerCallbacks : public BLEServerCallbacks
{
    void onConnect(BLEServer *pServer)
    {
        debugLog("BLE client connected");
        bleClientConnected = true;
    }
    void onDisconnect(BLEServer *pServer)
    {
        debugLog("BLE client disconnected");
        pAdvertising->start();
        bleClientConnected = false;
    }
};

void initBle()
{
    initBle("InkWatchy");
}

void initBle(String name)
{
    debugLog("Init ble called");
    bleClientConnected = false;
    BLEDevice::init(name);
    pServer = BLEDevice::createServer();
    pServer->setCallbacks(new bleServerCallbacks());
}

void startBle()
{
    debugLog("Start ble called");
    bleService->start();

    pAdvertising = BLEDevice::getAdvertising();
    pAdvertising->setAppearance(ESP_BLE_APPEARANCE_GENERIC_WATCH);
    pAdvertising->start();
    resetSleepDelay(BLE_ADVERTISE_TIME * 1000);
}

void exitBle()
{
    pAdvertising->stop();
    pServer->disconnect(0);
    bleService->stop();
    BLEDevice::deinit(false);
    bleClientConnected = false;
}

#endif
