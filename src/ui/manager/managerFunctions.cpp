#include "manager.h"

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
#if PONG
    {
        debugLog("Adding pong");
        count = count + 1;
        buttons[count] = {"Pong", getImg("pong"), switchPong};
    }
#endif
#if CONWAY
    {
        debugLog("Adding Conway");
        count = count + 1;
        buttons[count] = {"Conway", getImg("conway"), switchConway};
    }
#endif
    count = count + 1;
    initMenu(buttons, count, "Main menu", 1);
}

#if DEBUG_MENUS
void initDebugMenu()
{
    int count = -1;
    entryMenu buttons[4];
    {
        debugLog("Adding general to menu");
        count = count + 1;
        buttons[count] = {"General", &emptyImgPack, switchGeneralDebug};
    }
    {
        debugLog("Adding battery to menu");
        count = count + 1;
        buttons[count] = {"Battery", &emptyImgPack, switchBatteryDebug};
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

void initSettingsMenu() {
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

void toggleDisableVibrationsEntry()
{
    toggleAllVibration();
    initpowerMenu();
}

void toggleWakeUpEntry()
{
    toggleWakeUp();
    /*
    if (disableWakeUp == false)
    {
        wakeUpManageRTC();
    }
    else
    {
        SRTC.clearAlarm();
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
        if (disableAllVibration == true)
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
        if (disableWakeUp == true)
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

#if FONT_MENU_ENABLED
void initFontMenu()
{
    /*
    entryMenu buttons[FONT_COUNT];
    for (int i = 0; i < FONT_COUNT; i++)
    {
        String str = fontListStr[i];
        if(str.length() > 20) {
            str = str.substring(0, 20);
        }
        buttons[i] = {str, &emptyImgPack, switchFontsPreview};
    }
    initMenu(buttons, FONT_COUNT, "Preview fonts");
    */
}
#endif