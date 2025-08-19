#pragma once

#include "defines.h"

extern std::mutex serialWrite;
#if DEBUG
void initLogs();
void logFunction(String file, int line, String func, String message);
void logCleanup(bool serial = true, bool fs = true);
extern bool disableFsLogging;
#endif
void flushLogs(); // This should be moved inside of DEBUG but the code is messed up in other places, for now leave it
bool containsBelowChar(String str);
void delayTask(int timeMs);
void checkMaxMin(int *value, int max, int min = 0, bool wrapback = true);
String strTime(time_t unixTime);
String addZero(String str, uint8_t minimumLength);

float findHighest(float* numbers, int size);
float findLowest(float* numbers, int size);
void sortList(float* numbers, int size);
void concatenateFloatLists(float* sourceList1, int size1, float* sourceList2, int size2, float* destinationList);
float precision(float f, int places);
void setBoolMutex(std::mutex* theMutex, bool* theBool, bool boolValue);
int betterRandom(int min, int max);
int betterRandom(int max);

void setBit(uint8_t *num, uint8_t position, bool value);
bool getBit(uint8_t num, uint8_t position);
void toggleBit(uint8_t *num, uint8_t position);

void invertScreenColors();
void resetScreenColors();