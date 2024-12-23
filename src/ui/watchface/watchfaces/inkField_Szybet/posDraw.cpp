#include "inkput.h"
#include "rtcMem.h"

#if WATCHFACE_INKFIELD_SZYBET

void drawMarker1(uint16_t c, bool active)
{
    dis->drawPixel(108, 60, c);
    dis->drawPixel(109, 60, c);
    dis->drawPixel(108, 61, c);
    if (active)
    {
        // dis->drawFastHLine(106, 58, 6, c);
        dis->drawFastHLine(107, 59, 4, c);
        // dis->drawFastVLine(106, 58, 6, c);
        dis->drawFastVLine(107, 59, 4, c);
    }
}

void drawMarker2(uint16_t c, bool active)
{
    dis->drawPixel(191, 195, c);
    dis->drawPixel(191, 196, c);
    dis->drawPixel(190, 196, c);
    if (active)
    {
        dis->drawLine(189, 196, 191, 194, c);
        dis->drawLine(188, 196, 191, 193, c);
    }
}

void drawMarker3(uint16_t c, bool active)
{
    drawMarker2(c, active);
    dis->drawFastHLine(188, 159, 5, c);
    dis->drawFastHLine(189, 160, 4, c);
    dis->drawFastHLine(190, 161, 3, c);
    dis->drawFastHLine(191, 162, 2, c);
    dis->drawFastHLine(192, 163, 1, c);
}

void cleanMarkers()
{
    drawMarker1(GxEPD_WHITE, true);
    drawMarker2(GxEPD_WHITE, true);
    drawMarker3(GxEPD_WHITE, true);
}

void drawPosMarker()
{
    // First clean all Pos markers
    cleanMarkers();

    // Then draw the one
    if (rM.inkfield.watchfacePos == EMPTY_POS)
    {
        drawMarker1(GxEPD_BLACK, rM.inkfield.positionEngaged);
    }
    else if (rM.inkfield.watchfacePos == MODULE_POS)
    {
        drawMarker2(GxEPD_BLACK, rM.inkfield.positionEngaged);
    } if (rM.inkfield.watchfacePos == MODULE_ENG_POS)
    {
        drawMarker3(GxEPD_BLACK, rM.inkfield.positionEngaged);
    }
    dUChange = true; 
}

#endif