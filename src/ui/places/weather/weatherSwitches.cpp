#include "weatherSwitches.h"

#if WEATHER_INFO

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
    debugLog("There are so many days: " + String(days));

    if (days == 0)
    {
        overwriteSwitch(textDialog);
        showTextDialog("Weather not available");
        return;
    }

    long daysUnixList[days] = {0};
    File root = LittleFS.open(WEATHER_HOURLY_DIR);
    File file = root.openNextFile();
    u8_t c = 0;
    while (file)
    {
        if (file.isDirectory() == false)
        {
            String unixTmp = String(file.name());
            debugLog("Got unix file name: " + unixTmp);
            daysUnixList[c] = unixTmp.toInt();
            c = c + 1;
        }
        file = root.openNextFile();
    }
    root.close();
    file.close();

    // Sort daysListPure
    // for (u8_t i = 0; i < days; i++)
    // {
    //     debugLog("i: " + String(daysUnixList[i]));
    // }
    // debugLog("Sorting!");
    std::sort(daysUnixList, daysUnixList + days);
    // for (u8_t i = 0; i < days; i++)
    // {
    //     debugLog("i: " + String(daysUnixList[i]));
    // }

    // First, get the current day from that list
    uint theUnixFromListIndex = 9999;
    long currentUnix = getUnixTime(timeRTCLocal);
    uint smallestDifference = -1;
    for (u8_t i = 0; i < days; i++)
    {
        if (llabs(currentUnix - daysUnixList[i]) < smallestDifference && day(currentUnix) == day(daysUnixList[i]))
        {
            theUnixFromListIndex = i;
            smallestDifference = llabs(currentUnix - daysUnixList[i]);
        }
    }

    debugLog("Got current unix from list: " + String(theUnixFromListIndex));

    if (theUnixFromListIndex == 9999)
    {
        debugLog("Failed to find unix with the same date, skipping further sorting");
        std::reverse(daysUnixList, daysUnixList + days);
    }
    else
    {
        // Move arround the list
        long daysUnixListTmp[days];
        // Copy everything before theUnixFromListIndex to daysUnixListTmp
        std::copy(daysUnixList, daysUnixList + theUnixFromListIndex, daysUnixListTmp);
        // Copy everything after theUnixFromListIndex to the beginning of daysUnixList
        std::copy(daysUnixList + theUnixFromListIndex, daysUnixList + days, daysUnixList);
        // Copy elements from daysUnixListTmp to the remaining space of daysUnixList
        std::copy(daysUnixListTmp, daysUnixListTmp + theUnixFromListIndex, daysUnixList + days - theUnixFromListIndex);
    }

    entryMenu buttons[days];
    for (u8_t i = 0; i < days; i++)
    {
        buttons[i] = {unixToDate(daysUnixList[i]), &emptyImgPack, switchWeatherSelectorMenu};
    }

    initMenu(buttons, days, "Select date", 1);
}

char weatherDayChoosed[12];
void initWeatherConditionMenu()
{
    debugLog("initWeatherConditionMenu called");
    // Check if the last menu item name is a date
    int dayIndex = lastMenuSelected.indexOf(".");
    // if (lastMenuSelected.length() >= 5 && lastMenuSelected[2] == '.' && lastMenuSelected[5] == '.')
    if (dayIndex != -1)
    {
        lastMenuSelected.toCharArray(weatherDayChoosed, 12);
        weatherDayChoosed[11] = '\0';
    }
    else if (String(weatherDayChoosed).indexOf(".") == -1)
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

    entryMenu buttons[9] = {{"Temperature", &emptyImgPack, showTemp}, {"Pressure", &emptyImgPack, showPressure}, {"Humidity", &emptyImgPack, showHumidity}, {"Weather conditions", &emptyImgPack, showWeatherCond}, {"Cloudiness", &emptyImgPack, showClouds}, {"Wind speed", &emptyImgPack, showWindSpeed}, {"Wind gusts", &emptyImgPack, showWindGuts}, {"Visibility", &emptyImgPack, showVisibility}, {"% of precipitation", &emptyImgPack, showPop}};
    initMenu(buttons, 9, "Weather stat", 1);
}

OM_HourlyForecastReturn generalWeatherGetData()
{
    String weatherDay = String(weatherDayChoosed);
    debugLog("Showing temperature for day: " + weatherDay);

    bufSize weatherData = fsGetBlob(String(dateToUnix(weatherDay)), String(WEATHER_HOURLY_DIR) + "/");
    debugLog("Weather size is: " + String(weatherData.size) + " While is should be: " + String(sizeof(OM_HourlyForecast)));
    OM_HourlyForecastReturn forecast = {};
    if (weatherData.size != sizeof(OM_HourlyForecast))
    {
        debugLog("Weather data is bad.");
        free(weatherData.buf);
        overwriteSwitch(textDialog);
        showTextDialog("Weather corrupted");
        forecast.fine = false;
        return forecast;
    }
    memcpy(&forecast.data, weatherData.buf, weatherData.size);
    free(weatherData.buf);

    forecast.fine = true;
    return forecast;
}

OM_OneHourWeather weatherGetDataHourly(uint8_t hourOffset)
{
    String unixTimeWeather = String(simplifyUnix(getUnixTime(timeRTCLocal)));
    debugLog("Getting weather for unix: " + unixTimeWeather);
    bufSize weatherData = fsGetBlob(unixTimeWeather, String(WEATHER_HOURLY_DIR) + "/");
    debugLog("Weather size is: " + String(weatherData.size) + " While is should be: " + String(sizeof(OM_HourlyForecast)));
    OM_OneHourWeather forecast = {0};
    if (weatherData.size != sizeof(OM_HourlyForecast))
    {
        debugLog("Weather data is bad.");
        free(weatherData.buf);
        forecast.fine = false;
        return forecast;
    }
    OM_HourlyForecast *weatherDataWork = (OM_HourlyForecast *)weatherData.buf;
    int64_t unixNow = getUnixTime(timeRTCUTC0) + (60 * 60) * hourOffset; // Using utc0 as the stored data from open meteo uses utc0 unix timestamps for time
    int64_t smallestDiffUnix = 0;
    uint8_t smallestDiffIndex = 0;
    for (int i = 0; i < OM_WEATHER_MAX_HOURS; i++)
    {
        if (llabs(unixNow - (int64_t)weatherDataWork->hourly_time[i]) < llabs(unixNow - smallestDiffUnix))
        {
            smallestDiffUnix = weatherDataWork->hourly_time[i];
            smallestDiffIndex = i;
        }
        // debugLog("Current time: " + String(unixNow) + " choosed time: " + String(weatherDataWork->hourly_time[i]) + " diff: " + String(llabs(unixNow - weatherDataWork->hourly_time[i])));
    }
    debugLog("FINAL time: " + String(unixNow) + " choosed time: " + String(weatherDataWork->hourly_time[smallestDiffIndex]) + " diff: " + String(llabs(unixNow - weatherDataWork->hourly_time[smallestDiffIndex])) + " so it's index is: " + String(smallestDiffIndex));

    forecast.hourly_time = weatherDataWork->hourly_time[smallestDiffIndex];
    forecast.temp = weatherDataWork->temp[smallestDiffIndex];
    forecast.humidity = weatherDataWork->humidity[smallestDiffIndex];
    forecast.apparent_temp = weatherDataWork->apparent_temp[smallestDiffIndex];
    forecast.pressure = weatherDataWork->pressure[smallestDiffIndex];
    forecast.precipitation = weatherDataWork->precipitation[smallestDiffIndex];
    forecast.cloud_cover = weatherDataWork->cloud_cover[smallestDiffIndex];
    forecast.visibility = weatherDataWork->visibility[smallestDiffIndex];
    forecast.wind_speed = weatherDataWork->wind_speed[smallestDiffIndex];
    forecast.wind_deg = weatherDataWork->wind_deg[smallestDiffIndex];
    forecast.wind_gust = weatherDataWork->wind_gust[smallestDiffIndex];
    forecast.weather_code = weatherDataWork->weather_code[smallestDiffIndex];
    forecast.is_day = weatherDataWork->is_day[smallestDiffIndex];
    // Daily things!
    forecast.daily_time = weatherDataWork->daily_time[0];
    forecast.sunrise = weatherDataWork->sunrise[0];
    forecast.sunset = weatherDataWork->sunset[0];

    free(weatherData.buf);
    forecast.fine = true;
    return forecast;
}

void showTemp()
{
    OM_HourlyForecastReturn forecast = generalWeatherGetData();
    if (forecast.fine == false)
    {
        return;
    }
#if DEBUG
    debugLog("Dumping temp");
    for (int i = 0; i < OM_WEATHER_MAX_HOURS; i++)
    {
        debugLog(String(i) + ": " + String(forecast.data.temp[i]));
    }
#endif
    showChart(forecast.data.temp, OM_WEATHER_MAX_HOURS, "Temp / C");
    generalSwitch(ChartPlace);
}

void showPressure()
{
    OM_HourlyForecastReturn forecast = generalWeatherGetData();
    if (forecast.fine == false)
    {
        return;
    }
#if DEBUG
    debugLog("Dumping pressure");
    for (int i = 0; i < OM_WEATHER_MAX_HOURS; i++)
    {
        debugLog(String(i) + ": " + String(forecast.data.pressure[i]));
    }
#endif
    showChart(forecast.data.pressure, OM_WEATHER_MAX_HOURS, "Pressure / hPa");
    generalSwitch(ChartPlace);
}

void showHumidity()
{
    OM_HourlyForecastReturn forecast = generalWeatherGetData();
    if (forecast.fine == false)
    {
        return;
    }
#if DEBUG
    debugLog("Dumping humidity");
    for (int i = 0; i < OM_WEATHER_MAX_HOURS; i++)
    {
        debugLog(String(i) + ": " + String(forecast.data.humidity[i]));
    }
#endif
    float humidity[OM_WEATHER_MAX_HOURS] = {0};
    for (u8_t i = 0; i < OM_WEATHER_MAX_HOURS; i++)
    {
        humidity[i] = u8_t(forecast.data.humidity[i]);
    }
    showChart(humidity, OM_WEATHER_MAX_HOURS, "Humidity / %");
    generalSwitch(ChartPlace);
}

void showWeatherCond()
{
    OM_HourlyForecastReturn forecast = generalWeatherGetData();
    if (forecast.fine == false)
    {
        return;
    }
    String weatherCond[OM_WEATHER_MAX_HOURS];
    for (u8_t i = 0; i < OM_WEATHER_MAX_HOURS; i++)
    {
        weatherCond[i] = weatherConditionIdToStr(forecast.data.weather_code[i]);
    }
    textPage("Weather conditions", weatherCond, OM_WEATHER_MAX_HOURS, getFont("dogicapixel4"));
    generalSwitch(ChartPlace);
}

void showClouds()
{
    OM_HourlyForecastReturn forecast = generalWeatherGetData();
    if (forecast.fine == false)
    {
        return;
    }
#if DEBUG
    debugLog("Dumping clouds");
    for (int i = 0; i < OM_WEATHER_MAX_HOURS; i++)
    {
        debugLog(String(i) + ": " + String(forecast.data.cloud_cover[i]));
    }
#endif
    float clouds[OM_WEATHER_MAX_HOURS] = {0};
    for (u8_t i = 0; i < OM_WEATHER_MAX_HOURS; i++)
    {
        clouds[i] = float(forecast.data.cloud_cover[i]);
    }
    showChart(clouds, OM_WEATHER_MAX_HOURS, "Clouds / %");
    generalSwitch(ChartPlace);
}

void showWindSpeed()
{
    OM_HourlyForecastReturn forecast = generalWeatherGetData();
    if (forecast.fine == false)
    {
        return;
    }
#if DEBUG
    debugLog("Dumping wind speed");
    for (int i = 0; i < OM_WEATHER_MAX_HOURS; i++)
    {
        debugLog(String(i) + ": " + String(forecast.data.wind_speed[i]));
    }
#endif
    showChart(forecast.data.wind_speed, OM_WEATHER_MAX_HOURS, "Wind speed / km/h");
    generalSwitch(ChartPlace);
}

void showWindGuts()
{
    OM_HourlyForecastReturn forecast = generalWeatherGetData();
    if (forecast.fine == false)
    {
        return;
    }
#if DEBUG
    debugLog("Dumping wind guts");
    for (int i = 0; i < OM_WEATHER_MAX_HOURS; i++)
    {
        debugLog(String(i) + ": " + String(forecast.data.wind_gust[i]));
    }
#endif
    showChart(forecast.data.wind_gust, OM_WEATHER_MAX_HOURS, "Wind gusts / km/h");
    generalSwitch(ChartPlace);
}

void showVisibility()
{
    OM_HourlyForecastReturn forecast = generalWeatherGetData();
    if (forecast.fine == false)
    {
        return;
    }
#if DEBUG
    debugLog("Dumping visibility");
    for (int i = 0; i < OM_WEATHER_MAX_HOURS; i++)
    {
        debugLog(String(i) + ": " + String(forecast.data.visibility[i]));
    }
#endif
    float vis[OM_WEATHER_MAX_HOURS] = {0};
    for (u8_t i = 0; i < OM_WEATHER_MAX_HOURS; i++)
    {
        vis[i] = float(forecast.data.visibility[i]);
    }
    showChart(vis, OM_WEATHER_MAX_HOURS, "Visib. / m");
    generalSwitch(ChartPlace);
}

void showPop()
{
    OM_HourlyForecastReturn forecast = generalWeatherGetData();
    if (forecast.fine == false)
    {
        return;
    }
#if DEBUG
    debugLog("Dumping pop");
    for (int i = 0; i < OM_WEATHER_MAX_HOURS; i++)
    {
        debugLog(String(i) + ": " + String(forecast.data.precipitation[i]));
    }
#endif
    float pop[OM_WEATHER_MAX_HOURS] = {0};
    for (u8_t i = 0; i < OM_WEATHER_MAX_HOURS; i++)
    {
        pop[i] = float(forecast.data.precipitation[i]);
    }
    showChart(pop, OM_WEATHER_MAX_HOURS, "Preper. / %");
    generalSwitch(ChartPlace);
}
#endif