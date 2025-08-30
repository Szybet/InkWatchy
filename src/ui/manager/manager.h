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
    motorDebug,
    wifiDebug,
    gitDebug,
    accDebug,
    weatherMenu,             // Weather main menu
    weatherDateMenu,         // Weather submenu (formally weatherMenu)
    weatherConditionMenu,    // Weather date selection
    airQualityDateMenu,      // Air quality submenu
    airQualityConditionMenu, // Air quality date selection
    alarmSelectorMenu,
    alarmEditMenu,
    alarmRing,
    alarmQuick,
    alarmSetChooser,
    setTimePlace,
    alarmEditDays,
    pomodoroMenu,
    powerMenu,
    book,
    bookSelector,
    vault,
    vaultImage, // because of exit function
    vaultMenu,  // because of exit function
    apple,
    apple2,
    calendarDateMenu,
    calendarEventMenu,
    videoPlayer,
    videoMenu,
    pong,
    tetris,
    jumperGame,
    snake,
    gamesMenu,
    credits,
    conwayApp,
    partyApp,
    heartMonitor,
    baikyApp,
    fontPreview,
    fontPreviewMenu,
    watchfaceSelector,
    // General places now:
    inputPinPlace,
    generalMenuPlace,
    imagePlace,
    textDialog,
    errorDialog,
    ChartPlace,
    NoPlace,
} UiPlace;

extern int currentPlaceIndex;
extern UiPlace placeTree[PLACE_TREE_MAX_DEPTH];
extern UiPlace currentPlace;
extern void (*exitFunc)();
extern void (*exitFuncGlob)(); // Executed when a place is exiting and it was requested

extern bool disabledBacking;

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
void initWeatherDateMenu();
void initWeatherConditionMenu();
void initAirQualityDateMenu();
void initAirQualityConditionMenu();
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
void switchAccDebug();
void switchMotorDebug();
void switchWeatherMenu();
void switchWeatherDateMenu();
void switchWeatherConditionMenu();
void switchAirQualityDateMenu();
void switchAirQualityConditionMenu();
#if INK_ALARMS
void switchAlarmSelectorMenu();
void switchAlarmEditMenu();
void switchAlarmEditDays();
void switchAlarmRing();
void switchAlarmSetChooser();
void switchAlarmQuick();
void switchPomodoroMenu();
#endif
void switchSetTime();
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
#if JUMPER
void switchJumper();
#endif
#if SNAKE
void switchSnake();
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
#if HEART_MONITOR
void switchHeartMonitor();
#endif
#if BAIKY
void switchBaiky();
#endif
#if FONT_MENU_ENABLED
void switchFontsPreview();
#endif
void showTextDialog(String str, bool center, String title = "");
