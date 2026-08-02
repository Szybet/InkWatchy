#include "cyber.h"

#if WATCHFACE_CYBER
#include "rtcMem.h"

#define SYS_CORD_X 127
#define SYS_CORD_Y 147

void drawMarker1Cyber(uint16_t c, bool active)
{
    dis->fillRect(SYS_CORD_X, SYS_CORD_Y, 21, 10, SCWhite);
    if (c != SCWhite)
    {
        if (active)
        {
            writeImageN(SYS_CORD_X, SYS_CORD_Y, getImg("cyber/sysOk"));
        }
        else
        {
            writeImageN(SYS_CORD_X, SYS_CORD_Y, getImg("cyber/sysOff"));
        }
    }
}

void drawMarker2Cyber(uint16_t c, bool active)
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

void drawMarker3Cyber(uint16_t c, bool active)
{
    drawMarker2Cyber(c, active);
    dis->drawFastHLine(188, 159, 5, c);
    dis->drawFastHLine(189, 160, 4, c);
    dis->drawFastHLine(190, 161, 3, c);
    dis->drawFastHLine(191, 162, 2, c);
    dis->drawFastHLine(192, 163, 1, c);
}

void cleanMarkersCyber()
{
    drawMarker1Cyber(SCWhite, true);
    drawMarker2Cyber(SCWhite, true);
    drawMarker3Cyber(SCWhite, true);
}

void drawPosMarkerCyber()
{
    // First clean all Pos markers
    cleanMarkersCyber();

    // Then draw the one
    if (rM.cyber.watchfacePos == EMPTY_POS)
    {
        drawMarker1Cyber(SCBlack, rM.cyber.positionEngaged);
    }
    else if (rM.cyber.watchfacePos == MODULE_POS)
    {
        drawMarker2Cyber(SCBlack, rM.cyber.positionEngaged);
    }
    if (rM.cyber.watchfacePos == MODULE_ENG_POS)
    {
        drawMarker3Cyber(SCBlack, rM.cyber.positionEngaged);
    }
    dUChange = true;
}

void movePosCyber(int add)
{
    rM.cyber.watchfacePos = rM.cyber.watchfacePos + add;
    checkMaxMin(&rM.cyber.watchfacePos, WATCHFACE_POSITIONS - 1);
    drawPosMarkerCyber();
}

void cyberManageInput(buttonState bt)
{
    {
        if (rM.cyber.watchfacePos == MODULE_ENG_POS && rM.cyber.positionEngaged == true && bt != None)
        {
            debugLog("Module eng entered");
            if (bt != LongMenu)
            {
                wfModulesManage(bt);
            }
            else
            {
                debugLog("Dis engage mogule eng pos");
                rM.cyber.positionEngaged = false;
                drawPosMarkerCyber();
            }
            return;
        }
        switch (bt)
        {
        case Up:
        {
            if (rM.cyber.positionEngaged == false)
            {
                movePosCyber(1);
            }
            else
            {
                if (rM.cyber.watchfacePos == MODULE_POS)
                {
                    wfModuleSwitch(Right);
                    drawModuleCount();
                }
            }
            break;
        }
        case Down:
        {
            if (rM.cyber.positionEngaged == false)
            {
                movePosCyber(-1);
            }
            else
            {
                if (rM.cyber.watchfacePos == MODULE_POS)
                {
                    wfModuleSwitch(Left);
                    drawModuleCount();
                }
            }
            break;
        }
        case Menu:
        {
            debugLog("rM.cyber.watchfacePos: " + String(rM.cyber.watchfacePos));
            if (rM.cyber.watchfacePos == EMPTY_POS)
            {
                generalSwitch(mainMenu);
            }
            else if (rM.cyber.watchfacePos == MODULE_POS)
            {
                wfModulesManage(Menu);
            }
            break;
        }
        case LongUp:
        {
            if (rM.cyber.watchfacePos == MODULE_POS)
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
            rM.cyber.positionEngaged = !rM.cyber.positionEngaged;
            drawPosMarkerCyber();
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
