#include "modules.h"

RTC_DATA_ATTR int watchfacePos = 0;
RTC_DATA_ATTR bool positionEngaged = true;

void movePos(int add)
{
    watchfacePos = watchfacePos + add;
    checkMaxMin(&watchfacePos, WATCHFACE_POSITIONS - 1);
    drawPosMarker();
}

void modulesButtons(buttonState bt)
{
    if (watchfacePos == MODULE_ENG_POS && positionEngaged == true && bt != None)
    {
        debugLog("Module eng entered");
        if (bt != LongMenu)
        {
            wfModulesManage(bt);
        }
        else
        {
            debugLog("Dis engage mogule eng pos");
            positionEngaged = false;
            drawPosMarker();
        }
        return;
    }
    switch (bt)
    {
    case Up:
    {
        if (positionEngaged == false)
        {
            movePos(1);
        }
        else
        {
            if (watchfacePos == MODULE_POS)
            {
                wfModuleSwitch(Right);
                drawModuleCount();
            }
        }
        break;
    }
    case Down:
    {
        if (positionEngaged == false)
        {
            movePos(-1);
        }
        else
        {
            if (watchfacePos == MODULE_POS)
            {
                wfModuleSwitch(Left);
                drawModuleCount();
            }
        }
        break;
    }
    case Menu:
    {
        debugLog("watchfacePos: " + String(watchfacePos));
        if (watchfacePos == EMPTY_POS)
        {
            generalSwitch(mainMenu);
        }
        else if (watchfacePos == MODULE_POS)
        {
            wfModulesManage(Menu);
        }
        break;
    }
    case LongUp:
    {
        if (watchfacePos == MODULE_POS)
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
        positionEngaged = !positionEngaged;
        drawPosMarker();
        break;
    }
    }
}