#pragma once
#include "defines.h"

// Simplified language definitions using just country codes
#define EN   1  // English  
#define PL   2  // Polish
//#define DE   3  // German

// Main localization functions
String getLocalizedWeatherCondition(uint8_t weatherCode);
String getLocalizedDayName(int offset = 0);

// 12-hour format support (uses WATCHFACE_12H from config.h)
String getLocalizedTimeString(tmElements_t timeEl);
String getLocalizedAMPM(tmElements_t timeEl);