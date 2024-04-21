#include "functions.h"

// This needs to be here for some reason
// This clears the warning about const char to chat
// This will be an issue when updating smallrtc maybe
#ifdef RTCC_VERSION
#undef RTCC_VERSION
#define RTCC_VERSION (char*)"Pcf8563 v1.0.3"
#endif

std::mutex serialWrite;
#if DEBUG // For not creating the huge list
int savedLogsIndex = 0;
char savedLogs[2000] = {0};
bool areLogsSaved = false;

void logFunction(String file, int line, String func, String message)
{
  String log = file + ":" + String(line) + " " + func + ": " + message + "\n";
  if (serialWrite.try_lock())
  {
    Serial.flush(true);
    if (areLogsSaved == true)
    {
      areLogsSaved = false;
      //debugLog("Printing out saved logs");
      Serial.print(savedLogs);
      Serial.flush(true);
      savedLogsIndex = 0;
    }
    Serial.print(log);
    Serial.flush(true);
    serialWrite.unlock();
#if SCOM_TASK_ENABLED
  printEndPacket = true;
#endif
  }
  else
  {
    if (savedLogsIndex + log.length() < 2000)
    {
      strcpy(savedLogs + savedLogsIndex, log.c_str());
      savedLogsIndex += log.length();
      areLogsSaved = true;
    }
  }
}
#endif

// Check if a function contains a character that has a line below like... g p q j
bool containsBelowChar(String str)
{
  if (str.indexOf("g") > 0 || str.indexOf("p") > 0 || str.indexOf("q") > 0 || str.indexOf("j") > 0 || str.indexOf("y") > 0)
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