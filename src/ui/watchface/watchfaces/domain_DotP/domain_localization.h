#pragma once
#include "defines.h"

// Localization functions for Domain watchface
// Domain uses the same everything as InkField, becuase it's a cheap knockoff glitz to hell, which uses the same day name system as Slate, so we share those functions

// Re-use Slate localization functions for common elements
String getLocalizedDayName(int offset);  // From slate_localization.cpp - no default arg here
String getLocalizedTimeString(tmElements_t timeEl);  // From slate_localization.cpp
String getLocalizedAMPM(tmElements_t timeEl);  // From slate_localization.cpp

// Domain specific functions
String getDomainLocalizedError();
String getDomainLocalizedErrorShort();