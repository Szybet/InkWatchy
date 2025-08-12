#include "gadgetbridge.h"
#include "rtcMem.h"

#if GADGETBRIDGE_ENABLED

BLECharacteristic *txHandle = NULL;

void notify(JsonDocument doc)
{
  String jsonBuffer;
  serializeJson(doc, jsonBuffer);
  jsonBuffer = "\r\n" + jsonBuffer + "\r\n";
  debugLog("Sending notification: " + jsonBuffer);
  txHandle->setValue(jsonBuffer);
  txHandle->notify();
}

class gadgetbridgeServerCallbacks : public BLEServerCallbacks
{
  void onConnect(BLEServer *pServer)
  {
    debugLog("BLE client connected");
    bleClientConnected = true;

    BLEDevice::getAdvertising()->stop();
    rM.ble_connection_attempts = 0;
    resetSleepDelay();
  }
  void onDisconnect(BLEServer *pServer)
  {
    debugLog("BLE client disconnected");
    pServer->getAdvertising()->start();
    bleClientConnected = false;
  }
};

class rxCallback : public BLECharacteristicCallbacks
{

  String dataBuffer = "";

  void onWrite(BLECharacteristic *pCharacteristic)
  {
    uint8_t *data = pCharacteristic->getData();
    uint16_t length = pCharacteristic->getLength();

    dataBuffer += String((char *)data, length);

    // Find command start marker
    int startPos = dataBuffer.indexOf('\x10');
    if (startPos == -1)
    {
      // No start marker found, clear buffer and exit
      dataBuffer = "";
      return;
    }

    // Find command end marker
    int endPos = dataBuffer.indexOf('\n', startPos);
    if (endPos == -1)
    {
      // No end marker found, keep data in buffer for next time
      // But remove anything before the start marker
      dataBuffer = dataBuffer.substring(startPos);
      return;
    }

    // Extract the complete command
    String command = dataBuffer.substring(startPos + 1, endPos);
    debugLog("Received command: " + command);

    // Remove processed command from buffer
    dataBuffer = dataBuffer.substring(endPos + 1);

    // Check if it's a GB command
    if (command.startsWith("GB(") && command.endsWith(")"))
    {
      // Extract JSON content
      String jsonContent = command.substring(3, command.length() - 1);

      // Process the JSON content
      processGBCommand(jsonContent);
      return;
    }

#if GADGETBRIDGE_SYNC_TIME
    // Check for setTime command
    long long timestamp;
    if (sscanf(command.c_str(), "setTime(%lld)", &timestamp) == 1)
    {
      debugLog("setTime command received with timestamp:" + String(timestamp));
      setRTCTime(timestamp);
    }
    int timezonePos = command.indexOf("setTimeZone(");

    if (timezonePos == -1)
    {
      debugLog("No setTimeZone command found");
      return;
    }

    // Check for setTimeZone command
    float timezoneOffset;
    if (sscanf(command.substring(timezonePos).c_str(), "setTimeZone(%f)", &timezoneOffset) == 1)
    {
      debugLog("setTimeZone command received with offset:" + String(timezoneOffset));
      int tzInt = static_cast<int>(timezoneOffset);
      setRTCTimeZoneByUtcOffset(tzInt);
    }
#endif

    resetSleepDelay();
  }

  void processGBCommand(String jsonContent)
  {
    // Here you would parse the JSON and handle different command types
    debugLog("Received GB command: " + jsonContent);

    // // Example: Use ArduinoJson to parse the content
    // JsonDocument doc;
    // DeserializationError error = deserializeJson(doc, jsonContent);

    // if (!error)
    // {
    //   // Process the command based on its type
    //   if (doc.containsKey("t"))
    //   {
    //     String type = doc["t"];

    //     if (type == "notify")
    //     {
    //       // Handle notification
    //       String title = doc["title"];
    //       String body = doc["body"];
    //       // Show notification on watch
    //     }
    //     // Handle other command types...
    //   }
    // }
  }
};

void nvsInit(void)
{
  // Initialize NVS
  esp_err_t ret = nvs_flash_init();

  if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
  {
    // NVS partition was truncated and needs to be erased
    // Retry nvs_flash_init
    debugLog("NVS partition needs to be erased. Erasing...");
    ESP_ERROR_CHECK(nvs_flash_erase());
    ret = nvs_flash_init();
  }

  // Check if the final initialization was successful
  ESP_ERROR_CHECK(ret);

  debugLog("NVS initialized successfully.");
}

void gadgetbridgeInit()
{
  const char *NUS_UUID = "6E400001-B5A3-F393-E0A9-E50E24DCCA9E";
  const char *NUS_RX_UUID = "6E400002-B5A3-F393-E0A9-E50E24DCCA9E";
  const char *NUS_TX_UUID = "6E400003-B5A3-F393-E0A9-E50E24DCCA9E";

  debugLog("Init Gadgetbridge called");

  nvsInit();

  initBle("Bangle.js InkWatchy"); // Bangle.js prefix is important for the Gadgetbridge app
  pServer->setCallbacks(new gadgetbridgeServerCallbacks());

  // Enable bonding with PIN display (true) or without (false)
  enableBonding();

  bleService = pServer->createService(NUS_UUID); // NUS server ID
  {
    BLECharacteristic *pCharacteristic = bleService->createCharacteristic(
        NUS_RX_UUID,
        BLECharacteristic::PROPERTY_WRITE);
    pCharacteristic->setCallbacks(new rxCallback());
    pCharacteristic->setAccessPermissions(ESP_GATT_PERM_WRITE_ENC_MITM);
  }

  {
    BLECharacteristic *pCharacteristic = bleService->createCharacteristic(
        NUS_TX_UUID,
        BLECharacteristic::PROPERTY_NOTIFY);
    pCharacteristic->setAccessPermissions(ESP_GATT_PERM_READ_ENC_MITM);

    txHandle = pCharacteristic;
  }

  startBle();
}

#endif
