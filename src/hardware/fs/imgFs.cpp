#include "littlefs.h"

String loadedImgNames[IMG_COUNT];
ImageDef loadedImg[IMG_COUNT];

ImageDef *getImg(String name)
{
    if (setupFsManager() == false)
    {
        debugLog("Failed to setup fs");
        return &emptyImgPack;
    }
    uint8_t emptyListIndex = 0;
    for (int i = 0; i < IMG_COUNT; i++)
    {
        if (name == loadedImgNames[i])
        {
            return &loadedImg[i];
        }
        else
        {
            if (loadedImgNames[i] == "")
            {
                emptyListIndex = i;
                break;
            }
        }
    }
    File file = LittleFS.open("/img/" + name);
    if (file == false)
    {
        debugLog("File is not available: " + name);
        return &emptyImgPack;
    }
    int fileSize = file.size();
    debugLog("file size: " + String(fileSize));
    if (fileSize <= 0)
    {
        debugLog("This file has size 0: " + name);
    }
    uint8_t *imgBuf = (uint8_t *)malloc(fileSize * sizeof(uint8_t));
    if (file.read(imgBuf, fileSize) < 0)
    {
        debugLog("Failed to read the file: " + name);
        return &emptyImgPack;
    }
    // Now the inf file
    File fileInf = LittleFS.open("/img/" + name + "Inf");
    if (fileInf == false)
    {
        debugLog("File is not available: " + name + "Inf");
        return &emptyImgPack;
    }
    int fileInfSize = fileInf.size();
    uint8_t *InfBuf = (uint8_t *)malloc(fileInfSize * sizeof(uint8_t));
    if (fileInf.read(InfBuf, fileInfSize) < 0)
    {
        debugLog("Failed to read the file: " + name + "Inf");
        return &emptyImgPack;
    }
    String str = String((char *)InfBuf);
    debugLog("Bare inf str is: " + str);
    int newlinePos = str.indexOf('\n');
    int16_t width = str.substring(0, newlinePos).toInt();
    int16_t height = str.substring(newlinePos + 1).toInt();
    free(InfBuf);
    ImageDef newImg = {
        imgBuf,
        width,
        height,
    };
    loadedImgNames[emptyListIndex] = name;
    loadedImg[emptyListIndex] = newImg;
    return &loadedImg[emptyListIndex];
}

int16_t getImgWidth(String name)
{
    return getImg(name)->bw;
}

int16_t getImgHeight(String name)
{
    return getImg(name)->bh;
}