#include "manager.h"

RTC_DATA_ATTR int currentPlaceIndex = 0;
RTC_DATA_ATTR UiPlace placeTree[PLACE_TREE_MAX_DEPTH] = {NoPlace};
RTC_DATA_ATTR UiPlace currentPlace = NoPlace; // For loop manager for launching init or loop of a function

void initManager()
{
    placeTree[0] = FIRST_PLACE;
}

void managerLaunchFunc(UiPlace place, void (*initFunc)(), void (*loopFunc)())
{
    if (currentPlace == place)
    {
        loopFunc();
    }
    else
    {
        currentPlace = place;
        initFunc();
    }
}

void loopManager()
{
    buttonState backButton = useButtonBack();
    if (backButton != None)
    {
        if (currentPlaceIndex > 0)
        {
            if (backButton == Back)
            {
                placeTree[currentPlaceIndex] = NoPlace;
                currentPlaceIndex -= 1;
            } else if(backButton == LongBack) {
                for(int i = 1; i < PLACE_TREE_MAX_DEPTH; i++) {
                    placeTree[i] = NoPlace;
                }
                currentPlaceIndex = 0;
            }
        }
    }

    switch (placeTree[currentPlaceIndex])
    {
    case watchface:
    {

        break;
    }
    case mainMenu:
    {
        managerLaunchFunc(mainMenu, initMainMenu, loopMenu);
        break;
    }
    case debugMenu:
    {
        managerLaunchFunc(debugMenu, initDebugMenu, loopMenu);
        break;
    }
    }
}
