#pragma once

#include "defines.h"

#if WATCHFACE_ORBITAL

// Forward declaration
struct watchfaceDefOne;

// Orbital watchface - concentric rings displaying time, moon phase, and sunrise/sunset
// To configure for your location, set these in src/defines/config.h:
//   #define ORBITAL_LAT 51.5074f       // Your latitude (decimal degrees)
//   #define ORBITAL_LON -0.1278f       // Your longitude (decimal degrees)
//   #define ORBITAL_GMT_OFFSET 0       // Your GMT offset in hours (e.g., 1 for CET, -5 for EST)

extern const watchfaceDefOne orbitalDef;

#endif
