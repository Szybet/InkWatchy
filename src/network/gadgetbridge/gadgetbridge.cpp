#include "gadgetbridge.h"

static const char *NUS_UUID = "6E400001-B5A3-F393-E0A9-E50E24DCCA9E";
static const char *NUS_RX_UUID = "6E400002-B5A3-F393-E0A9-E50E24DCCA9E";
static const char *NUS_TX_UUID = "6E400003-B5A3-F393-E0A9-E50E24DCCA9E";

class rxCallback : public BLECharacteristicCallbacks
{
  void onWrite(BLECharacteristic *pCharacteristic)
  {
    uint8_t *data = pCharacteristic->getData();
    uint16_t length = pCharacteristic->getLength();
    debugLog("Received data: " + String((char *)data, length));
  }
};

BLECharacteristic *txHandle = NULL;

void initGadgetbridge()
{
  debugLog("Init Gadgetbridge called");

  initBle("Bangle.js InkWatchy"); // Bangle.js prefix is important for the Gadgetbridge app

  pServer->createService(NUS_UUID); // NUS server ID
  {
    BLECharacteristic *pCharacteristic = bleService->createCharacteristic(
        NUS_RX_UUID,
        BLECharacteristic::PROPERTY_WRITE | BLECharacteristic::PROPERTY_WRITE_NR);
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
