#pragma once
#include "defines.h"

// Main localization functions for Taychron
String getTaychronLocalizedTimeString(tmElements_t timeEl);
String getTaychronLocalizedAMPM(tmElements_t timeEl);

// Helper functions
String convertTo12HourFormat(String time24);
String getAMPM(String time24);