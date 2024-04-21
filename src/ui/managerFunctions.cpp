#include "manager.h"

void initMainMenu()
{
    int count = -1;
    entryMenu buttons[8];
#if BOOK
    {
        count = count + 1;
        buttons[count] = {"Book " + bookGetPages(), &bookImgPack, switchBook};
    }
#endif
    {
        count = count + 1;
        buttons[count] = {"Weather", &weatherImgPack, switchWeatherMenu};
    }
    {
        count = count + 1;
        buttons[count] = {"Debug", &debugImgPack, switchDebugMenu};
    }
    {
        count = count + 1;
        buttons[count] = {"Power settings", &batteryImgPack, switchPowerMenu};
    }
#if VAULT
    {
        count = count + 1;
        buttons[count] = {"Vault", &vaultImgPack, switchVault};
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
        buttons[count] = {"Wifi tool: " + wifiToolStatus(), &wifiToolImgPack, wifiToolFunc};
    }
#endif
#if APPLE_JOKE
    {
        debugLog("Adding apple joke to menu");
        count = count + 1;
        buttons[count] = {"Eating apples", &appleImgPack, switchApple};
    }
    {
        debugLog("Adding apple joke to menu");
        count = count + 1;
        buttons[count] = {"Smashing apples", &appleImgPack, switchApple2};
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

void initWeatherMenu()
{
    loadWeatherData();
    if (isWeatherAvailable == false)
    {
        display.fillScreen(GxEPD_WHITE);
        simpleCenterText("Weather not available");
        disUp(true);
        overwriteSwitch(textDialog);
        return void();
    }

    int day = 0;
    int c = 0;
    entryMenu buttons[MAX_DAYS] = {};
    for (int i = 0; i < MAX_DAYS; i++)
    {
        for (int j = 0; j < WEATHER_PER_DAY; j++)
        {
            tmElements_t times;
            time_t dtTime = weatherDataDays[i][j].dt;
            SRTC.doBreakTime(dtTime, times);
            if (day != times.Day)
            {
                day = times.Day;
                String dayStr = String(times.Day);
                String monthStr = String(times.Month + 1);
                if (dayStr.length() == 1)
                {
                    dayStr = "0" + dayStr;
                }
                if (monthStr.length() == 1)
                {
                    monthStr = "0" + monthStr;
                }
                buttons[c].text = dayStr + "." + monthStr + "." + String(tmYearToCalendar(times.Year));
                buttons[c].image = &emptyImgPack;
                // Lambda doesn't work :( global values then... weatherDayChoosed
                /*
                auto lambdaFunction = [day]()
                {
                    switchWeatherSelectorMenu(day);
                };
                */
                buttons[c].function = switchWeatherSelectorMenu;
                c = c + 1;
            }
        }
    }
    initMenu(buttons, MAX_DAYS, "Weather info", 1);
}

int weatherDayChoosed;
void initWeatherConditionMenu()
{
    // Check if the last menu item name is a date
    int dayIndex = lastMenuSelected.indexOf(".");
    // if (lastMenuSelected.length() >= 5 && lastMenuSelected[2] == '.' && lastMenuSelected[5] == '.')
    if (dayIndex != -1)
    {
        weatherDayChoosed = lastMenuSelected.substring(0, dayIndex).toInt();
        debugLog("weatherDayChoosed: " + String(weatherDayChoosed));
    }
    else
    {
        debugLog("Error finding date for weather condition");
    }

    entryMenu buttons[9] = {{"Temperature", &emptyImgPack, showTemp}, {"Pressure", &emptyImgPack, showPressure}, {"Humidity", &emptyImgPack, showHumidity}, {"Weather conditions", &emptyImgPack, showWeatherCond}, {"Cloudiness", &emptyImgPack, showClouds}, {"Wind speed", &emptyImgPack, showWindSpeed}, {"Wind guts", &emptyImgPack, showWindGuts}, {"Visibility", &emptyImgPack, showVisibility}, {"% of precipitation", &emptyImgPack, showPop}};
    initMenu(buttons, 9, "Weather stat", 1);
}

void toggleDisableVibrationsEntry()
{
    toggleAllVibration();
    initpowerMenu();
}

void toggleWakeUpEntry()
{
    toggleWakeUp();
    if (disableWakeUp == false)
    {
        wakeUpManageRTC();
    }
    else
    {
        SRTC.clearAlarm();
    }
    initpowerMenu();
}

void initpowerMenu()
{
    int count = -1;
    entryMenu buttons[2];

    {
        count = count + 1;
        const ImageDef *image;
        if (disableAllVibration == true)
        {
            image = &acceptImgPack;
        }
        else
        {
            image = &crossImgPack;
        }
        buttons[count] = {"Vibrations disabled", image, toggleDisableVibrationsEntry};
    }
    {
        count = count + 1;
        const ImageDef *image;
        if (disableWakeUp == true)
        {
            image = &acceptImgPack;
        }
        else
        {
            image = &crossImgPack;
        }
        buttons[count] = {"Wake up disabled", image, toggleWakeUpEntry};
    }
    count = count + 1;
    initMenu(buttons, count, "Power menu", 1);
}

#if FONT_MENU_ENABLED
void initFontMenu()
{
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
}
#endif