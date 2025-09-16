#pragma once

#include "rtc.h"
#include "../../reUse/menu/menu.h"

#define TZ_COUNT 25 // UTC -12 to UTC +12

// Entry point functions for the timezone menu
void enterSetTimezoneWatch();
void loopSetTimezoneWatch();
void exitSetTimezoneWatch();
