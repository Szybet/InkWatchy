#include "terrain.h"

#if WATCHFACE_TERRAIN
#include "rtcMem.h"

#define SYS_CORD_X 127
#define SYS_CORD_Y 147

void drawMarker1Terrain(uint16_t c, bool active)
{
    dis->fillRect(SYS_CORD_X, SYS_CORD_Y, 21, 10, SCWhite);
    if (c != SCWhite)
    {
        if (active)
        {
            writeImageN(SYS_CORD_X, SYS_CORD_Y, getImg("terrain/sysOk"));
        }
        else
        {
            writeImageN(SYS_CORD_X, SYS_CORD_Y, getImg("terrain/sysOff"));
        }
    }
}

void drawMarker2Terrain(uint16_t c, bool active)
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

void drawMarker3Terrain(uint16_t c, bool active)
{
    drawMarker2Terrain(c, active);
    dis->drawFastHLine(188, 159, 5, c);
    dis->drawFastHLine(189, 160, 4, c);
    dis->drawFastHLine(190, 161, 3, c);
    dis->drawFastHLine(191, 162, 2, c);
    dis->drawFastHLine(192, 163, 1, c);
}

void cleanMarkersTerrain()
{
    drawMarker1Terrain(SCWhite, true);
    drawMarker2Terrain(SCWhite, true);
    drawMarker3Terrain(SCWhite, true);
}

void drawPosMarkerTerrain()
{
    // First clean all Pos markers
    cleanMarkersTerrain();

    // Then draw the one
    if (rM.terrain.watchfacePos == EMPTY_POS)
    {
        drawMarker1Terrain(SCBlack, rM.terrain.positionEngaged);
    }
    else if (rM.terrain.watchfacePos == MODULE_POS)
    {
        drawMarker2Terrain(SCBlack, rM.terrain.positionEngaged);
    }
    if (rM.terrain.watchfacePos == MODULE_ENG_POS)
    {
        drawMarker3Terrain(SCBlack, rM.terrain.positionEngaged);
    }
    dUChange = true;
}

void movePosTerrain(int add)
{
    rM.terrain.watchfacePos = rM.terrain.watchfacePos + add;
    checkMaxMin(&rM.terrain.watchfacePos, WATCHFACE_POSITIONS - 1);
    drawPosMarkerTerrain();
}

void terrainManageInput(buttonState bt)
{
    {
        if (rM.terrain.watchfacePos == MODULE_ENG_POS && rM.terrain.positionEngaged == true && bt != None)
        {
            debugLog("Module eng entered");
            if (bt != LongMenu)
            {
                wfModulesManage(bt);
            }
            else
            {
                debugLog("Dis engage mogule eng pos");
                rM.terrain.positionEngaged = false;
                drawPosMarkerTerrain();
            }
            return;
        }
        switch (bt)
        {
        case Up:
        {
            if (rM.terrain.positionEngaged == false)
            {
                movePosTerrain(1);
            }
            else
            {
                if (rM.terrain.watchfacePos == MODULE_POS)
                {
                    wfModuleSwitch(Right);
                    drawModuleCount();
                }
            }
            break;
        }
        case Down:
        {
            if (rM.terrain.positionEngaged == false)
            {
                movePosTerrain(-1);
            }
            else
            {
                if (rM.terrain.watchfacePos == MODULE_POS)
                {
                    wfModuleSwitch(Left);
                    drawModuleCount();
                }
            }
            break;
        }
        case Menu:
        {
            debugLog("rM.terrain.watchfacePos: " + String(rM.terrain.watchfacePos));
            if (rM.terrain.watchfacePos == EMPTY_POS)
            {
                generalSwitch(mainMenu);
            }
            else if (rM.terrain.watchfacePos == MODULE_POS)
            {
                wfModulesManage(Menu);
            }
            break;
        }
        case LongUp:
        {
            if (rM.terrain.watchfacePos == MODULE_POS)
            {
                wfModulesManage(LongUp);
            }
            break;
        }
        case LongDown:
        {
            break;
        }
        case LongMenu:
        {
            rM.terrain.positionEngaged = !rM.terrain.positionEngaged;
            drawPosMarkerTerrain();
            break;
        }
#if LONG_BACK_FULL_REFRESH
        case LongBack:
        {
            debugLog("Because of LONG_BACK_FULL_REFRESH in watchface, I shall now refresh the screen fully");
            updateDisplay(FULL_UPDATE);
            break;
        }
#endif
        default:
        {
            break;
        }
        }
    }
}

#endif
