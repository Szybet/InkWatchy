#include "gadgetbridge.h"
#include "rtcMem.h"

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

BLECharacteristic *txHandle = NULL;

void initGadgetbridge()
{
  const char *NUS_UUID = "6E400001-B5A3-F393-E0A9-E50E24DCCA9E";
  const char *NUS_RX_UUID = "6E400002-B5A3-F393-E0A9-E50E24DCCA9E";
  const char *NUS_TX_UUID = "6E400003-B5A3-F393-E0A9-E50E24DCCA9E";

  debugLog("Init Gadgetbridge called");

  initBle("Bangle.js InkWatchy"); // Bangle.js prefix is important for the Gadgetbridge app

  // Enable bonding with PIN display (true) or without (false)
  enableBonding();

  bleService = pServer->createService(NUS_UUID); // NUS server ID
  {
    BLECharacteristic *pCharacteristic = bleService->createCharacteristic(
        NUS_RX_UUID,
        BLECharacteristic::PROPERTY_WRITE);
    pCharacteristic->setCallbacks(new rxCallback());
  }

  {
    BLECharacteristic *pCharacteristic = bleService->createCharacteristic(
        NUS_TX_UUID,
        BLECharacteristic::PROPERTY_NOTIFY);
    txHandle = pCharacteristic;
  }

  startBle();
}
