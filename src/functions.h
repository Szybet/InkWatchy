#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "defines/defines.h"

void logFunction(String file, int line, String func, String message);
bool containsBelowChar(String str);
void delayTask(int timeMs);
void checkMaxMin(int *value, int max, int min = 0, bool wrapback = true);
String strTime(time_t unixTime);

float findHighest(float* numbers, int size);
float findLowest(float* numbers, int size);
void sortList(float* numbers, int size);
void concatenateFloatLists(float* sourceList1, int size1, float* sourceList2, int size2, float* destinationList);

#endif
