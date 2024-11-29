#include "functions.h"

std::mutex serialWrite;
#if DEBUG // For not creating the huge list
#if PUT_LOGS_TO_SERIAL
int savedLogsIndex = 0;
char *savedLogs; // [LOG_SERIAL_BUFFER_SIZE] = {0};
#endif

std::mutex logFileWrite;
File logFile;
bool disableFsLogging = false; // We can't have trying to log to littlefs while trying to init it
int savedLogsFileIndex = 0;
char *savedLogsFile; // [LOG_FILE_BUFFER_SIZE] = {0};

void initLogs()
{
#if PUT_LOGS_TO_SERIAL
  savedLogs = (char *)malloc(LOG_SERIAL_BUFFER_SIZE * sizeof(char));
  memset(savedLogs, 0, LOG_SERIAL_BUFFER_SIZE);
#endif
#if PUT_LOGS_TO_FS
  savedLogsFile = (char *)malloc(LOG_FILE_BUFFER_SIZE * sizeof(char));
  memset(savedLogsFile, 0, LOG_FILE_BUFFER_SIZE);
#endif
}

#if PUT_LOGS_TO_FS
#define LF1 "/logs1.txt"
#define LF2 "/logs2.txt"
#define CONF_PREVIOUS_FILE "previous_log"
bool openLogFile()
{
  // this will lock it if it will not succed
  disableFsLogging = true;
  if (fsSetup() == false)
  {
    return false;
  }

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
  disableFsLogging = false;
  return true;
}
#endif

void logCleanup(bool serial, bool fs)
{
#if PUT_LOGS_TO_FS
  if (fs == true)
  {
    if (disableFsLogging == false)
    {
      if (openLogFile() == true)
      {
        if (savedLogsFileIndex > 0)
        {
          // This function here can output old logs, mostly from FsSetup
          logFile.print(savedLogsFile);
          savedLogsFileIndex = 0;
          memset(savedLogsFile, 0, LOG_FILE_BUFFER_SIZE);
        }
        logFile.close();
      }
    }
    disableFsLogging = true;
  }
#endif
#if PUT_LOGS_TO_SERIAL
  if (serial == true)
  {
    if (savedLogsIndex > 0)
    {
      // debugLog("Printing out saved logs");
      Serial.print(savedLogs);
      flushLogs();
      savedLogsIndex = 0;
      memset(savedLogs, 0, LOG_SERIAL_BUFFER_SIZE);
    }
  }
#endif
}

void logFunction(String file, int line, String func, String message)
{
#if MINIMAL_LOGS
  String log = String(line) + ": " + message + "\n";
#else
  String log = file + ":" + String(line) + " " + func + ": " + message + "\n";
#endif

  int logLength = log.length();
#if PUT_LOGS_TO_SERIAL
  if (serialWrite.try_lock())
  {
    flushLogs();
    logCleanup(true, false);
    Serial.print(log);
    flushLogs();
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
#endif
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

void flushLogs()
{
#if DEBUG
#if ATCHY_VER == WATCHY_2 || ATCHY_VER == WATCHY_1 || ATCHY_VER == WATCHY_1_5
  Serial.flush(true);
#elif ATCHY_VER == WATCHY_3
  Serial.flush();
#endif
#if SERIAL_LOG_DELAY
  delayTask(SERIAL_LOG_DELAY_MS);
#endif
#endif
}

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
  // unixTime += TIME_OFFSET_S;
  return String(ctime(&unixTime));
}

String addZero(String str, uint8_t minimumLength) {
  String str_tmp = str;
  while(str_tmp.length() < minimumLength) {
    str_tmp = "0" + str_tmp;
  }
  return str_tmp;
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