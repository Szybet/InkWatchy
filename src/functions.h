#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "defines/defines.h"

void logFunction(String file, int line, String func, String message);
bool containsBelowChar(String str);
void delayTask(int timeMs);
void checkMaxMin(int *value, int max, int min = 0);
String strTime(time_t unixTime);

#endif
