#include "manager.h"

void generalSwitch(UiPlace place)
{
    currentPlaceIndex += 1;
    placeTree[currentPlaceIndex] = place;
}

void overwriteSwitch(UiPlace place)
{
    placeTree[currentPlaceIndex] = place;
}

void switchDebugMenu()
{
    generalSwitch(debugMenu);
}

void switchGeneralDebug()
{
    generalSwitch(generalDebug);
}

void switchBatteryDebug()
{
    generalSwitch(batteryDebug);
}

void switchWifiDebug()
{
    generalSwitch(wifiDebug);
}

void switchWeatherMenu()
{
    generalSwitch(weatherMenu);
}

void switchWeatherSelectorMenu()
{
    generalSwitch(weatherConditionMenu);
}

void showTemp()
{
    debugLog("Launched");
    debugLog("weatherDayChoosed:" + String(weatherDayChoosed));
    int c = 0;
    float tempMax[MAX_DAYS * WEATHER_PER_DAY];
    float tempMin[MAX_DAYS * WEATHER_PER_DAY];
    tmElements_t times;
    for (int i = 0; i < MAX_DAYS; i++)
    {
        for (int j = 0; j < WEATHER_PER_DAY; j++)
        {
            breakTime(weatherDataDays[i][j].dt, times);
            if (times.Day == weatherDayChoosed)
            {
                tempMax[c] = weatherDataDays[i][j].temp;
                tempMin[c] = weatherDataDays[i][j].minTemp;
                c = c + 1;
            }
            if (times.Day > weatherDayChoosed)
            {
                break;
            }
        }
        if (times.Day > weatherDayChoosed)
        {
            break;
        }
    }

    for (int i = 0; i < c; i++)
    {
        if(tempMax[i] < tempMin[i]) {
            float tmp = tempMax[i];
            tempMax[i] = tempMin[i];
            tempMin[i] = tmp;
        }
    }
    
    showDoubleDataBarChart(tempMax, tempMin, c, "Temperature");
    generalSwitch(ChartPlace);
}

void showPressure()
{
    debugLog("Launched");
}

void showHumidity()
{
    debugLog("Launched");
}

void showWeatherCond()
{
    debugLog("Launched");
}

void showClouds()
{
    debugLog("Launched");
}

void showWindSpeed()
{
    debugLog("Launched");
}

void showWindGuts()
{
    debugLog("Launched");
}

void showVisibility()
{
    debugLog("Launched");
}

void showPop()
{
    debugLog("Launched");
}
