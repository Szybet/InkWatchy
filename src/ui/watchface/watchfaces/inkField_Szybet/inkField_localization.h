#pragma once
#include "defines.h"

// Localization functions for InkField watchface
// InkField uses the same day name system as Slate, so we share those functions

// Re-use Slate localization functions for common elements
String getLocalizedDayName(int offset);  // From slate_localization.cpp - no default arg here
String getLocalizedTimeString(tmElements_t timeEl);  // From slate_localization.cpp
String getLocalizedAMPM(tmElements_t timeEl);  // From slate_localization.cpp

// InkField specific functions
String getInkFieldLocalizedError();
String getInkFieldLocalizedErrorShort();