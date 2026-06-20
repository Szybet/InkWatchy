#include "manager.h"
#include "rtcMem.h"

void initMainMenu()
{
    int count = -1;
    entryMenu buttons[14];
#if CALENDAR
    {
        count = count + 1;
        buttons[count] = {MENU_CALENDAR, "calendar", switchCalendarDateMenu};
    }
#endif
#if BOOK
    debugLog("Getting book here");
    String book = getCurrentBook();
    debugLog("Book got: " + book);
    String bookPages = bookGetPages();
    debugLog("Book pages: " + bookPages);
    if (book != "")
    {
        count = count + 1;
        buttons[count] = {book + " " + bookPages, "book", switchBook};
    }
    {
        count = count + 1;
        buttons[count] = {MENU_SELECT_BOOK, "book", switchBookSelector};
    }
#endif
#if INK_ALARMS
    {
        count = count + 1;
        buttons[count] = {MENU_ALARMS, "alarms/alarmIcon", switchAlarmSetChooser};
    }
#endif
#if WEATHER_INFO
    {
        count = count + 1;
        buttons[count] = {MENU_WEATHER, "weather", switchWeatherMenu};
    }
#endif

    {
        count = count + 1;
        buttons[count] = {MENU_SETTINGS, "settings", switchSettingsMenu};
    }
#if HEALTH_MENU
    {
        count = count + 1;
        buttons[count] = {MENU_HEALTH, "healthIcon", switchHealthMenu};
    }
#endif
#if VAULT
    {
        count = count + 1;
        buttons[count] = {MENU_VAULT, "vault", switchVault};
    }
#endif
#if NOTES_APP
    {
        count = count + 1;
        buttons[count] = {"Notes", "notes/notesIcon", switchNotesManager};
    }
#endif
#if WIFI_TOOL
    {
        count = count + 1;
        void (*wifiToolFunc)();
        if (wifiToolRunning == true)
        {
            wifiToolFunc = stopWifiTool;
        }
        else
        {
            wifiToolFunc = initWifiTool;
        }
        buttons[count] = {MENU_WIFI_TOOL ": " + wifiToolStatus(), "wifiTool", wifiToolFunc};
    }
#endif
#if APPLE_JOKE
    {
        debugLog("Adding apple joke to menu");
        count = count + 1;
        buttons[count] = {MENU_EATING_APPLES, "apple", switchApple};
    }
    {
        debugLog("Adding apple joke to menu");
        count = count + 1;
        buttons[count] = {MENU_SMASHING_APPLES, "apple", switchApple2};
    }
#endif
#if PONG || TETRIS || JUMPER || SNAKE || DICE || PAINT
    {
        debugLog("Adding games");
        count = count + 1;
        buttons[count] = {MENU_GAMES, "games", switchGamesMenu};
    }
#endif
#if CREDITS
    {
        debugLog("Adding Credits");
        count = count + 1;
        buttons[count] = {MENU_CREDITS, "credits/qr", switchCredits};
    }
#endif
    count = count + 1;
    initMenu(buttons, count, MENU_MAIN, 1);
}

#if DEBUG_MENUS
void initDebugMenu()
{
    int count = -1;
    entryMenu buttons[7];
    {
        debugLog("Adding clock to menu");
        count = count + 1;
        buttons[count] = {DEBUG_ITEM_CLOCK, "", switchClockDebug};
    }
    {
        debugLog("Adding general to menu");
        count = count + 1;
        buttons[count] = {DEBUG_ITEM_HARDWARE, "", switchGeneralDebug};
    }
    {
        debugLog("Adding battery to menu");
        count = count + 1;
        buttons[count] = {DEBUG_ITEM_BATTERY, "", switchBatteryDebug};
    }
    {
        debugLog("Adding git to menu");
        count = count + 1;
        buttons[count] = {DEBUG_ITEM_GIT, "", switchGitDebug};
    }
    {
        debugLog("Adding acc to menu");
        count = count + 1;
        buttons[count] = {DEBUG_ITEM_ACC, "", switchAccDebug};
    }
    {
        debugLog("Adding motor to menu");
        count = count + 1;
        buttons[count] = {DEBUG_ITEM_MOTOR, "", switchMotorDebug};
    }
#if FONT_MENU_ENABLED
    {
        debugLog("Adding font preview to menu");
        count = count + 1;
        buttons[count] = {DEBUG_ITEM_FONT_PREVIEW, "", switchFontsPreviewMenu};
    }
#endif
    count = count + 1;
    initMenu(buttons, count, MENU_DEBUG, 1);
}
#endif

void initSettingsMenu()
{
    int count = -1;
    entryMenu buttons[6];
    {
        debugLog("Adding wifi to menu");
        count = count + 1;
        buttons[count] = {MENU_WIFI, "wifiIcon", switchWifiDebug};
    }
#if DEBUG_MENUS
    {
        count = count + 1;
        buttons[count] = {MENU_DEBUG, "debug", switchDebugMenu};
    }
#endif
    {
        count = count + 1;
        buttons[count] = {MENU_POWER_SETTINGS, "battery", switchPowerMenu};
    }
    {
        count = count + 1;
        buttons[count] = {MENU_CHANGE_WATCHFACE, "watchfaceIcon", switchWatchfaceSelectorMenu};
    }
#if SET_CLOCK_GUI
    {
        count = count + 1;
        buttons[count] = {SETCLOCK_CLOCK_SETTINGS, "setTimeGui/setTimeClock", switchSetClockMenu};
    }
#endif

    count = count + 1;
    initMenu(buttons, count, MENU_SETTINGS, 1);
}

void initGamesMenu()
{
    int count = -1;
    entryMenu buttons[11];
#if TETRIS
    {
        debugLog("Adding Tetris");
        count = count + 1;
        buttons[count] = {MENU_TETRIS, "tetris/tetrisIcon", switchTetris};
    }
#endif
#if SNAKE
    {
        debugLog("Adding Snake");
        count = count + 1;
        buttons[count] = {MENU_SNAKE, "snake/snakeIcon", switchSnake};
    }
#endif
#if MAZE
    {
        debugLog("Adding maze");
        count = count + 1;
        buttons[count] = {"Maze", "maze/mazeIcon", switchMaze};
    }
#endif
#if JUMPER
    {
        debugLog("Adding jumper");
        count = count + 1;
        buttons[count] = {MENU_JUMPER, "jumper/jumperIcon", switchJumper};
    }
#endif
#if DICE
    {
        debugLog("Adding Dice");
        count = count + 1;
        buttons[count] = {MENU_DICE, "dice/diceIcon", switchDiceMenu};
    }
#endif
#if PAINT
    {
        debugLog("Adding paint");
        count = count + 1;
        buttons[count] = {"Paint", "paint/paintIcon", switchPaint};
    }
#endif
#if PONG
    {
        debugLog("Adding pong");
        count = count + 1;
        buttons[count] = {MENU_PONG, "pong", switchPong};
    }
#endif
#if BLE_PERIPHERAL
    {
        count = count + 1;
        String mess = "BLE Peripheral";
        buttons[count] = {mess, "blePeripheral/BLEPIcon", switchBlePeripheral};
    }
#endif
#if VIDEO_PLAYER
    {
        debugLog("Adding video player");
        count = count + 1;
        buttons[count] = {MENU_VIDEO_PLAYER, "videoIcon", switchVideoMenu};
    }
#endif
#if CONWAY
    {
        debugLog("Adding Conway");
        count = count + 1;
        buttons[count] = {MENU_CONWAY, "conway", switchConway};
    }
#endif
#if RGB_DIODE
    {
        debugLog("Adding party");
        count = count + 1;
        buttons[count] = {MENU_PARTY, "rgb", switchParty};
    }
#endif
    count = count + 1;
    initMenu(buttons, count, MENU_GAMES, 1);
}

void initHealthMenu()
{
    int count = -1;
    entryMenu buttons[3];
#if BAIKY
    {
        count = count + 1;
        buttons[count] = {"Baiky", "baiky/baikyIcon", switchBaiky};
    }
#endif
#if HEART_MONITOR
    {
        count = count + 1;
        buttons[count] = {MENU_HEART_MONITOR, "heartMonitor/heartIcon", switchHeartMonitor};
    }
#endif
#if PRECISE_STEP_COUNTING
    {
        count = count + 1;
        buttons[count] = {MENU_STEPS_PER_DAY, "", switchDailyStepsChartMenu};
    }
#endif
    count = count + 1;
    initMenu(buttons, count, MENU_HEALTH, 1);
}
