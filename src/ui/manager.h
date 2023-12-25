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

// Manager switches
void switchDebugMenu();

#endif
