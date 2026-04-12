#include "bleMain.h"
#include <vector>

#if BLE_ENABLED

bool bleClientConnected = false; // Used in both bluetooths.

BLEServer *pServer = NULL;
BLEService *bleService = NULL;
BLEAdvertising *pAdvertising = NULL;

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
    debugLog("Init ble called");
    bleClientConnected = false;
    BLEDevice::init("InkWatchy");
    pServer = BLEDevice::createServer();
    pServer->setCallbacks(new bleServerCallbacks());
}

void startBle()
{
    debugLog("Start ble called");
    bleService->start();
    pAdvertising = BLEDevice::getAdvertising();
    pAdvertising->start();
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

#if BLE_HOST_ENABLED

BLESecurity *pSecurity = NULL;
BLEScan *pBLEScan = NULL;
BLEScanResults *pScannedDevices = NULL;
BLEClient *hostBlePclient = NULL;
String hostBleClientName = ""; // Available after hostBleConnectToDevice
notify_callback hostBleNotifyCallback = NULL;

class MyAdvertisedDeviceCallbacks : public BLEAdvertisedDeviceCallbacks
{
    void onResult(BLEAdvertisedDevice advertisedDevice)
    {
        // debugLog("Advertised Device found: " + String(advertisedDevice.toString().c_str()));
    }
};

void remove_all_bonded_devices()
{
    int device_count = esp_ble_get_bond_device_num();

    if (device_count < 0)
    {
        debugLog("Error: Failed to retrieve the number of bonded devices.");
        return;
    }

    if (device_count == 0)
    {
        debugLog("No bonded devices found to remove.");
        return;
    }

    esp_ble_bond_dev_t *device_list = (esp_ble_bond_dev_t *)malloc(sizeof(esp_ble_bond_dev_t) * device_count);

    if (device_list == NULL)
    {
        debugLog("Error: Failed to allocate memory for bonded devices.");
        return;
    }

    esp_err_t ret = esp_ble_get_bond_device_list(&device_count, device_list);

    if (ret != ESP_OK)
    {
        String errStr = String(ret);
        debugLog("Error: Failed to retrieve bonded device list. Code: ");
        debugLog(errStr);
        free(device_list);
        return;
    }

    debugLog("Successfully retrieved " + String(device_count) + " bonded devices.");

    for (int i = 0; i < device_count; i++)
    {
#if DEBUG
        String logMsg = "Attempting to remove device " + String(i + 1) + "/" + String(device_count) + " with address: ";
        for (int j = 0; j < 6; j++)
        {
            char buffer[3];
            sprintf(buffer, "%02X", device_list[i].bd_addr[j]);
            logMsg += String(buffer);
            if (j < 5)
                logMsg += ":";
        }
        debugLog(logMsg);
#endif
        esp_err_t remove_ret = esp_ble_remove_bond_device(device_list[i].bd_addr);

        if (remove_ret != ESP_OK)
        {
            debugLog("Failed to remove: " + String(remove_ret));
        }
        else
        {
            debugLog("Successfully removed.");
        }
    }

    free(device_list);
}

void hostBleDeInitEverything()
{
    debugLog("Called hostBleDeInitEverything");
    /*
    if (hostBlePclient != NULL)
    {
        delete hostBlePclient;
        hostBlePclient = NULL;
    }
    if (pSecurity != NULL)
    {
        BLESecurity::resetSecurity();
        delete pSecurity;
        pSecurity = NULL;
    }
    if (pBLEScan != NULL)
    {
        pBLEScan->stop();
        delete pBLEScan;
        pBLEScan = NULL;
    }
    if (pScannedDevices != NULL) {
        // So what
        pScannedDevices = NULL;
    }
    */
    remove_all_bonded_devices();
    hostBleClientName = "";
    bleClientConnected = false;
    BLEDevice::deinit();
}

void hostBleInitClient()
{
    debugLog("Init ble client called");
    resetSleepDelay(SLEEP_EVERY_MS);
    hostBleDeInitEverything();
    bleClientConnected = false;
    BLEDevice::init(BLE_NAME);
    pSecurity = new BLESecurity();
    pSecurity->setCapability(ESP_IO_CAP_NONE);
    pSecurity->setAuthenticationMode(true, false, true);
    pSecurity->regenPassKeyOnConnect(true);
    BLEDevice::setSecurityCallbacks(new BLESecurityCallbacks());

    pBLEScan = BLEDevice::getScan();
    pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
    pBLEScan->setActiveScan(true);
    pBLEScan->setInterval(100);
    pBLEScan->setWindow(99);
    resetSleepDelay(SLEEP_EVERY_MS);
}

void hostBleStartScan(uint32_t durationSeconds)
{
    debugLog("Start ble scan called");
    pScannedDevices = pBLEScan->start(durationSeconds, false);
    debugLog("Start ble scan ended");
}

int hostBleGetScannedDevicesCount()
{
    if (pScannedDevices == NULL)
        return 0;
    return pScannedDevices->getCount();
}

String hostBleGetScannedDeviceName(int index)
{
    if (pScannedDevices == NULL)
        return "";
    BLEAdvertisedDevice device = pScannedDevices->getDevice(index);
    String name = device.getName().c_str();
    if (name == "")
    {
        name = device.getAddress().toString().c_str();
    }
    return name;
}

class hostBleMyClientCallback : public BLEClientCallbacks
{
    void onConnect(BLEClient *pclient)
    {
        debugLog("Connected to device");
        bleClientConnected = true;
    }

    void onDisconnect(BLEClient *pclient)
    {
        debugLog("Disconnected from device");
        bleClientConnected = false;
    }
};

void hostBleGeneralNotify(
    BLERemoteCharacteristic *pBLERemoteCharacteristic,
    uint8_t *pData,
    size_t length,
    bool isNotify)
{
    debugLog("Notify for " + String(pBLERemoteCharacteristic->getUUID().toString()) + " len: " + String(length));
    hostBleNotifyCallback(pBLERemoteCharacteristic, pData, length, isNotify);
}

bool hostBleConnectToDevice(int index)
{
    // Always set it
    hostBleClientName = hostBleGetScannedDeviceName(index);

    if (pScannedDevices == NULL)
        return false;

    BLEAdvertisedDevice device = pScannedDevices->getDevice(index);

    resetSleepDelay(SLEEP_EVERY_MS * 5);
    debugLog("Connecting to " + String(device.getAddress().toString()));

    if (hostBlePclient == NULL)
    {
        hostBlePclient = BLEDevice::createClient();
        hostBlePclient->setClientCallbacks(new hostBleMyClientCallback());
    }

    if (!hostBlePclient->connect(&device))
    {
        debugLog("Connect failed");
        return false;
    }

    hostBlePclient->setMTU(255);

    bool sec = hostBlePclient->secureConnection();
    debugLog("Secure: " + String(sec));

    resetSleepDelay(SLEEP_EVERY_MS);
    delayTask(2000);
    resetSleepDelay(SLEEP_EVERY_MS * 5);
    std::map<std::string, BLERemoteService *> *services = hostBlePclient->getServices();
    if (!services)
        return false;

    for (auto &svcEntry : *services)
    {
        resetSleepDelay(SLEEP_EVERY_MS);
        BLERemoteService *service = svcEntry.second;
        if (!service)
            continue;

        BLEUUID svcUUID = service->getUUID();
        debugLog("Service UUID: " + svcUUID.toString());

        std::map<std::string, BLERemoteCharacteristic *> *charMap = service->getCharacteristics();
        if (!charMap)
            continue;

        for (auto &charEntry : *charMap)
        {
            BLERemoteCharacteristic *pChar = charEntry.second;
            if (!pChar)
                continue;

            BLEUUID charUUID = pChar->getUUID();
            debugLog("Char UUID: " + charUUID.toString());

            if (pChar->canNotify() || pChar->canIndicate())
            {
                debugLog("Subscribing to updates: " + charUUID.toString());
                pChar->registerForNotify(hostBleGeneralNotify);
            }
        }
    }
    return true;
}

void hostBleDisconnectDevice()
{
    if (hostBlePclient != NULL && hostBlePclient->isConnected())
    {
        hostBlePclient->disconnect();
    }
}

#endif
