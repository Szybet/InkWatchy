#pragma once

#include "defines.h"

#if WATCHFACE_TAYCHRON

// This is a forward declaration
struct watchfaceDefOne;

String convertTo12HourFormat(String time24);
String getAMPM(String time24);

const extern watchfaceDefOne taychronDef;

#endif
