#include "cyber.h"

#if WATCHFACE_CYBER
#include "rtcMem.h"

void cyberDrawModulePos(uint8_t color, bool active)
{
    dis->drawLine(186, 184, 186, 189, color);
    dis->drawLine(186, 189, 191, 189, color);
    dis->drawLine(191, 189, 186, 184, color);
    if (active)
    {
        dis->drawLine(187, 186, 187, 188, color);
        dis->drawLine(187, 188, 189, 188, color);
        dis->drawLine(189, 188, 187, 186, color);
    }
}

void cyberDrawModuleEngPos(uint8_t color, bool active)
{
    dis->drawLine(186, 191, 186, 196, color);
    dis->drawLine(186, 196, 192, 191, color);
    dis->drawLine(192, 191, 186, 191, color);
    if (active)
    {
        dis->drawLine(187, 192, 187, 194, color);
        dis->drawLine(187, 194, 189, 192, color);
        dis->drawLine(189, 192, 187, 192, color);
    }
}

void cyberClearMarkers()
{
    cyberDrawModulePos(SCWhite, true);
    cyberDrawModuleEngPos(SCWhite, true);
}

void drawPosMarkerCyber()
{
    // First clean all Pos markers
    cyberClearMarkers();

    // Then draw the one
    if (rM.cyber.watchfacePos == EMPTY_POS)
    {
        cyberClearMarkers();
    }
    else if (rM.cyber.watchfacePos == MODULE_POS)
    {
        cyberDrawModulePos(SCBlack, rM.cyber.positionEngaged);
    }
    if (rM.cyber.watchfacePos == MODULE_ENG_POS)
    {
        cyberDrawModuleEngPos(SCBlack, rM.cyber.positionEngaged);
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
