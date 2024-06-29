#include "littlefs.h"

#define STR_ERROR "Failed to setup fs"

/*
// Just use default value
bool fsIsConfig(String conf, String dir)
{
    if (fsSetup() == false)
    {
        debugLog("Failed to setup fs");
        return false;
    }
    File file = LittleFS.open(dir + conf);
    if (file == false)
    {
        debugLog("There is no conf file: " + conf);
        return false;
    }
    file.close();
    return true;
}
*/

String fsGetString(String conf, String defaultValue, String dir)
{
    if (fsSetup() == false)
    {
        debugLog("Failed to setup fs");
        return defaultValue;
    }
    File file = LittleFS.open(dir + conf);
    if (file == false)
    {
        debugLog("There is no conf file: " + conf);
        fsSetString(conf, defaultValue, dir);
        return defaultValue;
    }
    int fileSize = file.size();
    uint8_t *buf = (uint8_t *)malloc(fileSize * sizeof(uint8_t));
    if (file.read(buf, fileSize) == 0)
    {
        debugLog("Failed to read the file: " + conf);
        fsSetString(conf, defaultValue, dir);
        return defaultValue;
    }
    String str = String((char *)buf);
    str = str.substring(0, fileSize); // Garbage?
    debugLog("Conf file: " + conf + " value: " + str);
    file.close();
    return str;
}

bool fsSetString(String conf, String value, String dir)
{
    if (fsSetup() == false)
    {
        debugLog("Failed to setup fs");
        return false;
    }
    File file = LittleFS.open(dir + conf, FILE_WRITE);
    if (file == false)
    {
        debugLog("Failed to set conf: " + conf);
        return false;
    }
    if(file.print(value) == false) {
        debugLog("Failed to print to file " + conf + " value: " + value);
        return false;
    }
    file.close();
    return true;
}

bufSize fsGetBlob(String conf, String dir)
{
    if (fsSetup() == false)
    {
        debugLog("Failed to setup fs");
        return emptyBuff;
    }
    File file = LittleFS.open(dir + conf);
    if (file == false)
    {
        debugLog("There is no conf file: " + conf);
        return emptyBuff;
    }
    int fileSize = file.size();
    uint8_t *buf = (uint8_t *)malloc(fileSize * sizeof(uint8_t));
    if (file.read(buf, fileSize) == 0)
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

void fsSetBlob(String conf, uint8_t* value, int size, String dir)
{
    if (fsSetup() == false)
    {
        debugLog("Failed to setup fs");
        return;
    }
    File file = LittleFS.open(dir + conf, FILE_WRITE);
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