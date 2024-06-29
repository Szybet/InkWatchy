#include "littlefs.h"

char loadedImgNames[IMG_COUNT][RESOURCES_NAME_LENGTH] = {0};
ImageDef loadedImg[IMG_COUNT];

ImageDef *getImg(String name)
{
    if (fsSetup() == false)
    {
        debugLog("Failed to setup fs");
        return &emptyImgPack;
    }
    uint8_t emptyListIndex = 0;
    for (int i = 0; i < IMG_COUNT; i++)
    {
        String loadedImgString = String(loadedImgNames[i]);
        //debugLog("char test: \"" + loadedImgString + "\"");
        if (name == loadedImgString)
        {
            return &loadedImg[i];
        }
        else
        {
            if (loadedImgString == "")
            {
                emptyListIndex = i;
                //debugLog("current img final index: " + String(i));
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
    //debugLog("file size: " + String(fileSize));
    if (fileSize <= 0)
    {
        debugLog("This file has size 0: " + name);
    }
    uint8_t *imgBuf = (uint8_t *)malloc(fileSize * sizeof(uint8_t));
    if (file.read(imgBuf, fileSize) == 0)
    {
        debugLog("Failed to read the file: " + name);
        return &emptyImgPack;
    }
    file.close();
    // Now the inf file
    File fileInf = LittleFS.open("/img/" + name + "Inf");
    if (fileInf == false)
    {
        debugLog("File is not available: " + name + "Inf");
        return &emptyImgPack;
    }
    int fileInfSize = fileInf.size();
    //debugLog("File inf size: " + String(fileInfSize));
    uint8_t *InfBuf = (uint8_t *)malloc(fileInfSize * sizeof(uint8_t));
    if (fileInf.read(InfBuf, fileInfSize) == 0)
    {
        debugLog("Failed to read the file: " + name + "Inf");
        return &emptyImgPack;
    }
    fileInf.close();
    String str = String((char *)InfBuf);
    str = str.substring(0, fileInfSize); // Because there is some garbage?
    //debugLog("Bare inf str is: " + str);
    int newlinePos = str.indexOf('\n');
    int16_t width = str.substring(0, newlinePos).toInt();
    int16_t height = str.substring(newlinePos + 1).toInt();
    free(InfBuf);
    ImageDef newImg = {
        imgBuf,
        width,
        height,
    };
    //loadedImgNames[emptyListIndex] = name;
    int nameLength = name.length();
#if DEBUG
    if(nameLength > RESOURCES_NAME_LENGTH) {
        debugLog("Resource name: " + name + " is too big because RESOURCES_NAME_LENGTH. Buffer overflow.");
    }
#endif
    strncpy(loadedImgNames[emptyListIndex], name.c_str(), nameLength);
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