#pragma once

#include "defines.h"

#if GSR_WATCHFACES && GSR_STARFIELD
#include "../../watchfaces/gsr/starfield/GSRWatchFaceStarfield.h"
#endif
#if GSR_WATCHFACES && (GSR_CLASSICS_BASICS || GSR_CLASSICS_7SEG || GSR_CLASSICS_DOS || GSR_CLASSICS_POKE || GSR_CLASSICS_STARRY || GSR_CLASSICS_TETRIS || GSR_CLASSICS_MAC || GSR_CLASSICS_MARIO)
#include "../../watchfaces/gsr/classics/WatchyClassicsAddOn.h"
#endif
#if GSR_WATCHFACES && GSR_STATIONARY
#include "../../watchfaces/gsr/stationary/stationaryGSR.h"
#endif