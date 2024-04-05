#include "modules.h"

#define WATCHFACE_POSITIONS 2 // There is one "empty" on 0 too
#define EMPTY_POS 0
#define MODULE_POS 1
RTC_DATA_ATTR int watchfacePos = 0;
RTC_DATA_ATTR bool positionEngaged = false;

void drawPosMarker()
{
    // First clean all Pos markers
    display.drawPixel(190, 160, GxEPD_WHITE);
    display.drawPixel(191, 160, GxEPD_WHITE);
    display.drawPixel(191, 161, GxEPD_WHITE);

    display.drawPixel(108, 60, GxEPD_WHITE);
    display.drawPixel(109, 60, GxEPD_WHITE);
    display.drawPixel(108, 59, GxEPD_WHITE);

    // Then draw the one
    if (watchfacePos == MODULE_POS)
    {
        // 190, 160
        // 191, 160
        // 191, 161
        display.drawPixel(190, 160, GxEPD_BLACK);
        display.drawPixel(191, 160, GxEPD_BLACK);
        display.drawPixel(191, 161, GxEPD_BLACK);
    }
    else if (watchfacePos == EMPTY_POS)
    {
        // 108, 60
        // 109, 60
        // 108, 59
        display.drawPixel(108, 60, GxEPD_BLACK);
        display.drawPixel(109, 60, GxEPD_BLACK);
        display.drawPixel(108, 59, GxEPD_BLACK);
    }
    dUChange = true;
}

void movePos(int add)
{
    watchfacePos = watchfacePos + add;
    checkMaxMin(&watchfacePos, WATCHFACE_POSITIONS - 1);
    drawPosMarker();
}

void modulesButtons(buttonState bt)
{
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
        if (watchfacePos == EMPTY_POS)
        {
            generalSwitch(mainMenu);
        }
        else if (watchfacePos == MODULE_POS)
        {
            wfModulesManage(Menu);
            drawModuleCount();
        }
        break;
    }
    case LongUp:
    {

        break;
    }
    case LongDown:
    {

        break;
    }
    case LongMenu:
    {
        positionEngaged = !positionEngaged;
        break;
    }
    }
}