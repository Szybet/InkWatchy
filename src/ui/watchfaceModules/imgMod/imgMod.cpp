#include "imgMod.h"
#include "rtcMem.h"

#if IMAGE_MODULE

#include <esp32/rom/crc.h>

void wfImagecheckShow(bool *showBool, bool *redrawBool)
{
    *showBool = true;
#if IMG_MODULE_CHANGE_EVERY_HOUR
    if (timeRTCLocal.Hour != rM.imageCurrentHour)
    {
        rM.imageCurrentHour = timeRTCLocal.Hour;
        *redrawBool = true;
    }
    else
    {
        *redrawBool = false;
    }
#else
    *redrawBool = false;
#endif
    debugLog("wfImage redrawBool: " + String(*redrawBool));
}

#define MAX_TRIES 3
uint8_t tries = 0;

void redrawModuleImage()
{
    tries = 0;
    u8_t c = 0;
    {
        File root = LittleFS.open("/img/" + String(IMAGE_MODULE_PATH));
        File file = root.openNextFile();
        while (file)
        {
            if (file.isDirectory() == false)
            {
                c = c + 1;
            }
            file = root.openNextFile();
        }
        root.close();
        file.close();
    }
    debugLog("There are so many module images: " + String(c));
    u8_t finalImageIndex = betterRandom(1, c + 1);
    debugLog("We want the image: " + String(finalImageIndex));
    u8_t cFinal = 0;
    {
        File root = LittleFS.open("/img/" + String(IMAGE_MODULE_PATH));
        File file = root.openNextFile();
        while (file)
        {
            if (file.isDirectory() == false)
            {
                cFinal = cFinal + 1;
                if (cFinal == finalImageIndex)
                {
                    uint32_t romCRC = (~crc32_le((uint32_t) ~(0xffffffff), (const uint8_t *)file.name(), strlen(file.name()))) ^ 0xffffffff;
                    debugLog("Got new crc: " + String(romCRC) + " from file: " + String(file.name()));
                    if (rM.imageNameCrc32 == romCRC && tries < MAX_TRIES)
                    {
                        tries = tries + 1;
                        root.close();
                        file.close();
                        redrawModuleImage();
                        return;
                    }
                    rM.imageNameCrc32 = romCRC;
                    squareInfo modSq = getWatchModuleSquare();
                    writeImageN(modSq.cord.x, modSq.cord.y, getImg(IMAGE_MODULE_PATH + String(file.name())));
                    dUChange = true;
                }
            }
            file = root.openNextFile();
        }
        root.close();
        file.close();
    }
}

void wfImagerequestShow(buttonState button, bool *showBool)
{
    redrawModuleImage();
}

#endif