#pragma once

#include "defines.h"

#define FIRST_PLACE watchface
#define PLACE_TREE_MAX_DEPTH 15

typedef enum
{
    watchface,
    mainMenu,
    debugMenu,
    settingsMenu,
    generalDebug,
    clockDebug,
    batteryDebug,
    wifiDebug,
    gitDebug,
    weatherMenu,
    weatherConditionMenu,
    powerMenu,
    book,
    bookSelector,
    vault,
    vaultImage, // because of exit function
    vaultMenu, // because of exit function
    apple,
    apple2,
    calendarDateMenu,
    calendarEventMenu,
    videoPlayer,
    videoMenu,
    pong,
    tetris,
    gamesMenu,
    credits,
    conwayApp,
    partyApp,
    fontPreview,
    fontPreviewMenu,
    watchfaceSelector,
    // General places now:
    inputPinPlace,
    generalMenuPlace,
    imagePlace,
    textDialog,
    ChartPlace,
    NoPlace,
} UiPlace;

extern int currentPlaceIndex;
extern UiPlace placeTree[PLACE_TREE_MAX_DEPTH];
extern UiPlace currentPlace;
extern void (*exitFunc)();
extern void (*exitFuncGlob)(); // Executed when a place is exiting and it was requested

void executeExitFunc();

void initManager();
void loopManager();

// Manager functions
void initMainMenu();
#if DEBUG_MENUS
void initDebugMenu();
#endif
void initSettingsMenu();
void initGamesMenu();
#if WEATHER_INFO
void initWeatherMenu();
void initWeatherConditionMenu();
#endif
void initpowerMenu();
#if FONT_MENU_ENABLED
void initFontMenu();
void switchFontsPreviewMenu();
#endif

// Manager switches
void generalSwitch(UiPlace place);
void overwriteSwitch(UiPlace place);

void switchDebugMenu();
void switchSettingsMenu();
void switchGamesMenu();
void switchGeneralDebug();
void switchClockDebug();
void switchBatteryDebug();
void switchWifiDebug();
void switchGitDebug();
void switchWeatherMenu();
void switchWeatherSelectorMenu();
void switchPowerMenu();
void switchWatchfaceSelectorMenu();

void switchBook();
void switchBookSelector();
void switchVault();
void switchBack();
#if VIDEO_PLAYER
void switchVideoPlayer();
void switchVideoMenu();
#endif
void switchApple();
void switchApple2();
void switchPong();
#if TETRIS
void switchTetris();
#endif
#if CREDITS
void switchCredits();
#endif
#if CONWAY
void switchConway();
#endif
#if RGB_DIODE
void switchParty();
#endif
#if FONT_MENU_ENABLED
void switchFontsPreview();
#endif
void showTextDialog(String str);

