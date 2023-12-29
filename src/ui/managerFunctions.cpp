#include "manager.h"

void initMainMenu()
{
#if BOOK
    entryMenu buttons[3] = {{"Debug", &debugImgPack, switchDebugMenu}, {"Weather", &weatherImgPack, switchWeatherMenu}, {"Book " + bookGetPages(), &bookImgPack, switchBook}};
    initMenu(buttons, 3, "Main menu", 1);
#else
    entryMenu buttons[2] = {{"Debug", &debugImgPack, switchDebugMenu}, {"Weather", &weatherImgPack, switchWeatherMenu}};
    initMenu(buttons, 2, "Main menu", 1);
#endif

}

void initDebugMenu()
{
    entryMenu buttons[3] = {{"General", &emptyImgPack, switchGeneralDebug}, {"Battery", &emptyImgPack, switchBatteryDebug}, {"Wifi", &emptyImgPack, switchWifiDebug}};
    initMenu(buttons, 3, "Debug menu", 1);
}

void initWeatherMenu()
{
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
            breakTime(weatherDataDays[i][j].dt, times);
            if (day != times.Day)
            {
                day = times.Day;
                buttons[c].text = String(times.Day) + "." + String(times.Month) + "." + String(tmYearToCalendar(times.Year));
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
    if (lastMenuSelected.length() >= 5 && lastMenuSelected[2] == '.' && lastMenuSelected[5] == '.')
    {
        int dayIndex = lastMenuSelected.indexOf(".");
        weatherDayChoosed = lastMenuSelected.substring(0, dayIndex).toInt();
        debugLog("weatherDayChoosed: " + String(weatherDayChoosed));
    }

    entryMenu buttons[9] = {{"Temperature", &emptyImgPack, showTemp}, {"Pressure", &emptyImgPack, showPressure}, {"Humidity", &emptyImgPack, showHumidity}, {"Weather conditions", &emptyImgPack, showWeatherCond}, {"Cloudiness", &emptyImgPack, showClouds}, {"Wind speed", &emptyImgPack, showWindSpeed}, {"Wind guts", &emptyImgPack, showWindGuts}, {"Visibility", &emptyImgPack, showVisibility}, {"% of precipitation", &emptyImgPack, showPop}};
    initMenu(buttons, 9, "Weather stat", 1);
}
