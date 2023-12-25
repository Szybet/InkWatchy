#ifndef MANAGER_H
#define MANAGER_H

#include "../defines/defines.h"

#define FIRST_PLACE mainMenu
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
    ChartPlace,
    NoPlace,
} UiPlace;

extern RTC_DATA_ATTR int currentPlaceIndex;
extern RTC_DATA_ATTR UiPlace placeTree[PLACE_TREE_MAX_DEPTH];
extern RTC_DATA_ATTR UiPlace currentPlace;

void initManager();
void loopManager();

// Manager functions
void initMainMenu();
void initDebugMenu();
void initWeatherMenu();

// Manager switches
void generalSwitch(UiPlace place);
void overwriteSwitch(UiPlace place);

void switchDebugMenu();
void switchGeneralDebug();
void switchBatteryDebug();
void switchWifiDebug();
void switchWeatherMenu();
void switchWeatherSelectorMenu();
extern int weatherDayChoosed;
void initWeatherConditionMenu();

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
