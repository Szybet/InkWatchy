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

void switchPowerMenu() {
    generalSwitch(powerMenu);
}

void switchVault() {
    generalSwitch(vault);
}

void switchBack() {
    setButton(Back);
}

void switchApple() {
    generalSwitch(apple);
}

void switchApple2() {
    generalSwitch(apple2);
}

#if FONT_MENU_ENABLED
void switchFontsPreview() {
    generalSwitch(fontPreview);
}

void switchFontsPreviewMenu() {
    generalSwitch(fontPreviewMenu);
}
#endif

void showTemp()
{
    debugLog("Launched");
    debugLog("weatherDayChoosed:" + String(weatherDayChoosed));
    int c = 0;
    float tempMax[MAX_DAYS * WEATHER_PER_DAY] = {0};
    float tempMin[MAX_DAYS * WEATHER_PER_DAY] = {0};
    tmElements_t times;
    for (int i = 0; i < MAX_DAYS; i++)
    {
        for (int j = 0; j < WEATHER_PER_DAY; j++)
        {
            time_t dtTime = weatherDataDays[i][j].dt;
            SRTC.doBreakTime(dtTime, times);
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
        // Make sure its in the right order... feels like temp can be always flipper
        if (tempMax[i] < tempMin[i])
        {
            float tmp = tempMax[i];
            tempMax[i] = tempMin[i];
            tempMin[i] = tmp;
        }
        if (tempMax[i] == tempMin[i])
        {
            debugLog("The temperature IS THE SAME!");
            tempMax[i] = tempMax[i] + 0.2;
        }
    }

#if DEBUG
    for (int i = 0; i < MAX_DAYS * WEATHER_PER_DAY; i++)
    {
        debugLog(String(i) + " : " + String(tempMax[i]));
        debugLog(String(i) + " : " + String(tempMin[i]));
    }
#endif

    showDoubleDataBarChart(tempMax, tempMin, c, "Temperature / C");
    generalSwitch(ChartPlace);
}

void showPressure()
{
    debugLog("Launched");
    int c = 0;
    float pressure[MAX_DAYS * WEATHER_PER_DAY] = {0};
    tmElements_t times;
    for (int i = 0; i < MAX_DAYS; i++)
    {
        for (int j = 0; j < WEATHER_PER_DAY; j++)
        {
            time_t dtTime = weatherDataDays[i][j].dt;
            SRTC.doBreakTime(dtTime, times);
            if (times.Day == weatherDayChoosed)
            {
                pressure[c] = weatherDataDays[i][j].pressure;
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

#if DEBUG
    for (int i = 0; i < MAX_DAYS * WEATHER_PER_DAY; i++)
    {
        debugLog(String(i) + " : " + String(pressure[i]));
    }
#endif

    showChart(pressure, c, "Pressure / hPa");
    generalSwitch(ChartPlace);
}

void showHumidity()
{
    debugLog("Launched");
    int c = 0;
    float humidity[MAX_DAYS * WEATHER_PER_DAY] = {0};
    tmElements_t times;
    for (int i = 0; i < MAX_DAYS; i++)
    {
        for (int j = 0; j < WEATHER_PER_DAY; j++)
        {
            time_t dtTime = weatherDataDays[i][j].dt;
            SRTC.doBreakTime(dtTime, times);
            if (times.Day == weatherDayChoosed)
            {
                humidity[c] = weatherDataDays[i][j].humidity;
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

#if DEBUG
    for (int i = 0; i < MAX_DAYS * WEATHER_PER_DAY; i++)
    {
        debugLog(String(i) + " : " + String(humidity[i]));
    }
#endif

    showChart(humidity, c, "Humidity / %");
    generalSwitch(ChartPlace);
}

void showWeatherCond()
{
    debugLog("Launched");
    int c = 0;
    String weatherCond[MAX_DAYS * WEATHER_PER_DAY];
    tmElements_t times;
    for (int i = 0; i < MAX_DAYS; i++)
    {
        for (int j = 0; j < WEATHER_PER_DAY; j++)
        {
            time_t dtTime = weatherDataDays[i][j].dt;
            SRTC.doBreakTime(dtTime, times);
            if (times.Day == weatherDayChoosed)
            {
                weatherCond[c] = weatherConditionIdToStr(weatherDataDays[i][j].weatherConditionId);
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

#if DEBUG
    for (int i = 0; i < MAX_DAYS * WEATHER_PER_DAY; i++)
    {
        debugLog(String(i) + " : " + String(weatherCond[i]));
    }
#endif

    textPage("Weather conditions", weatherCond, c);
    generalSwitch(ChartPlace);
}

void showClouds()
{
    debugLog("Launched");
    int c = 0;
    float clouds[MAX_DAYS * WEATHER_PER_DAY] = {0};
    tmElements_t times;
    for (int i = 0; i < MAX_DAYS; i++)
    {
        for (int j = 0; j < WEATHER_PER_DAY; j++)
        {
            time_t dtTime = weatherDataDays[i][j].dt;
            SRTC.doBreakTime(dtTime, times);
            if (times.Day == weatherDayChoosed)
            {
                clouds[c] = weatherDataDays[i][j].cloudsPerc;
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

#if DEBUG
    for (int i = 0; i < MAX_DAYS * WEATHER_PER_DAY; i++)
    {
        debugLog(String(i) + " : " + String(clouds[i]));
    }
#endif

    showChart(clouds, c, "Cloudiness / %");
    generalSwitch(ChartPlace);
}

void showWindSpeed()
{
    debugLog("Launched");
    int c = 0;
    float windSpeed[MAX_DAYS * WEATHER_PER_DAY] = {0};
    tmElements_t times;
    for (int i = 0; i < MAX_DAYS; i++)
    {
        for (int j = 0; j < WEATHER_PER_DAY; j++)
        {
            time_t dtTime = weatherDataDays[i][j].dt;
            SRTC.doBreakTime(dtTime, times);
            if (times.Day == weatherDayChoosed)
            {
                windSpeed[c] = weatherDataDays[i][j].windSpeed;
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

#if DEBUG
    for (int i = 0; i < MAX_DAYS * WEATHER_PER_DAY; i++)
    {
        debugLog(String(i) + " : " + String(windSpeed[i]));
    }
#endif

    showChart(windSpeed, c, "Wind speed / m/s");
    generalSwitch(ChartPlace);
}

void showWindGuts()
{
    debugLog("Launched");
    int c = 0;
    float windGusts[MAX_DAYS * WEATHER_PER_DAY] = {0};
    tmElements_t times;
    for (int i = 0; i < MAX_DAYS; i++)
    {
        for (int j = 0; j < WEATHER_PER_DAY; j++)
        {
            time_t dtTime = weatherDataDays[i][j].dt;
            SRTC.doBreakTime(dtTime, times);
            if (times.Day == weatherDayChoosed)
            {
                windGusts[c] = weatherDataDays[i][j].windGusts;
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

#if DEBUG
    for (int i = 0; i < MAX_DAYS * WEATHER_PER_DAY; i++)
    {
        debugLog(String(i) + " : " + String(windGusts[i]));
    }
#endif

    showChart(windGusts, c, "Wind gusts / m/s");
    generalSwitch(ChartPlace);
}

void showVisibility()
{
    debugLog("Launched");
    int c = 0;
    float visibility[MAX_DAYS * WEATHER_PER_DAY] = {0};
    tmElements_t times;
    for (int i = 0; i < MAX_DAYS; i++)
    {
        for (int j = 0; j < WEATHER_PER_DAY; j++)
        {
            time_t dtTime = weatherDataDays[i][j].dt;
            SRTC.doBreakTime(dtTime, times);
            if (times.Day == weatherDayChoosed)
            {
                visibility[c] = weatherDataDays[i][j].visibility;
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

#if DEBUG
    for (int i = 0; i < MAX_DAYS * WEATHER_PER_DAY; i++)
    {
        debugLog(String(i) + " : " + String(visibility[i]));
    }
#endif

    showChart(visibility, c, "Visibility / km");
    generalSwitch(ChartPlace);
}

void showPop()
{
    debugLog("Launched");
    int c = 0;
    float pop[MAX_DAYS * WEATHER_PER_DAY] = {0};
    tmElements_t times;
    for (int i = 0; i < MAX_DAYS; i++)
    {
        for (int j = 0; j < WEATHER_PER_DAY; j++)
        {
            time_t dtTime = weatherDataDays[i][j].dt;
            SRTC.doBreakTime(dtTime, times);
            if (times.Day == weatherDayChoosed)
            {
                pop[c] = weatherDataDays[i][j].pop;
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

#if DEBUG
    for (int i = 0; i < MAX_DAYS * WEATHER_PER_DAY; i++)
    {
        debugLog(String(i) + " : " + String(pop[i]));
    }
#endif

    showChart(pop, c, "% of precipitation");
    generalSwitch(ChartPlace);
}

void switchBook() {
    generalSwitch(book);
}
