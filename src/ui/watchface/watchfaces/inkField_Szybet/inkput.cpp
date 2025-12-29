#include "inkput.h"
#include "rtcMem.h"

#if WATCHFACE_INKFIELD_SZYBET

void movePos(int add)
{
    rM.inkfield.watchfacePos = rM.inkfield.watchfacePos + add;
    checkMaxMin(&rM.inkfield.watchfacePos, WATCHFACE_POSITIONS - 1);
    drawPosMarker();
}

void inkFieldManageInput(buttonState bt)
{
    if (rM.inkfield.watchfacePos == MODULE_ENG_POS && rM.inkfield.positionEngaged == true && bt != None)
    {
        debugLog("Module eng entered");
        if (bt != LongMenu)
        {
            wfModulesManage(bt);
        }
        else
        {
            debugLog("Dis engage mogule eng pos");
            rM.inkfield.positionEngaged = false;
            drawPosMarker();
        }
        return;
    }
    switch (bt)
    {
    case Up:
    {
        if (rM.inkfield.positionEngaged == false)
        {
            movePos(1);
        }
        else
        {
            if (rM.inkfield.watchfacePos == MODULE_POS)
            {
                wfModuleSwitch(Right);
                drawModuleCount();
            }
        }
        break;
    }
    case Down:
    {
        if (rM.inkfield.positionEngaged == false)
        {
            movePos(-1);
        }
        else
        {
            if (rM.inkfield.watchfacePos == MODULE_POS)
            {
                wfModuleSwitch(Left);
                drawModuleCount();
            }
        }
        break;
    }
    case Menu:
    {
        debugLog("rM.inkfield.watchfacePos: " + String(rM.inkfield.watchfacePos));
        if (rM.inkfield.watchfacePos == EMPTY_POS)
        {
            generalSwitch(mainMenu);
        }
        else if (rM.inkfield.watchfacePos == MODULE_POS)
        {
            wfModulesManage(Menu);
        }
        break;
    }
    case LongUp:
    {
        if (rM.inkfield.watchfacePos == MODULE_POS)
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
        rM.inkfield.positionEngaged = !rM.inkfield.positionEngaged;
        drawPosMarker();
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

#endif
