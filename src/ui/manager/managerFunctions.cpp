#include "manager.h"
#include "rtcMem.h"

void initMainMenu()
{
    int count = -1;
    entryMenu buttons[11];
#if CALENDAR
    {
        count = count + 1;
        buttons[count] = {"Calendar", getImg("calendar"), switchCalendarDateMenu};
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
        buttons[count] = {"Select book", getImg("book"), switchBookSelector};
    }
#endif
#if WEATHER_INFO
    {
        count = count + 1;
        buttons[count] = {"Weather", getImg("weather"), switchWeatherMenu};
    }
#endif
    {
        count = count + 1;
        buttons[count] = {"Settings", getImg("settings"), switchSettingsMenu};
    }
#if VAULT
    {
        count = count + 1;
        buttons[count] = {"Vault", getImg("vault"), switchVault};
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
        buttons[count] = {"Wifi tool: " + wifiToolStatus(), getImg("wifiTool"), wifiToolFunc};
    }
#endif
#if APPLE_JOKE
    {
        debugLog("Adding apple joke to menu");
        count = count + 1;
        buttons[count] = {"Eating apples", getImg("apple"), switchApple};
    }
    {
        debugLog("Adding apple joke to menu");
        count = count + 1;
        buttons[count] = {"Smashing apples", getImg("apple"), switchApple2};
    }
#endif
    {
        debugLog("Adding games");
        count = count + 1;
        buttons[count] = {"Games", getImg("games"), switchGamesMenu};
    }
#if CREDITS
    {
        debugLog("Adding Credits");
        count = count + 1;
        buttons[count] = {"Credits", getImg("credits/qr"), switchCredits};
    }
#endif
    count = count + 1;
    initMenu(buttons, count, "Main menu", 1);
}

#if DEBUG_MENUS
void initDebugMenu()
{
    int count = -1;
    entryMenu buttons[5];
    {
        debugLog("Adding clock to menu");
        count = count + 1;
        buttons[count] = {"Clock", &emptyImgPack, switchClockDebug};
    }
    {
        debugLog("Adding general to menu");
        count = count + 1;
        buttons[count] = {"Hardware", &emptyImgPack, switchGeneralDebug};
    }
    {
        debugLog("Adding battery to menu");
        count = count + 1;
        buttons[count] = {"Battery", &emptyImgPack, switchBatteryDebug};
    }
    {
        debugLog("Adding git to menu");
        count = count + 1;
        buttons[count] = {"Git", &emptyImgPack, switchGitDebug};
    }
#if FONT_MENU_ENABLED
    {
        debugLog("Adding font preview to menu");
        count = count + 1;
        buttons[count] = {"Font preview", &emptyImgPack, switchFontsPreviewMenu};
    }
#endif
    count = count + 1;
    initMenu(buttons, count, "Debug menu", 1);
}
#endif

void initSettingsMenu()
{
    int count = -1;
    entryMenu buttons[5];
    {
        debugLog("Adding wifi to menu");
        count = count + 1;
        buttons[count] = {"Wifi", getImg("wifiIcon"), switchWifiDebug};
    }
#if DEBUG_MENUS
    {
        count = count + 1;
        buttons[count] = {"Debug", getImg("debug"), switchDebugMenu};
    }
#endif
    {
        count = count + 1;
        buttons[count] = {"Power settings", getImg("battery"), switchPowerMenu};
    }
    {
        count = count + 1;
        buttons[count] = {"Change watchface", getImg("watchfaceIcon"), switchWatchfaceSelectorMenu};
    }
    count = count + 1;
    initMenu(buttons, count, "Settings menu", 1);
}

void initGamesMenu()
{
    int count = -1;
    entryMenu buttons[5];
#if TETRIS
    {
        debugLog("Adding Tetris");
        count = count + 1;
        buttons[count] = {"Blockchy", getImg("tetris/tetrisIcon"), switchTetris};
    }
#endif
#if PONG
    {
        debugLog("Adding pong");
        count = count + 1;
        buttons[count] = {"Pong", getImg("pong"), switchPong};
    }
#endif
#if VIDEO_PLAYER
    {
        debugLog("Adding video player");
        count = count + 1;
        buttons[count] = {"Video player", getImg("videoIcon"), switchVideoMenu};
    }
#endif
#if CONWAY
    {
        debugLog("Adding Conway");
        count = count + 1;
        buttons[count] = {"Conway", getImg("conway"), switchConway};
    }
#endif
#if RGB_DIODE
    {
        debugLog("Adding party");
        count = count + 1;
        buttons[count] = {"Party", getImg("rgb"), switchParty};
    }
#endif
    count = count + 1;
    initMenu(buttons, count, "Games menu", 1);
}

void toggleDisableVibrationsEntry()
{
    toggleAllVibration();
    initpowerMenu();
}

void toggleWakeUpEntry()
{
    toggleWakeUp();
    /*
    if (rM.disableWakeUp == false)
    {
        wakeUpManageRTC();
    }
    else
    {
        rM.SRTC.clearAlarm();
    }
    */
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
        buttons[count] = {"Vibrations disabled", image, toggleDisableVibrationsEntry};
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
        buttons[count] = {"Wake up disabled", image, toggleWakeUpEntry};
    }
    count = count + 1;
    initMenu(buttons, count, "Power menu", 1);
}