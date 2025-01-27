#include "manager.h"
#include "rtcMem.h"

int currentPlaceIndex = 0;
int menuSelectedTree[PLACE_TREE_MAX_DEPTH] = {0};
bool wasBacked = false;
void (*exitFuncGlob)() = nullptr; // Executed when a place is exiting and it was requested

void initManager()
{
    rM.placeTree[0] = FIRST_PLACE;
}

// in managerLaunchFunc, sleep and in manager long back.
void executeExitFunc()
{
    if (exitFuncGlob != nullptr)
    {
        debugLog("Executing exit func");
        void (*exitFuncGlobTmp)() = exitFuncGlob;
        exitFuncGlob = nullptr;
        exitFuncGlobTmp();
    }
    else
    {
        debugLog("No exit function supplied");
    }
}

void managerLaunchFunc(UiPlace place, void (*initFunc)(), void (*loopFunc)(), void (*exitFunc)() = nullptr)
{
    if (rM.currentPlace == place)
    {
        // debugLog("Executing loop function?");
        if (loopFunc != nullptr)
        {
            loopFunc();
        }
    }
    else
    {
        // debugLog("rM.currentPlace: " + String(rM.currentPlace));
        // debugLog("place: " + String(place));
        rM.currentPlace = place;
        if (wasBacked == false)
        {
            if (currentPlaceIndex - 1 >= 0)
            {
                debugLog("Saving to index " + String(currentPlaceIndex - 1) + " menu index currentMenuItem:" + String(currentMenuItem));
                menuSelectedTree[currentPlaceIndex - 1] = currentMenuItem;
                // currentMenuItem = menuSelectedTree[currentPlaceIndex]; // Hehe no, it goes to another menu on the same level
                currentMenuItem = 0;
            }
        }
        else
        {
            debugLog("Restoring menu of tree index " + String(currentPlaceIndex) + " value " + String(menuSelectedTree[currentPlaceIndex]));
            currentMenuItem = menuSelectedTree[currentPlaceIndex];
            wasBacked = false;
        }

        executeExitFunc();
        if (initFunc != nullptr)
        {
            debugLog("Executing init function");
            dis->fillScreen(GxEPD_WHITE);
            initFunc();
            if (exitFunc != nullptr)
            {
                debugLog("Saving exit func");
                exitFuncGlob = exitFunc;
            }
        }
        else
        {
            debugLog("No init function supplied");
        }
    }
}

void loopManager()
{
    // We want watchface to be able to use
    if (currentPlaceIndex != 0)
    {
        buttonState backButton = useButtonBack();
        if (backButton != None)
        {
            debugLog("Button is back, manager got it");
            if (currentPlaceIndex > 0)
            {
                if (backButton == Back)
                {
                    rM.placeTree[currentPlaceIndex] = NoPlace;
                    currentPlaceIndex -= 1;
                    wasBacked = true;
                }
                else if (backButton == LongBack)
                {
                    for (int i = 1; i < PLACE_TREE_MAX_DEPTH; i++)
                    {
                        rM.placeTree[i] = NoPlace;
                    }
#if LONG_BACK_FULL_REFRESH
                    debugLog("Forcing full update because of long back button");
                    dUChange = true;
                    rM.updateCounter = FULL_DISPLAY_UPDATE_QUEUE;
#endif
                    currentPlaceIndex = 0;
                }
                executeExitFunc();
            }
        }
    }

    // debugLog("Current place: " + String(rM.placeTree[currentPlaceIndex]) + " and index: " + String(currentPlaceIndex));
    // Don't forget break...
    switch (rM.placeTree[currentPlaceIndex])
    {
    case watchface:
    {
        managerLaunchFunc(watchface, initWatchfaceManage, loopWatchfaceManage);
        break;
    }
    case mainMenu:
    {
        managerLaunchFunc(mainMenu, initMainMenu, loopMenu);
        break;
    }
    case debugMenu:
    {
#if DEBUG_MENUS
        managerLaunchFunc(debugMenu, initDebugMenu, loopMenu);
#endif
        break;
    }
    case settingsMenu:
    {
        managerLaunchFunc(settingsMenu, initSettingsMenu, loopMenu);
        break;
    }
    case generalDebug:
    {
#if DEBUG_MENUS
        managerLaunchFunc(generalDebug, initGeneralDebugDisplay, loopGeneralDebugDisplay);
#endif
        break;
    }
    case clockDebug:
    {
#if DEBUG_MENUS
        managerLaunchFunc(clockDebug, initClockDebug, loopClockDebug);
#endif
        break;
    }
    case gitDebug:
    {
#if DEBUG_MENUS
        managerLaunchFunc(gitDebug, initGitDebug, useButtonBlank);
#endif
        break;
    }
    case batteryDebug:
    {
#if DEBUG_MENUS
        managerLaunchFunc(batteryDebug, initBatteryDebugDisplay, loopBatteryDebugDisplay);
#endif
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
#if WEATHER_INFO
        managerLaunchFunc(weatherMenu, initWeatherMenu, loopMenu);
#endif
        break;
    }
    case weatherConditionMenu:
    {
#if WEATHER_INFO
        managerLaunchFunc(weatherConditionMenu, initWeatherConditionMenu, loopMenu);
#endif
        break;
    }
    case powerMenu:
    {
        managerLaunchFunc(powerMenu, initpowerMenu, loopMenu, saveAllStorage);
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
    case bookSelector:
    {
#if BOOK
        managerLaunchFunc(book, initBookSelector, loopMenu);
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
    case vaultImage:
    {
#if VAULT
        managerLaunchFunc(imagePlace, NULL, useButtonBlank, exitVault);
#endif
        break;
    }
    case vaultMenu:
    {
#if VAULT
        // TODO: generalMenuPlace? Ooh?
        managerLaunchFunc(generalMenuPlace, NULL, loopMenu, exitVault);
#endif
        break;
    }
    case apple:
    {
#if APPLE_JOKE
        managerLaunchFunc(apple, initAppleJoke, loopAppleJoke, exitAppleJoke);
#endif
        break;
    }
    case apple2:
    {
#if APPLE_JOKE
        managerLaunchFunc(apple2, initAppleSour, loopAppleSour, exitAppleSour);
#endif
        break;
    }
    case calendarDateMenu:
    {
#if CALENDAR
        managerLaunchFunc(calendarDateMenu, initCalendar, loopMenu);
#endif
        break;
    }
    case calendarEventMenu:
    {
#if CALENDAR
        managerLaunchFunc(calendarEventMenu, initCalendar, loopMenu);
#endif
        break;
    }
    case videoPlayer:
    {
#if VIDEO_PLAYER
        managerLaunchFunc(videoPlayer, initVideoPlayer, loopVideoPlayer, exitVideoPlayer);
#endif
        break;
    }
    case videoMenu:
    {
#if VIDEO_PLAYER
        managerLaunchFunc(videoMenu, initVideoMenu, loopMenu);
#endif
        break;
    }
    case pong:
    {
#if PONG
        managerLaunchFunc(pong, initPong, loopPong);
#endif
        break;
    }
    case tetris:
    {
#if TETRIS
        managerLaunchFunc(tetris, initTetris, loopTetris, exitTetris);
#endif
        break;
    }
    case gamesMenu:
    {
        managerLaunchFunc(gamesMenu, initGamesMenu, loopMenu);
        break;
    }
    case credits:
    {
#if CREDITS
        managerLaunchFunc(credits, initCredits, loopCredits);
#endif
        break;
    }
    case conwayApp:
    {
#if CONWAY
        managerLaunchFunc(conwayApp, initConway, loopConway);
#endif
        break;
    }
    case partyApp:
    {
#if RGB_DIODE
        managerLaunchFunc(partyApp, initParty, loopParty, exitParty);
#endif
        break;
    }
    case fontPreview:
    {
#if FONT_MENU_ENABLED
        managerLaunchFunc(fontPreview, initFontPreview, useButtonBlank);
#endif
        break;
    }
    case fontPreviewMenu:
    {
#if FONT_MENU_ENABLED
        managerLaunchFunc(fontPreviewMenu, initFontMenu, loopMenu);
#endif
        break;
    }
    case watchfaceSelector:
    {
        managerLaunchFunc(watchfaceSelector, watchfaceSelInit, loopMenu);
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
    case NoPlace:
    {
        debugLog("This shouldn't happen?");
        break;
    }
    default:
    {
        debugLog("You forgot to implement a place!");
        break;
    }
    }
}
