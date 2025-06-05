#include "manager.h"
#include "localization.h" // For menu translations
#include "rtcMem.h"

void initMainMenu()
{
    int count = -1;
    entryMenu buttons[13];
#if CALENDAR
    {
        count = count + 1;
        buttons[count] = {MENU_CALENDAR, getImg("calendar"), switchCalendarDateMenu};
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
        buttons[count] = {book + " " + bookPages, getImg("book"), switchBook};
    }
    {
        count = count + 1;
        buttons[count] = {MENU_SELECT_BOOK, getImg("book"), switchBookSelector};
    }
#endif
#if INK_ALARMS
    {
        count = count + 1;
        buttons[count] = {MENU_ALARMS, getImg("alarms/alarmIcon"), switchAlarmSetChooser};
    }
#endif
#if WEATHER_INFO
    {
        count = count + 1;
        buttons[count] = {MENU_WEATHER, getImg("weather"), switchWeatherMenu};
    }
#endif
    {
        count = count + 1;
        buttons[count] = {MENU_SETTINGS, getImg("settings"), switchSettingsMenu};
    }
#if HEART_MONITOR
    {
        count = count + 1;
        buttons[count] = {MENU_HEART_MONITOR, getImg("heartmonitor/heartIcon"), switchHeartMonitor};
    }
#endif
#if VAULT
    {
        count = count + 1;
        buttons[count] = {MENU_VAULT, getImg("vault"), switchVault};
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
        buttons[count] = {MENU_WIFI_TOOL ": " + wifiToolStatus(), getImg("wifiTool"), wifiToolFunc};
    }
#endif
#if APPLE_JOKE
    {
        debugLog("Adding apple joke to menu");
        count = count + 1;
        buttons[count] = {MENU_EATING_APPLES, getImg("apple"), switchApple};
    }
    {
        debugLog("Adding apple joke to menu");
        count = count + 1;
        buttons[count] = {MENU_SMASHING_APPLES, getImg("apple"), switchApple2};
    }
#endif
    {
        debugLog("Adding games");
        count = count + 1;
        buttons[count] = {MENU_GAMES, getImg("games"), switchGamesMenu};
    }
#if CREDITS
    {
        debugLog("Adding Credits");
        count = count + 1;
        buttons[count] = {MENU_CREDITS, getImg("credits/qr"), switchCredits};
    }
#endif
    count = count + 1;
    initMenu(buttons, count, MENU_MAIN, 1);
}

#if DEBUG_MENUS
void initDebugMenu()
{
    int count = -1;
    entryMenu buttons[6];
    {
        debugLog("Adding clock to menu");
        count = count + 1;
        buttons[count] = {DEBUG_ITEM_CLOCK, &emptyImgPack, switchClockDebug};
    }
    {
        debugLog("Adding general to menu");
        count = count + 1;
        buttons[count] = {DEBUG_ITEM_HARDWARE, &emptyImgPack, switchGeneralDebug};
    }
    {
        debugLog("Adding battery to menu");
        count = count + 1;
        buttons[count] = {DEBUG_ITEM_BATTERY, &emptyImgPack, switchBatteryDebug};
    }
    {
        debugLog("Adding git to menu");
        count = count + 1;
        buttons[count] = {DEBUG_ITEM_GIT, &emptyImgPack, switchGitDebug};
    }
    {
        debugLog("Adding acc to menu");
        count = count + 1;
        buttons[count] = {DEBUG_ITEM_ACC, &emptyImgPack, switchAccDebug};
    }
#if FONT_MENU_ENABLED
    {
        debugLog("Adding font preview to menu");
        count = count + 1;
        buttons[count] = {DEBUG_ITEM_FONT_PREVIEW, &emptyImgPack, switchFontsPreviewMenu};
    }
#endif
    count = count + 1;
    initMenu(buttons, count, MENU_DEBUG, 1);
}
#endif

void initSettingsMenu()
{
    int count = -1;
    entryMenu buttons[5];
    {
        debugLog("Adding wifi to menu");
        count = count + 1;
        buttons[count] = {MENU_WIFI, getImg("wifiIcon"), switchWifiDebug};
    }
#if DEBUG_MENUS
    {
        count = count + 1;
        buttons[count] = {MENU_DEBUG, getImg("debug"), switchDebugMenu};
    }
#endif
    {
        count = count + 1;
        buttons[count] = {MENU_POWER_SETTINGS, getImg("battery"), switchPowerMenu};
    }
    {
        count = count + 1;
        buttons[count] = {MENU_CHANGE_WATCHFACE, getImg("watchfaceIcon"), switchWatchfaceSelectorMenu};
    }
    count = count + 1;
    initMenu(buttons, count, MENU_SETTINGS, 1);
}

void initGamesMenu()
{
    int count = -1;
    entryMenu buttons[6];
#if TETRIS
    {
        debugLog("Adding Tetris");
        count = count + 1;
        buttons[count] = {MENU_TETRIS, getImg("tetris/tetrisIcon"), switchTetris};
    }
#endif
#if SNAKE
    {
        debugLog("Adding Snake");
        count = count + 1;
        // Snake game, is everywhere in the world called "Snake", right? right? It's not WĄŻ
        buttons[count] = {"Snake", getImg("snake/snakeIcon"), switchSnake};
    }
#endif
#if PONG
    {
        debugLog("Adding pong");
        count = count + 1;
        buttons[count] = {MENU_PONG, getImg("pong"), switchPong};
    }
#endif
#if VIDEO_PLAYER
    {
        debugLog("Adding video player");
        count = count + 1;
        buttons[count] = {MENU_VIDEO_PLAYER, getImg("videoIcon"), switchVideoMenu};
    }
#endif
#if CONWAY
    {
        debugLog("Adding Conway");
        count = count + 1;
        buttons[count] = {MENU_CONWAY, getImg("conway"), switchConway};
    }
#endif
#if RGB_DIODE
    {
        debugLog("Adding party");
        count = count + 1;
        buttons[count] = {MENU_PARTY, getImg("rgb"), switchParty};
    }
#endif
    count = count + 1;
    initMenu(buttons, count, MENU_GAMES, 1);
}

void toggleDisableVibrationsEntry()
{
    toggleAllVibration();
    initpowerMenu();
}

void toggleWakeUpEntry()
{
    toggleWakeUp();
    initpowerMenu();
}

void initpowerMenu()
{
    int count = -1;
    entryMenu buttons[2];
    {
        count = count + 1;
        ImageDef *image;
        if (rM.disableAllVibration == true)
        {
            image = getImg("accept");
        }
        else
        {
            image = getImg("cross");
        }
        buttons[count] = {MENU_VIBRATIONS_DIS, image, toggleDisableVibrationsEntry};
    }
    {
        count = count + 1;
        ImageDef *image;
        if (rM.disableWakeUp == true)
        {
            image = getImg("accept");
        }
        else
        {
            image = getImg("cross");
        }
        buttons[count] = {MENU_WAKE_UP_DIS, image, toggleWakeUpEntry};
    }
    count = count + 1;
    initMenu(buttons, count, MENU_POWER, 1);
}