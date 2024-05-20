#include "littlefs.h"

bool fsInitDone = false;

bool fsSetup()
{
  if (fsInitDone == false)
  {
    debugLog("Trying to mount littleFS");
    if (LittleFS.begin(false, "/littlefs", 65) == false)
    {
      debugLog("Failed to mount littlefs");
      return false;
    }
    else
    {
      debugLog("Mounted little fs");
      fsInitDone = true;

      for (int i = 0; i < IMG_COUNT; i++)
      {
        loadedImgNames[i] = "";
      }

      for (int i = 0; i < FONT_COUNT; i++)
      {
        loadedFontNames[i] = "";
      }

      fsCreateDir("/conf");
      isDebug(fsListDir("/", 0));
    }
  }
  return true;
}

void fsCreateDir(String path)
{
  if (LittleFS.mkdir(path) <= 0)
  {
    debugLog("Failed to create dir: " + path);
  }
}

/*
File *fsOpenFile(String path)
{
  File file = LittleFS.open(path);
  if (file.isDirectory() == true)
  {
    debugLog("This file is a dir: " + path);
    return NULL;
  }
  if (file == false)
  {
    debugLog("File is not available:" + path);
    return NULL;
  }
  return &file;
}
*/

#if DEBUG
void fsListDir(String dirname, uint8_t levels)
{
  debugLog("Listing directory: " + dirname);

  File root = LittleFS.open(dirname);
  if (!root)
  {
    debugLog("Failed to open directory");
    return;
  }
  if (root.isDirectory() == false)
  {
    debugLog("Not a directory");
    return;
  }

  File file = root.openNextFile();
  while (file)
  {
    if (file.isDirectory())
    {
      Serial.print("  DIR : ");

      Serial.print(file.path());
      time_t t = file.getLastWrite();
      struct tm *tmstruct = localtime(&t);
      Serial.printf(
          "  LAST WRITE: %d-%02d-%02d %02d:%02d:%02d\n", (tmstruct->tm_year) + 1900, (tmstruct->tm_mon) + 1, tmstruct->tm_mday, tmstruct->tm_hour,
          tmstruct->tm_min, tmstruct->tm_sec);

      if (levels > 0)
      {
        debugLog("Going next level: " + String(levels));
        fsListDir(String(file.path()), levels - 1);
      }
    }
    else
    {
      Serial.print("  FILE: ");
      Serial.print(file.name());
      Serial.print("  SIZE: ");

      Serial.print(file.size());
      time_t t = file.getLastWrite();
      struct tm *tmstruct = localtime(&t);
      Serial.printf(
          "  LAST WRITE: %d-%02d-%02d %02d:%02d:%02d\n", (tmstruct->tm_year) + 1900, (tmstruct->tm_mon) + 1, tmstruct->tm_mday, tmstruct->tm_hour,
          tmstruct->tm_min, tmstruct->tm_sec);
    }
    file = root.openNextFile();
  }
}

#endif