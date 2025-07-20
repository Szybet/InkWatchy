#include "littlefs.h"

char loadedImgNames[IMG_COUNT][RESOURCES_NAME_LENGTH] = {0};
ImageDef loadedImg[IMG_COUNT];
uint8_t *loadedImgPointers[IMG_COUNT] = {NULL};
uint8_t loadedImgIndex = 0;

ImageDef *getImg(String name)
{
    if (fsSetup() == false)
    {
        debugLog("Failed to setup fs");
        return &emptyImgPack;
    }
    int8_t emptyListIndex = -1;
    for (int i = 0; i < IMG_COUNT; i++)
    {
        if (strcmp(loadedImgNames[i], name.c_str()) == 0)
        {
            debugLog("Image " + name + " was cached");
            return &loadedImg[i];
        }
        else
        {
            if (strlen(loadedImgNames[i]) == 0)
            {
                emptyListIndex = i;
                // debugLog("current img final index: " + String(i));
                break;
            }
        }
    }

    if (emptyListIndex == -1)
    {
        debugLog("Image count exceeded, freeing memory");
        loadedImgIndex = loadedImgIndex + 1;
        if (loadedImgIndex >= IMG_COUNT)
        {
            loadedImgIndex = 0;
        }
        debugLog("Freeing memory at index: " + String(loadedImgIndex));
        free(loadedImgPointers[loadedImgIndex]);
        loadedImgPointers[loadedImgIndex] = NULL;
        memset(loadedImgNames[loadedImgIndex], '\0', RESOURCES_NAME_LENGTH); // If something goes wrong along the lines, make sure we don't free it 2 times
        emptyListIndex = loadedImgIndex;
    }

    File file = LittleFS.open("/img/" + name);
    if (file == false)
    {
        debugLog("File is not available: " + name);
        return &emptyImgPack;
    }
    int fileSize = file.size();
    // debugLog("file size: " + String(fileSize));
    if (fileSize <= 0)
    {
        debugLog("This file has size 0: " + name);
        return &emptyImgPack;
    }
    uint8_t *imgBuf = (uint8_t *)malloc(fileSize * sizeof(uint8_t));
    if (file.read(imgBuf, fileSize) == 0)
    {
        debugLog("Failed to read the file: " + name);
        return &emptyImgPack;
    }
    file.close();

    ImageDef newImg = *(ImageDef *)imgBuf;
    newImg.bitmap = imgBuf + sizeof(newImg.bh) + sizeof(newImg.bw);
    // debugLog("newImg name: " + name);
    // debugLog("newImg bw: " + String(newImg.bw));
    // debugLog("newImg bh: " + String(newImg.bh));

    int nameLength = name.length();
#if DEBUG
    if (nameLength > RESOURCES_NAME_LENGTH)
    {
        debugLog("Resource name: " + name + " is too big because RESOURCES_NAME_LENGTH. Buffer overflow.");
    }
#endif

    debugLog("Image loaded on index: " + String(emptyListIndex));
    loadedImgIndex = emptyListIndex;
    loadedImgPointers[emptyListIndex] = imgBuf;
    memset(loadedImgNames[emptyListIndex], '\0', RESOURCES_NAME_LENGTH); // To be sure comparison works
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

void cleanImgCache()
{
    loadedImgIndex = 0;
    memset(loadedImgNames, 0, sizeof(loadedImgNames));
    memset(loadedImg, 0, sizeof(loadedImg));
    for (int i = 0; i < IMG_COUNT; i++)
    {   
        if(loadedImgPointers[i] != NULL) {
            free(loadedImgPointers[i]);
            loadedImgPointers[i] = NULL;
        }
    }
}