#include "minimalDefines.h"
#if GSR_WATCHFACES && GSR_STARFIELD

#ifndef WATCHY_STARFIELD_H
#define WATCHY_STARFIELD_H
#include <math.h>
#include "Dusk2Dawn.h"
#include "moonPhaser.h"
#include "Seven_Segment10pt7b.h"
#include "DSEG7_Classic_Regular_15.h"
#include "DSEG7_Classic_Bold_25.h"
#include "DSEG7_Classic_Bold_53.h"
#include "DSEG7_Classic_Regular_39.h"
#include "icons.h"
#include "GxEPD2.h"
#include "Watchy_GSR.h"

class WatchyStarfield {
    public:
        void handleButtonPress(uint8_t SwitchNumber);
        void drawWatchFace();
        void drawTime();
        void drawDate();
        void drawSteps();
        void drawBattery();
        void drawField();
        void drawMoon();
        void drawSun();
        void drawWeather();
};

#endif
#endif