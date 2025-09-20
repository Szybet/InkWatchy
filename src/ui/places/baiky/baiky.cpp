#include "baiky.h"

#if BAIKY

// Define the UUIDs for the GPS service and characteristic
#define GPS_SERVICE_UUID "00001819-0000-1000-8000-00805f9b34fb"
#define MAP_UUID "04ec3c4b-66c3-4eeb-942e-0b7c20885b50"
#define SPEED_UUID "095ace66-89ee-44d7-87c0-3c56f7ec686a"
#define DONE_UUID "29eab2b3-792d-4d05-a2d9-68047fd2cf7c"

#if DEBUG
uint64_t mapWriteMillis = 0;
#endif

uint8_t mapMemory[5000] = {0};
uint16_t mapMemoryIndex = 0;
class mapCallBack : public BLECharacteristicCallbacks
{
    void onWrite(BLECharacteristic *pCharacteristic)
    {
#if DEBUG
        if (mapWriteMillis == 0)
        {
            mapWriteMillis = millisBetter();
            debugLog("Started streaming map");
        }
#endif
        // return;
        // debugLog("Map called");
        uint8_t *data = pCharacteristic->getData();
        uint16_t len = pCharacteristic->getLength();
        memcpy(&mapMemory[mapMemoryIndex], data, len);
        mapMemoryIndex += len;
    }
};

bool isDone = false;
BLECharacteristic *pCharacteristicDone = NULL;
class doneCallBack : public BLECharacteristicCallbacks
{
    void onWrite(BLECharacteristic *pCharacteristic)
    {
// debugLog("Is done called");
#if DEBUG
        debugLog("Done called, map write took: " + String(float(millisBetter() - mapWriteMillis) / 1000.0));
        mapWriteMillis = 0;
#endif
        isDone = true;
    }
};

float speed = 0;
float speedOld = 0;
class speedCallBack : public BLECharacteristicCallbacks
{
    void onWrite(BLECharacteristic *pCharacteristic)
    {
        uint8_t *data = pCharacteristic->getData();
        speed = ((data[0] << 8) | data[1]) / 10.0f;
        // speed = ((data[0] << 8) | data[1]);
        if(speed < 0.8) {
            speed = 0.0;
        }
        debugLog("Speed is: " + String(speed));
    }
};

bool baikyBleClient = false;
void drawBleStatus() {
    if(baikyBleClient == true) {
        writeImageN(175, 0, getImg("baiky/bleConnected"));
    } else {
        writeImageN(175, 0, getImg("baiky/bleDisconnected"));
    }
}

void drawSpeed() {
    dis->fillRect(0, 0, 130, 38, SCWhite);
    setFont(getFont("baiky/UbuntuMono31"));
    String speedStr = String(speed);
    while(speedStr.length() < 4) {
        speedStr = "0" + speedStr;
    }
    while(speedStr.length() > 4) {
        speedStr.remove(speedStr.length() - 1);
    }
    writeTextReplaceBack(speedStr, 0, 38);

    writeImageN(130, 15, getImg("baiky/kms"));
}

cpuSpeed bleCpuSpeed;
void initBaiky()
{
    debugLog("Init baiky called");
    bleCpuSpeed = getCpuSpeed();
    setCpuSpeed(maxSpeed);
    initBle();

    bleService = pServer->createService(GPS_SERVICE_UUID);

    {
        BLECharacteristic *pCharacteristic = bleService->createCharacteristic(
            MAP_UUID,
            BLECharacteristic::PROPERTY_WRITE | BLECharacteristic::PROPERTY_WRITE_NR);
        pCharacteristic->setCallbacks(new mapCallBack());
    }
    {
        pCharacteristicDone = bleService->createCharacteristic(
            DONE_UUID,
            BLECharacteristic::PROPERTY_WRITE | BLECharacteristic::PROPERTY_READ);
        pCharacteristicDone->setCallbacks(new doneCallBack());
        uint8_t data[1] = {0};
        pCharacteristicDone->setValue(data, sizeof(data));
    }
    {
        BLECharacteristic *pCharacteristic = bleService->createCharacteristic(
            SPEED_UUID,
            BLECharacteristic::PROPERTY_WRITE | BLECharacteristic::PROPERTY_WRITE_NR);
        pCharacteristic->setCallbacks(new speedCallBack());
    }

    startBle();
    drawSpeed();
    
    drawBleStatus();
    disUp(true);
}

void loopBaiky()
{
    useButton();

    if(baikyBleClient != bleClientConnected) {
        baikyBleClient = bleClientConnected;
        drawBleStatus();
        dUChange = true;
    }

    if(speed != speedOld) {
        speedOld = speed;
        drawSpeed();
        dUChange = true;
    }

    if (isDone == true)
    {
        debugLog("Updating");
        ImageDef image = ImageDef{200, 160, mapMemory};
        writeImageN(0, 40, &image, SCBlack, SCWhite);
        disUp(true);
        dUChange = false;

        isDone = false;
        uint8_t data[1] = {0};
        pCharacteristicDone->setValue(data, sizeof(data));
        mapMemoryIndex = 0;
    }

    disUp();
    resetSleepDelay();
}

void exitBaiky()
{
    exitBle();
    setCpuSpeed(bleCpuSpeed);
}

#endif