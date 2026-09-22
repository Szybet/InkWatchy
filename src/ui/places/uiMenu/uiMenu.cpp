#include "uiMenu.h"
#include "rtcMem.h"

void toggleInvertScreen()
{
    rM.screenInverted = !rM.screenInverted;
    applyScreenColors();
    dUChange = true;
    rM.updateCounter = FULL_DISPLAY_UPDATE_QUEUE;
    initUiMenu();
}

void toggleInvertWatchface()
{
    rM.watchfaceInverted = !rM.watchfaceInverted;
    dUChange = true;
    rM.updateCounter = FULL_DISPLAY_UPDATE_QUEUE;
    initUiMenu();
}

void initUiMenu()
{
    int count = -1;
    entryMenu buttons[2];
    {
        count = count + 1;
        String image = rM.screenInverted ? "accept" : "cross";
        buttons[count] = {MENU_INVERT_SCREEN, image, toggleInvertScreen};
    }
    {
        count = count + 1;
        String image = rM.watchfaceInverted ? "accept" : "cross";
        buttons[count] = {MENU_INVERT_WATCHFACE, image, toggleInvertWatchface};
    }

    count = count + 1;
    initMenu(buttons, count, "UI", 1);
}
