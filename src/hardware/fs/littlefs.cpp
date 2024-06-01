#include "littlefs.h"

bool fsInitDone = false;

bool fsSetup()
{
  if (fsInitDone == false)
  {
    debugLog("Trying to mount littleFS");
    if (LittleFS.begin(false, "/littlefs", 100, "littlefs") == false)
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
      debugLog("Little fs total bytes: " + String(LittleFS.totalBytes()));
      debugLog("Little fs used bytes: " + String(LittleFS.usedBytes()));
    }
  }
  return true;
}

void fsCreateDir(String path)
{
  if (fsSetup() == false)
  {
    return;
  }
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

int fsItemsInDir(String dir)
{
  if (fsSetup() == false)
  {
    return 0;
  }
  File root = LittleFS.open(dir);
  if (!root)
  {
    debugLog("Failed to open directory");
    return -1;
  }
  if (root.isDirectory() == false)
  {
    debugLog("Not a directory");
    return -1;
  }
  int count = 0;
  File file = root.openNextFile();
  while (file)
  {
    file = root.openNextFile();
    count = count + 1;
  }
  return count;
}

void fsAppendToFile(String path, String str)
{
  if (fsSetup() == false)
  {
    return;
  }
  File file = LittleFS.open(path, FILE_APPEND);
  if (!file)
  {
    debugLog("Failed to open file: " + path);
    return;
  }
  if (file.isDirectory() == true)
  {
    debugLog("Is a dir: " + path);
    return;
  }

  file.println(str);

  file.close();
}

size_t fsGetFileSize(String path)
{
  if (fsSetup() == false)
  {
    return 0;
  }
  File file = LittleFS.open(path);
  if (!file)
  {
    debugLog("Failed to open file for size: " + path);
    return 0;
  }
  if (file.isDirectory() == true)
  {
    debugLog("Is a dir: " + path);
    return 0;
  }
  size_t size = file.size();
  file.close();
  return size;
}

void fsRemoveFile(String path)
{
  if (fsSetup() == false)
  {
    return;
  }
  if (LittleFS.remove(path) == false)
  {
    debugLog("Failed to remove file: " + path);
  }
}

bool fsFileExists(String path)
{
  if (fsSetup() == false)
  {
    return false;
  }
  //bool ret = LittleFS.exists(path);
  File f = LittleFS.open(path);
  bool ret = false;
  // Madness, idk
  if (!f)
  {
    ret = false;
  } else {
    ret = true;
    f.close();
  }
  
  debugLog("File: " + path + " exists: " + BOOL_STR(ret));
  return ret;
}

#if DEBUG
void fsListDir(String dirname, uint8_t levels)
{
  if (fsSetup() == false)
  {
    return;
  }
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
    String theLog = "";
    if (file.isDirectory() == true)
    {
      theLog = "Dir: ";
    }
    else
    {
      theLog = "File: ";
    }
    theLog = theLog + String(file.name()) + " Size: " + String(file.size());
    time_t t = file.getLastWrite();
    struct tm *tmstruct = localtime(&t);

    char buffer[50] = {0};
    snprintf(buffer, sizeof(buffer), " Last write: %d-%02d-%02d %02d:%02d:%02d",
             (tmstruct->tm_year) + 1900, (tmstruct->tm_mon) + 1, tmstruct->tm_mday,
             tmstruct->tm_hour, tmstruct->tm_min, tmstruct->tm_sec);

    theLog = theLog + String(buffer);
    debugLog(theLog);
    if (file.isDirectory() && levels > 0)
    {
      debugLog("Going next level: " + String(levels));
      fsListDir(String(file.path()), levels - 1);
    }
    file.close();
    file = root.openNextFile();
  }
}

#endif