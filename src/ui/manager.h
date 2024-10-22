#ifndef MANAGER_H
#define MANAGER_H

#include "../defines/defines.h"

#define FIRST_PLACE watchface
#define PLACE_TREE_MAX_DEPTH 30

typedef enum
{
    watchface,
    mainMenu,
    debugMenu,
    generalDebug,
    batteryDebug,
    wifiDebug,
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
    pong,
    fontPreview,
    fontPreviewMenu,
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
void initDebugMenu();
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
void switchGeneralDebug();
void switchBatteryDebug();
void switchWifiDebug();
void switchWeatherMenu();
void switchWeatherSelectorMenu();
void switchPowerMenu();

void switchBook();
void switchBookSelector();
void switchVault();
void switchBack();
void switchApple();
void switchApple2();
void switchPong();
#if FONT_MENU_ENABLED
void switchFontsPreview();
#endif
void showTextDialog(String str);

// Switches for weather
#if WEATHER_INFO
void showTemp();
void showPressure();
void showHumidity();
void showWeatherCond();
void showClouds();
void showWindSpeed();
void showWindGuts();
void showVisibility();
void showPop();
#endif

#endif
