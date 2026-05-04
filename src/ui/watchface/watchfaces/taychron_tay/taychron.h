#pragma once

#include "defines.h"

#if WATCHFACE_TAYCHRON

// This is a forward declaration
struct watchfaceDefOne;

#include "taychron_localization.h"

String getAMPM(String time24);

const extern watchfaceDefOne taychronDef;

#endif
