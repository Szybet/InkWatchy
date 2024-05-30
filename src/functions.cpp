#include "functions.h"

std::mutex serialWrite;
#if DEBUG // For not creating the huge list
int savedLogsIndex = 0;
char *savedLogs; // [LOG_SERIAL_BUFFER_SIZE] = {0};

std::mutex logFileWrite;
bool isLogFileOpened = false;
File logFile;
bool disableFsLogging = false; // We can't have trying to log to littlefs while trying to init it
int savedLogsFileIndex = 0;
char *savedLogsFile; // [LOG_FILE_BUFFER_SIZE] = {0};

void initLogs()
{
  savedLogs = (char *)malloc(LOG_SERIAL_BUFFER_SIZE * sizeof(char));
  savedLogsFile = (char *)malloc(LOG_FILE_BUFFER_SIZE * sizeof(char));
  memset(savedLogs, 0, LOG_SERIAL_BUFFER_SIZE);
  memset(savedLogsFile, 0, LOG_FILE_BUFFER_SIZE);
}

#define LF1 "/logs1.txt"
#define LF2 "/logs2.txt"
#define CONF_PREVIOUS_FILE "previousLogFile"
bool openLogFile()
{
  if (isLogFileOpened == false)
  {
    // this will lock it if it will not succed
    disableFsLogging = true;
    if (fsSetup() == false)
    {
      return false;
    }
    /*
    MAYBE IT DOESN'T LIKE THAT THIS FILE IS OPENED SECOND TIME IN ANOTHER THREAD HUH
    Processing backtrace: 0x4008eb2f:0x3ffcd8f0
0x4008eb2f:0x3ffcd8f0: /Users/ficeto/Desktop/ESP32/ESP32S2/esp-idf-public/components/freertos/port/xtensa/xtensa_context.S:194

Processing backtrace: 0x40082be1:0x3ffcd900
0x40082be1:0x3ffcd900: /Users/ficeto/Desktop/ESP32/ESP32S2/esp-idf-public/components/spi_flash/spi_flash_os_func_app.c:71

Processing backtrace: 0x4008eb2f:0x3ffcd920
0x4008eb2f:0x3ffcd920: /Users/ficeto/Desktop/ESP32/ESP32S2/esp-idf-public/components/freertos/port/xtensa/xtensa_context.S:194

Processing backtrace: 0x4008eb2f:0x3ffcd950
0x4008eb2f:0x3ffcd950: /Users/ficeto/Desktop/ESP32/ESP32S2/esp-idf-public/components/freertos/port/xtensa/xtensa_context.S:194

Processing backtrace: 0x4008eb2f:0x3ffcd970
0x4008eb2f:0x3ffcd970: /Users/ficeto/Desktop/ESP32/ESP32S2/esp-idf-public/components/freertos/port/xtensa/xtensa_context.S:194

Processing backtrace: 0x4008007d:0x3ffcd9a0
0x4008007d:0x3ffcd9a0: /Users/ficeto/Desktop/ESP32/ESP32S2/esp-idf-public/components/freertos/port/xtensa/xtensa_vectors.S:1802

Processing backtrace: 0x40082beb:0x3ffcd9c0
0x40082beb:0x3ffcd9c0: /Users/ficeto/Desktop/ESP32/ESP32S2/esp-idf-public/components/spi_flash/spi_flash_os_func_app.c:100

Processing backtrace: 0x40089315:0x3ffcd9e0
0x40089315:0x3ffcd9e0: /Users/ficeto/Desktop/ESP32/ESP32S2/esp-idf-public/components/spi_flash/esp_flash_api.c:139

Processing backtrace: 0x4008277a:0x3ffcda00
0x4008277a:0x3ffcda00: /Users/ficeto/Desktop/ESP32/ESP32S2/esp-idf-public/components/spi_flash/esp_flash_api.c:841

Processing backtrace: 0x40102c6b:0x3ffcda40
0x40102c6b:0x3ffcda40: /Users/ficeto/Desktop/ESP32/ESP32S2/esp-idf-public/components/spi_flash/partition.c:424

Processing backtrace: 0x4012e2f6:0x3ffcda70
0x4012e2f6:0x3ffcda70: /Users/ficeto/Desktop/ESP32/ESP32S2/esp32-arduino-lib-builder/components/esp_littlefs/src/littlefs_api.c:21

Processing backtrace: 0x4012e6c1:0x3ffcdaa0
0x4012e6c1:0x3ffcdaa0: /Users/ficeto/Desktop/ESP32/ESP32S2/esp32-arduino-lib-builder/components/esp_littlefs/src/littlefs/lfs.c:116

Processing backtrace: 0x4012e95d:0x3ffcdad0
0x4012e95d:0x3ffcdad0: /Users/ficeto/Desktop/ESP32/ESP32S2/esp32-arduino-lib-builder/components/esp_littlefs/src/littlefs/lfs.c:1009

Processing backtrace: 0x4012efc6:0x3ffcdb70
0x4012efc6:0x3ffcdb70: /Users/ficeto/Desktop/ESP32/ESP32S2/esp32-arduino-lib-builder/components/esp_littlefs/src/littlefs/lfs.c:1394

Processing backtrace: 0x40131c95:0x3ffcdbd0
0x40131c95:0x3ffcdbd0: /Users/ficeto/Desktop/ESP32/ESP32S2/esp32-arduino-lib-builder/components/esp_littlefs/src/littlefs/lfs.c:3605

Processing backtrace: 0x4012d800:0x3ffcdc20
0x4012d800:0x3ffcdc20: /Users/ficeto/Desktop/ESP32/ESP32S2/esp32-arduino-lib-builder/components/esp_littlefs/src/esp_littlefs.c:1350

Processing backtrace: 0x4010a9be:0x3ffcdd50
0x4010a9be:0x3ffcdd50: /Users/ficeto/Desktop/ESP32/ESP32S2/esp-idf-public/components/vfs/vfs.c:580 (discriminator 3)

Processing backtrace: 0x4018b791:0x3ffcdd70
0x4018b791:0x3ffcdd70: /builds/idf/crosstool-NG/.build/xtensa-esp32-elf/src/newlib/newlib/libc/syscalls/sysstat.c:11

Processing backtrace: 0x40183c0f:0x3ffcdd90
0x40183c0f:0x3ffcdd90: /home/szybet/.platformio/packages/framework-arduinoespressif32@3.20011.230801/libraries/FS/src/vfs_api.cpp:44

Processing backtrace: 0x400f46f5:0x3ffcde20
0x400f46f5:0x3ffcde20: /home/szybet/.platformio/packages/framework-arduinoespressif32@3.20011.230801/libraries/FS/src/FS.cpp:234

Processing backtrace: 0x400f4715:0x3ffcde50
0x400f4715:0x3ffcde50: /home/szybet/.platformio/packages/framework-arduinoespressif32@3.20011.230801/libraries/FS/src/FS.cpp:225

Processing backtrace: 0x400d7eda:0x3ffcde70
0x400d7eda:0x3ffcde70: /mnt/data/projects/git/InkWatchy/src/hardware/fs/littlefs.cpp:123

Processing backtrace: 0x400d3413:0x3ffcdef0
0x400d3413:0x3ffcdef0: /mnt/data/projects/git/InkWatchy/src/functions.cpp:38 (discriminator 1)

Processing backtrace: 0x400d3910:0x3ffcdf90
0x400d3910:0x3ffcdf90: /mnt/data/projects/git/InkWatchy/src/functions.cpp:160

Processing backtrace: 0x400d5678:0x3ffcdff0
0x400d5678:0x3ffcdff0: /mnt/data/projects/git/InkWatchy/src/hardware/buttons.cpp:99 (discriminator 16)

Processing backtrace: 0x400d57f9:0x3ffce080
0x400d57f9:0x3ffce080: /mnt/data/projects/git/InkWatchy/src/hardware/buttons.cpp:148
    */
    String logFilePath = LF1;
    if (fsGetFileSize(LF1) > MAX_LOG_FILE_SIZE_BYTES)
    {
      if (fsGetFileSize(LF2) > MAX_LOG_FILE_SIZE_BYTES)
      {
        String previousFile = fsGetString(CONF_PREVIOUS_FILE, LF1);
        if (previousFile == LF1)
        {
          fsRemoveFile(LF2);
          logFilePath = LF2;
        }
        else if (previousFile == LF2)
        {
          fsRemoveFile(LF1);
          logFilePath = LF1;
        }
        else
        {
          debugLog("Something is wrong with log files");
        }
      }
      else
      {
        logFilePath = LF2;
      }
    }
    else
    {
      logFilePath = LF1;
    }
    fsSetString(CONF_PREVIOUS_FILE, logFilePath);

    // FILE_APPEND
    // FILE_WRITE
    /*
    String fileOpenMode = "w";
    if (fsFileExists(logFilePath) == true)
    {
      fileOpenMode = "a";
    }
    */

    logFile = LittleFS.open(logFilePath, FILE_APPEND);
    if (!logFile)
    {
      debugLog("Failed to open logs");
      return false;
    }
    if (logFile.isDirectory() == true)
    {
      debugLog("how");
      return false;
    }
    isLogFileOpened = true;
    disableFsLogging = false;
  }
  return true;
}

void flushSavedLogs()
{
  if (savedLogsIndex > 0)
  {
    // debugLog("Printing out saved logs");
    Serial.print(savedLogs);
    Serial.flush(true);
    savedLogsIndex = 0;
    memset(savedLogs, 0, LOG_SERIAL_BUFFER_SIZE);
  }
}

void logCleanup()
{
  if (disableFsLogging == false)
  {
    if (savedLogsFileIndex > 0)
    {
      logFile.print(savedLogsFile);
    }
    logFile.close();
    isLogFileOpened = false;
  }
  flushSavedLogs();
}

void logFunction(String file, int line, String func, String message)
{
  String log = file + ":" + String(line) + " " + func + ": " + message + "\n";
  int logLength = log.length();
  if (serialWrite.try_lock())
  {
    Serial.flush(true);
    flushSavedLogs();
    Serial.print(log);
    Serial.flush(true);
    serialWrite.unlock();
#if SCOM_TASK_ENABLED
    printEndPacket = true;
#endif
  }
  else
  {
    if (savedLogsIndex + logLength < LOG_SERIAL_BUFFER_SIZE)
    {
      // log.toCharArray(savedLogs, logLength, savedLogsIndex);
      // strcpy(savedLogs + savedLogsIndex, log.c_str());
      strncpy(savedLogs + savedLogsIndex, log.c_str(), logLength);
      savedLogsIndex += logLength;
    }
  }
#if PUT_LOGS_TO_FS
  if (disableFsLogging == false)
  {
    logFileWrite.lock();
    if (savedLogsFileIndex + logLength < LOG_FILE_BUFFER_SIZE)
    {
      // log.toCharArray(savedLogsFile, logLength, savedLogsFileIndex);
      // strcpy(savedLogsFile + savedLogsFileIndex, log.c_str());
      strncpy(savedLogsFile + savedLogsFileIndex, log.c_str(), logLength);
      savedLogsFileIndex += logLength;
    }
    else
    {
      if (openLogFile() == true)
      {
        // Serial.println("\nFile logs: " + String(savedLogsFile) + "\n End of file logs\n");
        logFile.print(savedLogsFile);
        logFile.print(log);
        logFile.close();
        isLogFileOpened = false;
        // logFile.println("Saved to file: " + String(savedLogsFileIndex) + " bytes");
        savedLogsFileIndex = 0;
        memset(savedLogsFile, 0, LOG_FILE_BUFFER_SIZE);
      }
    }
    logFileWrite.unlock();
  }
#endif
}
#endif

// Check if a function contains a character that has a line below like... g p q j
bool containsBelowChar(String str)
{
  if (str.indexOf("g") != -1 || str.indexOf("p") != -1 || str.indexOf("q") != -1 || str.indexOf("j") != -1 || str.indexOf("y") != -1 || str.indexOf("_") != -1)
  {
    return true;
  }
  else
  {
    return false;
  }
}

void delayTask(int timeMs)
{
  TickType_t xDelay = (TickType_t)timeMs;
  xDelay = xDelay / portTICK_PERIOD_MS;
  // debugLog("Going to sleep for " + String(timeMs) + " Ms. In ticks that is: " + String(xDelay));
  vTaskDelay(xDelay);
}

void checkMaxMin(int *value, int max, int min, bool wrapback)
{
  if (*value > max)
  {
    debugLog("The value is max");
    if (wrapback == false)
    {
      *value = max;
    }
    else
    {
      *value = min;
    }
  }
  if (*value < min)
  {
    debugLog("The value is min");
    if (wrapback == false)
    {
      *value = min;
    }
    else
    {
      *value = max;
    }
  }
}

String strTime(time_t unixTime)
{
  unixTime += TIME_OFFSET_S;
  return ctime(&unixTime);
}

float findHighest(float *numbers, int size)
{
  if (size == 0)
  {
    return 0.0;
  }
  float highest = numbers[0];
  for (int i = 1; i < size; ++i)
  {
    if (numbers[i] > highest)
    {
      highest = numbers[i];
    }
  }
  return highest;
}

float findLowest(float *numbers, int size)
{
  if (size == 0)
  {
    return 0.0;
  }
  float lowest = numbers[0];
  for (int i = 1; i < size; ++i)
  {
    if (numbers[i] < lowest)
    {
      lowest = numbers[i];
    }
  }
  return lowest;
}

void sortList(float *numbers, int size)
{
  for (int i = 0; i < size - 1; ++i)
  {
    for (int j = 0; j < size - i - 1; ++j)
    {
      if (numbers[j] > numbers[j + 1])
      {
        float temp = numbers[j];
        numbers[j] = numbers[j + 1];
        numbers[j + 1] = temp;
      }
    }
  }
}

// use sizeof(sourceList1) / sizeof(sourceList1[0]) + sizeof(sourceList2) / sizeof(sourceList2[0]) for the size of the destination list, if you don't have pointers of the lists yet
void concatenateFloatLists(float *sourceList1, int size1, float *sourceList2, int size2, float *destinationList)
{
  for (int i = 0; i < size1; ++i)
  {
    destinationList[i] = sourceList1[i];
  }

  for (int i = 0; i < size2; ++i)
  {
    destinationList[size1 + i] = sourceList2[i];
  }
}

// https://stackoverflow.com/questions/70221264/how-do-i-set-the-precision-of-a-float-variable-in-c
float precision(float f, int places)
{
  float n = std::pow(10.0f, places);
  return std::round(f * n) / n;
}

void setBoolMutex(std::mutex *theMutex, bool *theBool, bool boolValue)
{
  theMutex->lock();
  *theBool = boolValue;
  theMutex->unlock();
}