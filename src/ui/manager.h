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
    textDialog, // So nothing, really
    weatherMenu,
    weatherConditionMenu,
    book,
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

// Manager switches
void generalSwitch(UiPlace place);
void overwriteSwitch(UiPlace place);

void switchDebugMenu();
void switchGeneralDebug();
void switchBatteryDebug();
void switchWifiDebug();
void switchWeatherMenu();
void switchWeatherSelectorMenu();
void switchBook();

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
