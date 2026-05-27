#pragma once

#include "defines.h"

#if WATCHFACE_ORBITAL

// Forward declaration
struct watchfaceDefOne;

// If you prefer automatic values, Orbital will use weather coordinates and the regular RTC subsystem. These are "overwrites"
#define ORBITAL_LAT 0.0f     // Latitude (decimal degrees) for moon
#define ORBITAL_LON 0.0f     // Longtitute (decimal degrees) for moon
#define ORBITAL_GMT_OFFSET 0 // Default GMT offset in hours (e.g., 1 for CET)

extern const watchfaceDefOne orbitalDef;

#endif
