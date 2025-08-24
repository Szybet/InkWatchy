#include "dotput.h"
#include "rtcMem.h"

#if WATCHFACE_DOMAIN_DOTP

void domainMovePos(int add)
{
    rM.domain.watchfacePos = rM.domain.watchfacePos + add;
    checkMaxMin(&rM.domain.watchfacePos, WATCHFACE_POSITIONS - 1);
    domainDrawPosMarker();
}

void domainManageInput(buttonState bt)
{
    if (rM.domain.watchfacePos == MODULE_ENG_POS && rM.domain.positionEngaged == true && bt != None)
    {
        debugLog("Module eng entered");
        if (bt != LongMenu)
        {
            wfModulesManage(bt);
        }
        else
        {
            debugLog("Dis engage mogule eng pos");
            rM.domain.positionEngaged = false;
            domainDrawPosMarker();
        }
        return;
    }
    switch (bt)
    {
    case Up:
    {
        if (rM.domain.positionEngaged == false)
        {
            domainMovePos(1);
        }
        else
        {
            if (rM.domain.watchfacePos == MODULE_POS)
            {
                wfModuleSwitch(Right);
                drawModuleCount();
            }
        }
        break;
    }
    case Down:
    {
        if (rM.domain.positionEngaged == false)
        {
            domainMovePos(-1);
        }
        else
        {
            if (rM.domain.watchfacePos == MODULE_POS)
            {
                wfModuleSwitch(Left);
                drawModuleCount();
            }
        }
        break;
    }
    case Menu:
    {
        debugLog("rM.domain.watchfacePos: " + String(rM.domain.watchfacePos));
        if (rM.domain.watchfacePos == EMPTY_POS)
        {
            generalSwitch(mainMenu);
        }
        else if (rM.domain.watchfacePos == MODULE_POS)
        {
            wfModulesManage(Menu);
        }
        break;
    }
    case LongUp:
    {
        if (rM.domain.watchfacePos == MODULE_POS)
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
        rM.domain.positionEngaged = !rM.domain.positionEngaged;
        domainDrawPosMarker();
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