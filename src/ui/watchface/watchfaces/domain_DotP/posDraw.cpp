#include "dotput.h"
#include "rtcMem.h"

#if WATCHFACE_DOMAIN_DOTP

void domainDrawMarker1(uint16_t c, bool active)
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

void domainDrawMarker2(uint16_t c, bool active)
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

void domainDrawMarker3(uint16_t c, bool active)
{
    domainDrawMarker2(c, active);
    dis->drawFastHLine(188, 159, 5, c);
    dis->drawFastHLine(189, 160, 4, c);
    dis->drawFastHLine(190, 161, 3, c);
    dis->drawFastHLine(191, 162, 2, c);
    dis->drawFastHLine(192, 163, 1, c);
}

void domainCleanMarkers()
{
    domainDrawMarker1(SCWhite, true);
    domainDrawMarker2(SCWhite, true);
    domainDrawMarker3(SCWhite, true);
}

void domainDrawPosMarker()
{
    // First clean all Pos markers
    domainCleanMarkers();

    // Then draw the one
    if (rM.domain.watchfacePos == EMPTY_POS)
    {
        domainDrawMarker1(SCBlack, rM.domain.positionEngaged);
    }
    else if (rM.domain.watchfacePos == MODULE_POS)
    {
        domainDrawMarker2(SCBlack, rM.domain.positionEngaged);
    } if (rM.domain.watchfacePos == MODULE_ENG_POS)
    {
        domainDrawMarker3(SCBlack, rM.domain.positionEngaged);
    }
    dUChange = true; 
}

#endif