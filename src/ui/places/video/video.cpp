#include "video.h"

#if VIDEO_PLAYER

String videoName = "";

bool ready = false;
bool firstImg = true;
int frame = 1;
uint8_t *image1 = NULL;
uint8_t *image2 = NULL;

void getBuf(void *parameter)
{
    bufSize buff = fsGetBlob(String(frame), "/videos/" + videoName + "/");
    if (buff.size <= 0)
    {
        debugLog("Size invalid, probably end of video, restarting");
        frame = 1;
        ready = true;
        vTaskDelete(NULL);
    }
    if (firstImg == true)
    {
        if (image1 != NULL)
        {
            free(image1);
            image1 = NULL;
        }
        image1 = buff.buf;
        memcpy(dis->_buffer, image1, 5000);
    }
    else
    {
        if (image2 != NULL)
        {
            free(image2);
            image2 = NULL;
        }
        image2 = buff.buf;
        memcpy(dis->_buffer, image2, 5000);
    }
    frame = frame + 1;
    ready = true;
    vTaskDelete(NULL);
}

TaskHandle_t bufTask;
void launchBuf()
{
    xTaskCreate(
        getBuf,
        "bufTask",
        4000,
        NULL,
        21,
        &bufTask);
}

void initVideoPlayer()
{
    setCpuSpeed(cpuSpeed::maxSpeed);
    frame = 1;
    launchBuf();
    dis->setCursor(0, 0);
}

int64_t timer1;
int64_t timer2;
int64_t timer3;
int64_t timer4;

void loopVideoPlayer()
{
    timer4 = millisBetter();
    while (ready == false)
    {
        delayTask(5);
    }
    debugLog("Waited for ready: " + String(millisBetter() - timer4));
    timer1 = millisBetter();
    ready = false;
    firstImg = !firstImg;
    launchBuf();

    // timer2 = millisBetter();
    timer3 = millisBetter();
    /*
    if (!firstImg == true)
    {
        // dis->writeImage(image1, 0, 0, 200, 200);
        // memcpy(dis->_buffer, image1, 5000);
        // timer3 = millisBetter();
        dis->display(PARTIAL_UPDATE);
    }
    else
    {
        // dis->writeImage(image2, 0, 0, 200, 200);
        // memcpy(dis->_buffer, image2, 5000);
        // timer3 = millisBetter();
        dis->display(PARTIAL_UPDATE);
    }
    */
    dis->display(PARTIAL_UPDATE);
    debugLog("Display took: " + String(millisBetter() - timer3));
    // debugLog("New loop took: " + String(millisBetter() - timer2));
    debugLog("Whole frame took: " + String(millisBetter() - timer1));
    // watchdogPing();
    resetSleepDelay();
}

void exitVideoPlayer()
{
    while (ready == false)
    {
        delayTask(5);
    }
    if (image2 != NULL)
    {
        free(image2);
        image2 = NULL;
    }
    if (image1 != NULL)
    {
        free(image1);
        image1 = NULL;
    }
    updateDisplay(FULL_UPDATE);
}

#endif