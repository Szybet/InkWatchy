#include "manager.h"

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
    int days = 0;
    // I want a break here...
    while (true == true)
    {

        File root = LittleFS.open(WEATHER_HOURLY_DIR);
        if (!root)
        {
            debugLog("Failed to open directory" + String(WEATHER_HOURLY_DIR));
            break;
        }
        if (root.isDirectory() == false)
        {
            debugLog("Not a directory weather?");
            break;
        }
        File file = root.openNextFile();
        while (file)
        {
            if (file.isDirectory() == false)
            {
                days = days + 1;
            }
            file = root.openNextFile();
        }
        root.close();
        break;
    }
    debugLog("There are so many books: " + String(days));

    if (days == 0)
    {
        overwriteSwitch(textDialog);
        showTextDialog("Weather not available");
        return;
    }

    entryMenu buttons[days];
    File root = LittleFS.open(WEATHER_HOURLY_DIR);
    File file = root.openNextFile();
    uint8_t counter = 0;
    while (file)
    {
        if (file.isDirectory() == false)
        {
            buttons[counter] = {String(file.name()), &emptyImgPack, switchWeatherSelectorMenu};
            counter = counter + 1;
        }
        file = root.openNextFile();
    }
    root.close();
    initMenu(buttons, counter, "Select date", 1);
}

char weatherDayChoosed[12];
void initWeatherConditionMenu()
{
    debugLog("initWeatherConditionMenu called")
        // Check if the last menu item name is a date
        int dayIndex = lastMenuSelected.indexOf(".");
    // if (lastMenuSelected.length() >= 5 && lastMenuSelected[2] == '.' && lastMenuSelected[5] == '.')
    if (dayIndex != -1)
    {
        lastMenuSelected.toCharArray(weatherDayChoosed, 12);
        weatherDayChoosed[11] = '\0';
    }
    else
    {
        debugLog("Error finding date for weather condition");
        overwriteSwitch(textDialog);
        showTextDialog("Date is wrong?");
        return;
    }

    // Temp
    // pressure
    // humidity
    // weather code
    // cloud cover
    // wind speed
    // wind gust
    // visibility
    // precipitation

    entryMenu buttons[9] = {{"Temperature", &emptyImgPack, showTemp}, {"Pressure", &emptyImgPack, showPressure}, {"Humidity", &emptyImgPack, showHumidity}, {"Weather conditions", &emptyImgPack, showWeatherCond}, {"Cloudiness", &emptyImgPack, showClouds}, {"Wind speed", &emptyImgPack, showWindSpeed}, {"Wind guts", &emptyImgPack, showWindGuts}, {"Visibility", &emptyImgPack, showVisibility}, {"% of precipitation", &emptyImgPack, showPop}};
    initMenu(buttons, 9, "Weather stat", 1);
}

OM_HourlyForecast generalWeatherGetData()
{
    String weatherDay = String(weatherDayChoosed);
    debugLog("Showing temperature for day: " + weatherDay);

    bufSize weatherData = fsGetBlob(weatherDay, String(WEATHER_HOURLY_DIR) + "/");
    debugLog("Weather size is: " + String(weatherData.size) + " While is should be: " + String(sizeof(OM_HourlyForecast)));
    OM_HourlyForecast forecast = {};
    if (weatherData.size != sizeof(OM_HourlyForecast))
    {
        debugLog("Weather data is bad.");
        free(weatherData.buf);
        overwriteSwitch(textDialog);
        showTextDialog("Weather corrupted");
        return forecast;
    }
    memcpy(&forecast, weatherData.buf, weatherData.size);
    free(weatherData.buf);

    return forecast;
}

void showTemp()
{
    OM_HourlyForecast forecast = generalWeatherGetData();
#if DEBUG
    debugLog("Dumping temp");
    for (int i = 0; i < OM_WEATHER_MAX_HOURS; i++)
    {
        debugLog(String(i) + ": " + String(forecast.temp[i]));
    }
#endif
    showChart(forecast.temp, OM_WEATHER_MAX_HOURS, "Temp / C");
}

void showPressure()
{
    OM_HourlyForecast forecast = generalWeatherGetData();
#if DEBUG
    debugLog("Dumping pressure");
    for (int i = 0; i < OM_WEATHER_MAX_HOURS; i++)
    {
        debugLog(String(i) + ": " + String(forecast.temp[i]));
    }
#endif
    showChart(forecast.pressure, OM_WEATHER_MAX_HOURS, "Pressure / hPa");
}

void showHumidity()
{
    OM_HourlyForecast forecast = generalWeatherGetData();
#if DEBUG
    debugLog("Dumping humidity");
    for (int i = 0; i < OM_WEATHER_MAX_HOURS; i++)
    {
        debugLog(String(i) + ": " + String(forecast.temp[i]));
    }
#endif
    float humidity[OM_WEATHER_MAX_HOURS] = {0};
    for (u8_t i = 0; i < OM_WEATHER_MAX_HOURS; i++)
    {
        humidity[i] = u8_t(forecast.humidity[i]);
    }
    showChart(humidity, OM_WEATHER_MAX_HOURS, "Humidity / %");
}

void showWeatherCond()
{
    OM_HourlyForecast forecast = generalWeatherGetData();
    String weatherCond[OM_WEATHER_MAX_HOURS];
    for(u8_t i = 0; i < OM_WEATHER_MAX_HOURS; i++) {
        weatherCond[i] = weatherConditionIdToStr(forecast.weather_code[i]);
    }
    textPage("Weather conditions", weatherCond, OM_WEATHER_MAX_HOURS);
    generalSwitch(ChartPlace);
}

void showClouds()
{
    OM_HourlyForecast forecast = generalWeatherGetData();
#if DEBUG
    debugLog("Dumping clouds");
    for (int i = 0; i < OM_WEATHER_MAX_HOURS; i++)
    {
        debugLog(String(i) + ": " + String(forecast.cloud_cover[i]));
    }
#endif
    float clouds[OM_WEATHER_MAX_HOURS] = {0};
    for (u8_t i = 0; i < OM_WEATHER_MAX_HOURS; i++)
    {
        clouds[i] = float(forecast.cloud_cover[i]);
    }
    showChart(clouds, OM_WEATHER_MAX_HOURS, "Clouds / %");
}

void showWindSpeed()
{
    OM_HourlyForecast forecast = generalWeatherGetData();
#if DEBUG
    debugLog("Dumping wind speed");
    for (int i = 0; i < OM_WEATHER_MAX_HOURS; i++)
    {
        debugLog(String(i) + ": " + String(forecast.wind_speed[i]));
    }
#endif
    showChart(forecast.wind_speed, OM_WEATHER_MAX_HOURS, "Wind speed / km/h");
}

void showWindGuts()
{
    OM_HourlyForecast forecast = generalWeatherGetData();
#if DEBUG
    debugLog("Dumping wind guts");
    for (int i = 0; i < OM_WEATHER_MAX_HOURS; i++)
    {
        debugLog(String(i) + ": " + String(forecast.wind_gust[i]));
    }
#endif
    showChart(forecast.wind_gust, OM_WEATHER_MAX_HOURS, "Wind guts / km/h");
}

void showVisibility()
{
    OM_HourlyForecast forecast = generalWeatherGetData();
#if DEBUG
    debugLog("Dumping visibility");
    for (int i = 0; i < OM_WEATHER_MAX_HOURS; i++)
    {
        debugLog(String(i) + ": " + String(forecast.visibility[i]));
    }
#endif
    float vis[OM_WEATHER_MAX_HOURS] = {0};
    for (u8_t i = 0; i < OM_WEATHER_MAX_HOURS; i++)
    {
        vis[i] = float(forecast.visibility[i]);
    }
    showChart(vis, OM_WEATHER_MAX_HOURS, "Visib. / m");
}

void showPop()
{
    OM_HourlyForecast forecast = generalWeatherGetData();
#if DEBUG
    debugLog("Dumping pop");
    for (int i = 0; i < OM_WEATHER_MAX_HOURS; i++)
    {
        debugLog(String(i) + ": " + String(forecast.precipitation[i]));
    }
#endif
    float pop[OM_WEATHER_MAX_HOURS] = {0};
    for (u8_t i = 0; i < OM_WEATHER_MAX_HOURS; i++)
    {
        pop[i] = float(forecast.precipitation[i]);
    }
    showChart(pop, OM_WEATHER_MAX_HOURS, "Preper. / %");
}