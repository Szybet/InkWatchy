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
    vault,
    apple,
    apple2,
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
extern RTC_DATA_ATTR UiPlace placeTree[PLACE_TREE_MAX_DEPTH];
extern RTC_DATA_ATTR UiPlace currentPlace;
extern RTC_DATA_ATTR void (*exitFunc)();

void initManager();
void loopManager();

// Manager functions
void initMainMenu();
void initDebugMenu();
void initWeatherMenu();
extern int weatherDayChoosed;
void initWeatherConditionMenu();
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
void switchVault();
void switchBack();
void switchApple();
void switchApple2();
#if FONT_MENU_ENABLED
void switchFontsPreview();
#endif

// Switches for weather
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
