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

uint8_t *mapMemory = NULL;
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
uint64_t totalSpeedSum = 0;
uint32_t speedCount = 0;

class speedCallBack : public BLECharacteristicCallbacks
{
    void onWrite(BLECharacteristic *pCharacteristic)
    {
        uint8_t *data = pCharacteristic->getData();
        float currentSpeed = ((data[0] << 8) | data[1]) / 10.0f;
        if (currentSpeed < 0.8)
        {
            currentSpeed = 0.0;
        }
        speed = currentSpeed;

        uint32_t currentTime = millisBetter();
        totalSpeedSum += (uint32_t)(speed * 10);
        speedCount++;

        debugLog("Speed is: " + String(speed));
    }
};

bool baikyBleClient = false;
void drawBleStatus()
{
    if (baikyBleClient == true)
    {
        writeImageN(175, 0, getImg("baiky/bleConnected"));
    }
    else
    {
        writeImageN(175, 0, getImg("baiky/bleDisconnected"));
    }
}

void drawSpeed()
{
    dis->fillRect(0, 0, 130, 38, SCWhite);
    setFont(getFont("baiky/UbuntuMono31"));
    String speedStr = String(speed);
    while (speedStr.length() < 4)
    {
        speedStr = "0" + speedStr;
    }
    while (speedStr.length() > 4)
    {
        speedStr.remove(speedStr.length() - 1);
    }
    while (speedStr.endsWith(".") && speedStr.length() > 0)
    {
        speedStr.remove(speedStr.length() - 1);
    }

    dis->fillRect(0, 0, 130, 40, SCWhite);
    writeTextReplaceBack(speedStr, 0, 38);

    float avgSpeed = (speedCount == 0) ? 0 : (float)totalSpeedSum / speedCount;
    avgSpeed /= 10.0f;

    String avgStr = String(avgSpeed);
    while (avgStr.length() > 3)
    {
        avgStr.remove(avgStr.length() - 1);
    }
    while (avgStr.endsWith(".") && avgStr.length() > 0)
    {
        avgStr.remove(avgStr.length() - 1);
    }

    setFont(getFont("UbuntuMono10"));
    dis->fillRect(130, 0, 30, 15, SCWhite);
    writeTextReplaceBack(avgStr, 130, 13);
    // debugLog("Average Speed: " + String(avgSpeed));

    writeImageN(130, 15, getImg("baiky/kmh"));
}

void initBaiky()
{
    debugLog("Init baiky called");
    saveCpuSpeed();
    setCpuSpeed(maxSpeed);
    initBle();
    bleService = pServer->createService(GPS_SERVICE_UUID);

    mapMemory = (uint8_t *)malloc(5000 * sizeof(uint8_t));
    memset(mapMemory, 0, 5000 * sizeof(uint8_t));

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

    if (baikyBleClient != bleClientConnected)
    {
        baikyBleClient = bleClientConnected;
        drawBleStatus();
        dUChange = true;
    }

    if (speed != speedOld)
    {
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
    free(mapMemory);
    mapMemory = NULL;
    restoreCpuSpeed();
}

#endif
