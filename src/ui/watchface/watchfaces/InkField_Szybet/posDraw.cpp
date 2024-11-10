#include "inkput.h"

void drawMarker1(uint16_t c, bool active)
{
    display.drawPixel(108, 60, c);
    display.drawPixel(109, 60, c);
    display.drawPixel(108, 61, c);
    if (active)
    {
        // display.drawFastHLine(106, 58, 6, c);
        display.drawFastHLine(107, 59, 4, c);
        // display.drawFastVLine(106, 58, 6, c);
        display.drawFastVLine(107, 59, 4, c);
    }
}

void drawMarker2(uint16_t c, bool active)
{
    display.drawPixel(191, 195, c);
    display.drawPixel(191, 196, c);
    display.drawPixel(190, 196, c);
    if (active)
    {
        display.drawLine(189, 196, 191, 194, c);
        display.drawLine(188, 196, 191, 193, c);
    }
}

void drawMarker3(uint16_t c, bool active)
{
    drawMarker2(c, active);
    display.drawFastHLine(188, 159, 5, c);
    display.drawFastHLine(189, 160, 4, c);
    display.drawFastHLine(190, 161, 3, c);
    display.drawFastHLine(191, 162, 2, c);
    display.drawFastHLine(192, 163, 1, c);
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
    if (watchfacePos == EMPTY_POS)
    {
        drawMarker1(GxEPD_BLACK, positionEngaged);
    }
    else if (watchfacePos == MODULE_POS)
    {
        drawMarker2(GxEPD_BLACK, positionEngaged);
    } if (watchfacePos == MODULE_ENG_POS)
    {
        drawMarker3(GxEPD_BLACK, positionEngaged);
    }
    dUChange = true; 
}