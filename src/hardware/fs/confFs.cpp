#include "littlefs.h"

#define STR_ERROR "Failed to setup fs"

bool fsIsConfig(String conf)
{
    if (fsSetup() == false)
    {
        debugLog("Failed to setup fs");
        return false;
    }
    File file = LittleFS.open("/conf/" + conf);
    if (file == false)
    {
        debugLog("There is no conf file: " + conf);
        return false;
    }
    file.close();
    return true;
}

String fsGetString(String conf, String defaultValue)
{
    if (fsSetup() == false)
    {
        debugLog("Failed to setup fs");
        fsSetString(conf, defaultValue);
        return defaultValue;
    }
    File file = LittleFS.open("/conf/" + conf);
    if (file == false)
    {
        debugLog("There is no conf file: " + conf);
        fsSetString(conf, defaultValue);
        return defaultValue;
    }
    int fileSize = file.size();
    uint8_t *buf = (uint8_t *)malloc(fileSize * sizeof(uint8_t));
    if (file.read(buf, fileSize) < 0)
    {
        debugLog("Failed to read the file: " + conf);
        fsSetString(conf, defaultValue);
        return defaultValue;
    }
    String str = String((char *)buf);
    str = str.substring(0, fileSize); // Garbage?
    debugLog("Conf file: " + conf + " value: " + str);
    file.close();
    return str;
}

void fsSetString(String conf, String value)
{
    if (fsSetup() == false)
    {
        debugLog("Failed to setup fs");
        return;
    }
    File file = LittleFS.open("/conf/" + conf, FILE_WRITE);
    if (file == false)
    {
        debugLog("Failed to set conf: " + conf);
        return;
    }
    if(file.print(value) == false) {
        debugLog("Failed to print to file " + conf + " value: " + value);
        return;
    }
    file.close();
}

bufSize fsGetBlob(String conf)
{
    if (fsSetup() == false)
    {
        debugLog("Failed to setup fs");
        return emptyBuff;
    }
    File file = LittleFS.open("/conf/" + conf);
    if (file == false)
    {
        debugLog("There is no conf file: " + conf);
        return emptyBuff;
    }
    int fileSize = file.size();
    uint8_t *buf = (uint8_t *)malloc(fileSize * sizeof(uint8_t));
    if (file.read(buf, fileSize) < 0)
    {
        debugLog("Failed to read the file: " + conf);
        return emptyBuff;
    }
    file.close();
    bufSize retBuf = {
        buf, fileSize
    };
    return retBuf;
}

void fsSetBlob(String conf, uint8_t* value, int size)
{
    if (fsSetup() == false)
    {
        debugLog("Failed to setup fs");
        return;
    }
    File file = LittleFS.open("/conf/" + conf, FILE_WRITE);
    if (file == false)
    {
        debugLog("Failed to set conf: " + conf);
        return;
    }
    if(file.write(value, size) == false) {
        debugLog("Failed to write blob to file " + conf);
        return;
    }
    file.close();
}