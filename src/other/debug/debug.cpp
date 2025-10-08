#include "debug.h"
#include "rtcMem.h"

#if DEBUG
#if EINK_COUNTER
#define SKIP_CYCLES 7
bool randomChoosed = false;
int counterX;
int counterY;
int countSkip = SKIP_CYCLES;
int counter = 0;
void showEinkCounter()
{
    debugLog("countSkip: " + String(countSkip));
    if (countSkip == SKIP_CYCLES)
    {
        countSkip = 0;
        setTextSize(2);
        if (randomChoosed == false)
        {
            randomChoosed = true;
            counterX = betterRandom(50, 150);
            counterY = betterRandom(150, 180);
        }
        dis->setCursor(counterX, counterY);
        counter = counter + 1;
        debugLog("Showing einkCounter: " + String(counter));
        writeTextReplaceBack(String(counter), counterX, counterY);
        disUp(true);
    }
    countSkip = countSkip + 1;
}
#endif

#if DUMP_LOOP_ACC
void initAccLog()
{
    initAcc();
}

#if DUMP_LOOP_ACC_DIFF
Accel accOld;
#endif

#define MINIMUM_ACC_DIFF -1
String accGoodStr(int16_t newD, int16_t *old)
{
    String accv;
    int16_t diff = abs(abs(newD) - abs(*old));
    // debugLog("New: " + String(newD) + " old: " + String(*old));
    // debugLog("Diff is:" + String(diff));
    if (diff > MINIMUM_ACC_DIFF)
    {
        accv = String(newD);
        *old = newD;
    }
    else
    {
        accv = String(*old);
    }

    if (accv.length() == 1)
    {
        accv = " " + accv;
    }
    while (accv.length() < 3)
    {
        accv = accv + " ";
    }
    return accv;
}

void loopAccLog()
{
    Accel acc;
    rM.SBMA.getAccelPure(&acc);
#if !DUMP_LOOP_ACC_DIFF
    debugLog("Acc X: " + String(acc.x) + " Y: " + String(acc.y) + " Z: " + String(acc.z));
#else

    if (memcmp(&acc, &accOld, sizeof(Accel)) == 0)
    {
        delayTask(10);
    }
    else
    {
        String x = accGoodStr(acc.x, &accOld.x);
        String y = accGoodStr(acc.y, &accOld.y);
        String z = accGoodStr(acc.z, &accOld.z);
        // debugLog("Acc diff X: " + x + " Y: " + y + " Z: " + z);
        Serial.println(x + "," + y + "," + z);
    }
#endif
}
#endif

#if SCREENSHOT_ENDPOINT

AsyncWebServer ssServer(80);

#define SCREEN_WIDTH 200
#define SCREEN_HEIGHT 200

#define CMD_FIRST_HALF "curl -s -o /tmp/screen.bin http://"
#define CMD_SECOND_HALF "/screen && ffmpeg -y -f rawvideo -pix_fmt monob -video_size 200x200 -i /tmp/screen.bin /tmp/screen.png && kolourpaint /tmp/screen.png"

void screenshotEndpointInit()
{
    debugLog("Connecting to wifi because of ssServer");
    WiFi.begin(wifiCredStatic[0]->ssid, wifiCredStatic[0]->password);
    delayTask(3000);
    while(WiFi.status() != WL_CONNECTED) {
        delay(500);
        debugLog("Waiting for wifi...");
    }
    debugLog("Connected to wifi: " + WiFi.localIP().toString());
    debugLog("Command is: \"" + String(CMD_FIRST_HALF) + WiFi.localIP().toString() + String(CMD_SECOND_HALF) + "\"");

    ssServer.on("/screen", HTTP_GET, [](AsyncWebServerRequest *request) {
        debugLog("Raw buffer download requested");
        const size_t buffer_size = (SCREEN_WIDTH * SCREEN_HEIGHT) / 8;
        request->send(200, "application/octet-stream", dis->_buffer, buffer_size);
    });

    ssServer.begin();
}
#endif
#endif
