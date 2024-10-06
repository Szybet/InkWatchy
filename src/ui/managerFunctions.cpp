#include "manager.h"

void initMainMenu()
{
    int count = -1;
    entryMenu buttons[9];
#if CALENDAR
    {
        count = count + 1;
        buttons[count] = {"Calendar", getImg("calendar"), switchCalendarMenu};
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
        buttons[count] = {"Debug", getImg("debug"), switchDebugMenu};
    }
    {
        count = count + 1;
        buttons[count] = {"Power settings", getImg("battery"), switchPowerMenu};
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

    count = count + 1;
    initMenu(buttons, count, "Main menu", 1);
}

void initDebugMenu()
{
    int count = -1;
    entryMenu buttons[8];
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
    {
        debugLog("Adding wifi to menu");
        count = count + 1;
        buttons[count] = {"Wifi", &emptyImgPack, switchWifiDebug};
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