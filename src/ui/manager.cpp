#include "manager.h"

int currentPlaceIndex = 0;
RTC_DATA_ATTR UiPlace placeTree[PLACE_TREE_MAX_DEPTH] = {NoPlace};
RTC_DATA_ATTR UiPlace currentPlace = NoPlace; // For loop manager for launching init or loop of a function
int menuSelectedTree[PLACE_TREE_MAX_DEPTH] = {0};
bool wasBacked = false;
RTC_DATA_ATTR void (*exitFuncGlob)() = nullptr; // Executed when a place is exiting and it was requested

void initManager()
{
    placeTree[0] = FIRST_PLACE;
}

void managerLaunchFunc(UiPlace place, void (*initFunc)(), void (*loopFunc)(), void (*exitFunc)() = nullptr)
{
    if (currentPlace == place)
    {
        if (loopFunc != nullptr)
        {
            loopFunc();
        }
    }
    else
    {
        currentPlace = place;
        if (wasBacked == false)
        {
            if (currentPlaceIndex - 1 >= 0)
            {
                debugLog("Saving to index " + String(currentPlaceIndex - 1) + " menu index " + String(currentMenuItem));
                menuSelectedTree[currentPlaceIndex - 1] = currentMenuItem;
                // currentMenuItem = menuSelectedTree[currentPlaceIndex]; // Hehe no, it goes to another menu on the same level
                currentMenuItem = 0;
            }
        }
        else
        {
            debugLog("Restoring menu of tree index " + String(currentMenuItem) + " value " + String(menuSelectedTree[currentPlaceIndex]));
            currentMenuItem = menuSelectedTree[currentPlaceIndex];
            wasBacked = false;
        }

        if (exitFuncGlob != nullptr)
        {
            debugLog("Executing exit func");
            exitFuncGlob();
            exitFuncGlob = nullptr;
        }
        if (initFunc != nullptr)
        {
            display.fillScreen(GxEPD_WHITE);
            initFunc();
            if (exitFunc != nullptr)
            {
                exitFuncGlob = exitFunc;
            }
        }
    }
}

void loopManager()
{
    buttonState backButton = useButtonBack();
    if (backButton != None)
    {
        debugLog("Button is back, manager got it");
        if (currentPlaceIndex > 0)
        {
            if (backButton == Back)
            {
                placeTree[currentPlaceIndex] = NoPlace;
                currentPlaceIndex -= 1;
                wasBacked = true;
            }
            else if (backButton == LongBack)
            {
                for (int i = 1; i < PLACE_TREE_MAX_DEPTH; i++)
                {
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
        managerLaunchFunc(watchface, initWatchfaceDisplay, loopWatchfaceLoop);
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
    case generalDebug:
    {
        managerLaunchFunc(generalDebug, initGeneralDebugDisplay, loopGeneralDebugDisplay);
        break;
    }
    case batteryDebug:
    {
        managerLaunchFunc(batteryDebug, initBatteryDebugDisplay, loopBatteryDebugDisplay);
        break;
    }
    case wifiDebug:
    {
        managerLaunchFunc(wifiDebug, initWifiDebugDisplay, loopWifiDebugDisplay);
        break;
    }
    case textDialog:
    {
        managerLaunchFunc(textDialog, NULL, useButtonBlank);
        break;
    }
    case weatherMenu:
    {
        managerLaunchFunc(weatherMenu, initWeatherMenu, loopMenu);
        break;
    }
    case weatherConditionMenu:
    {
        managerLaunchFunc(weatherConditionMenu, initWeatherConditionMenu, loopMenu);
        break;
    }
    case ChartPlace:
    {
        managerLaunchFunc(ChartPlace, NULL, useButtonBlank);
        break;
    }
    case book:
    {
#if BOOK
        managerLaunchFunc(book, initBook, loopBook, exitBook);
#endif
        break;
    }
    case vault:
    {
#if VAULT
        managerLaunchFunc(vault, initVault, loopVault, exitVault);
#endif
        break;
    }
    case imagePlace:
    {
        managerLaunchFunc(imagePlace, NULL, useButtonBlank);
        break;
    }
    case inputPinPlace:
    {
        managerLaunchFunc(inputPinPlace, initPinInput, loopPinInput, exitPinInput);
        break;
    }
    case generalMenuPlace:
    {
        managerLaunchFunc(generalMenuPlace, NULL, loopMenu);
        break;
    }
    }
}
