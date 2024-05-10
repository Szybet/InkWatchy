#include "littlefs.h"

bool fsInitDone = false;

void setupFsManager()
{
    if (fsInitDone == false)
    {
        debugLog("Trying to mount littleFS");
        if (fs.begin(false, "/lfs", 20, "spiffs") == false)
        {
            debugLog("Failed to mount littlefs");
            return;
        } else {
            debugLog("Mounted little fs");
            listDir("/", 0);
            fsInitDone = true;
        }
    }
}

#if DEBUG
void listDir(String dirname, uint8_t levels)
{
  debugLog("Listing directory: " + dirname);

  File root = fs.open(dirname);
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
  serialWrite.lock();
  while (file)
  {
    if (file.isDirectory())
    {
      Serial.print("  DIR : ");

      Serial.print(file.name());
      time_t t = file.getLastWrite();
      struct tm *tmstruct = localtime(&t);
      Serial.printf(
          "  LAST WRITE: %d-%02d-%02d %02d:%02d:%02d\n", (tmstruct->tm_year) + 1900, (tmstruct->tm_mon) + 1, tmstruct->tm_mday, tmstruct->tm_hour,
          tmstruct->tm_min, tmstruct->tm_sec);

      if (levels)
      {
        listDir(file.name(), levels - 1);
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
  serialWrite.unlock();
}
#endif