#include "bleMain.h"

#if BLE_ENABLED

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
}

void exitBle()
{
    pAdvertising->stop();
    pServer->disconnect(0);
    bleService->stop();
    BLEDevice::deinit(false);
    bleClientConnected = false;
}

// Class to handle security callbacks
class bleSecurityCallbacks : public BLESecurityCallbacks
{
    // Used to confirm a pin when using yes/no verification. Not used by us.
    bool onConfirmPIN(uint32_t pin)
    {
        debugLog("Confirm PIN: " + String(pin));
        // You can display the PIN on the e-paper display her is true
        return true; // Auto-confirm the PIN
    }

    // Used if we are the one requesting to bond with a device. This will be the passkey we send them. Not used by us.
    uint32_t onPassKeyRequest()
    {
        debugLog("PassKey Request");
        return 0;
    }

    void onPassKeyNotify(uint32_t pass_key)
    {
        debugLog("PassKey Notify: " + String(pass_key));
        // You can display the passkey on the e-paper display here
    }

    // This is also not used. not sure what it does.
    bool onSecurityRequest()
    {
        debugLog("Security Request");
        return false; // Auto-accept security request
    }

    void onAuthenticationComplete(esp_ble_auth_cmpl_t auth_cmpl)
    {
        if (auth_cmpl.success)
        {
            String success = (auth_cmpl.key_present ? "Yes" : "No");
            debugLog("Authentication successful. Bonded: " + success);
        }
        else
        {
            debugLog("Authentication failed. Reason: " + String(auth_cmpl.fail_reason));
        }
    }
};

void enableBonding()
{
    debugLog("Enabling BLE bonding");

    BLESecurity sec;
    // Set the security callbacks
    BLEDevice::setSecurityCallbacks(new bleSecurityCallbacks());
    BLEDevice::setEncryptionLevel(ESP_BLE_SEC_ENCRYPT_MITM);
    sec.setCapability(ESP_IO_CAP_OUT);
    sec.setAuthenticationMode(ESP_LE_AUTH_REQ_SC_MITM_BOND);

    // Set key size
    sec.setKeySize(16);

    // Set init key and response key
    sec.setInitEncryptionKey(ESP_BLE_ENC_KEY_MASK | ESP_BLE_ID_KEY_MASK);
    sec.setRespEncryptionKey(ESP_BLE_ENC_KEY_MASK | ESP_BLE_ID_KEY_MASK);
}

void removeBondedDevices()
{
    debugLog("Removing bonded devices");

    int dev_num = esp_ble_get_bond_device_num();
    if (dev_num == 0)
    {
        debugLog("No bonded devices found");
        return;
    }

    esp_ble_bond_dev_t *dev_list = (esp_ble_bond_dev_t *)malloc(sizeof(esp_ble_bond_dev_t) * dev_num);
    esp_ble_get_bond_device_list(&dev_num, dev_list);

    for (int i = 0; i < dev_num; i++)
    {
        esp_ble_remove_bond_device(dev_list[i].bd_addr);
    }

    free(dev_list);
    debugLog("All bonded devices removed");
}

#endif
