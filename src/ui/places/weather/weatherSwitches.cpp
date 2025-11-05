#include "weatherSwitches.h"
#include "weatherQuality.h"

#if WEATHER_INFO

void initWeatherMenu()
{
    // Check if we have both weather and air quality data
    bool hasWeather = false;
    bool hasAirQuality = false;

    // Check for weather data
    File weatherRoot = LittleFS.open(WEATHER_HOURLY_DIR);
    if (weatherRoot && weatherRoot.isDirectory())
    {
        File file = weatherRoot.openNextFile();
        hasWeather = (file && !file.isDirectory());
        weatherRoot.close();
    }

    // Check for air quality data
#if WEATHER_AIR_ENABLED
    File airQualityRoot = LittleFS.open(AIR_QUALITY_HOURLY_DIR);
    if (airQualityRoot && airQualityRoot.isDirectory())
    {
        File file = airQualityRoot.openNextFile();
        hasAirQuality = (file && !file.isDirectory());
        airQualityRoot.close();
    }
#endif

    // Create menu based on available data
    if (hasWeather && hasAirQuality)
    {
        int count = 1;
        entryMenu buttons[2];
        buttons[0] = {(WEATHER_AQ_MENU_WEATHER), &emptyImgPack, switchWeatherDateMenu};
#if WEATHER_AIR_ENABLED
        count = count + 1;
        buttons[1] = {(WEATHER_AQ_MENU_AIR_QUALITY), &emptyImgPack, switchAirQualityDateMenu};
#endif
        initMenu(buttons, count, (WEATHER_AQ_MENU_TITLE));
    }
    else if (hasWeather)
    {
        // If only weather data is available, go directly to weather date selection
        initWeatherDateMenu();
    }
#if WEATHER_AIR_ENABLED
    else if (hasAirQuality)
    {
        // If only air quality data is available, go directly to air quality date selection
        initAirQualityDateMenu();
    }
#endif
    else
    {
        // No data available
        overwriteSwitch(textDialog);
        showTextDialog((WEATHER_AQ_NOT_AVAILABLE), true);
    }
}

void initWeatherDateMenu()
{
    int days = 0;
    // Count weather data files
    File root = LittleFS.open(WEATHER_HOURLY_DIR);
    if (!root)
    {
        debugLog("Failed to open weather directory");
        overwriteSwitch(textDialog);
        showTextDialog("WEATHER_NOT_AVAILABLE", true);
        return;
    }

    if (!root.isDirectory())
    {
        debugLog("Not a directory: " + String(WEATHER_HOURLY_DIR));
        root.close();
        overwriteSwitch(textDialog);
        showTextDialog("WEATHER_NOT_AVAILABLE", true);
        return;
    }

    File file = root.openNextFile();
    while (file)
    {
        if (!file.isDirectory())
            days++;
        file = root.openNextFile();
    }
    root.close();

    if (days == 0)
    {
        overwriteSwitch(textDialog);
        showTextDialog("WEATHER_NOT_AVAILABLE", true);
        return;
    }

    // Get and sort dates (same logic as before)
    long daysUnixList[days] = {0};
    root = LittleFS.open(WEATHER_HOURLY_DIR);
    file = root.openNextFile();
    u8_t c = 0;
    while (file)
    {
        if (!file.isDirectory())
        {
            String unixTmp = String(file.name());
            daysUnixList[c] = unixTmp.toInt();
            c++;
        }
        file = root.openNextFile();
    }
    root.close();
    std::sort(daysUnixList, daysUnixList + days);

    // Find the current day (same logic as before)
    uint theUnixFromListIndex = 9999;
    long currentUnix = getUnixTime(timeRTCLocal);
    uint smallestDifference = -1;
    for (u8_t i = 0; i < days; i++)
    {
        if (llabs(currentUnix - daysUnixList[i]) < smallestDifference &&
            day(currentUnix) == day(daysUnixList[i]))
        {
            theUnixFromListIndex = i;
            smallestDifference = llabs(currentUnix - daysUnixList[i]);
        }
    }

    // Reorder the list with current day first (same logic as before)
    if (theUnixFromListIndex != 9999)
    {
        long daysUnixListTmp[days];
        std::copy(daysUnixList, daysUnixList + theUnixFromListIndex, daysUnixListTmp);
        std::copy(daysUnixList + theUnixFromListIndex, daysUnixList + days, daysUnixList);
        std::copy(daysUnixListTmp, daysUnixListTmp + theUnixFromListIndex, daysUnixList + days - theUnixFromListIndex);
    }
    else
    {
        std::reverse(daysUnixList, daysUnixList + days);
    }

    entryMenu buttons[days];
    for (u8_t i = 0; i < days; i++)
    {
        buttons[i] = {unixToDate(daysUnixList[i]), &emptyImgPack, switchWeatherConditionMenu};
    }

    initMenu(buttons, days, (WEATHER_SELECT_DATE), 1);
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
        showTextDialog(WEATHER_DATE_WRONG, true);
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

    entryMenu buttons[21] = {
        {WEATHER_MENU_TEMPERATURE, &emptyImgPack, showTemp},
        {WEATHER_MENU_PRESSURE, &emptyImgPack, showPressure},
        {WEATHER_MENU_HUMIDITY, &emptyImgPack, showHumidity},
        {WEATHER_MENU_CONDITIONS, &emptyImgPack, showWeatherCond},
        {WEATHER_MENU_CLOUDINESS, &emptyImgPack, showClouds},
        {WEATHER_MENU_WIND_SPEED, &emptyImgPack, showWindSpeed},
        {WEATHER_MENU_WIND_GUSTS, &emptyImgPack, showWindGuts},
        {WEATHER_MENU_VISIBILITY, &emptyImgPack, showVisibility},
        {WEATHER_MENU_PRECIPITATION, &emptyImgPack, showPop},

        {WEATHER_MENU_UV_INDEX, &emptyImgPack, showUVIndex},
        {WEATHER_MENU_UV_INDEX_CLEAR_SKY, &emptyImgPack, showUVIndexClearSky},
        {WEATHER_MENU_WET_BULB, &emptyImgPack, showWetBulb},
        {WEATHER_MENU_CAPE, &emptyImgPack, showCAPE},
        {WEATHER_MENU_DEW_POINT, &emptyImgPack, showDewPoint2m},
        {WEATHER_MENU_PRECIPITATION_AMOUNT, &emptyImgPack, showPrecipitationAmount},
        {WEATHER_MENU_SNOW_DEPTH, &emptyImgPack, showSnowDepth},
        {WEATHER_MENU_LIFTED_INDEX, &emptyImgPack, showLiftedIndex},
        {WEATHER_MENU_CONVECTIVE_INHIBITION, &emptyImgPack, showConvectiveInhibition},
        {WEATHER_MENU_TERRESTRIAL_RADIATION, &emptyImgPack, showTerrestrialRadiation},
        {WEATHER_MENU_DIFFUSE_RADIATION, &emptyImgPack, showDiffuseRadiation},
        {WEATHER_MENU_DIRECT_RADIATION, &emptyImgPack, showDirectRadiation},
        //     {WEATHER_MENU_TOTAL_WATER_VAPOUR, &emptyImgPack, showTotalColumnIntegratedWaterVapour}
    };
    initMenu(buttons, 21, WEATHER_STAT_TITLE, 1);
}

OM_HourlyForecastReturn generalWeatherGetData()
{
    String weatherDay = String(weatherDayChoosed);
    debugLog("Showing weather data for day: " + weatherDay);

    bufSize weatherData = fsGetBlob(String(dateToUnix(weatherDay)), String(WEATHER_HOURLY_DIR) + "/");
    debugLog("Weather size is: " + String(weatherData.size) + " While is should be: " + String(sizeof(OM_HourlyForecast)));
    OM_HourlyForecastReturn forecast = {};
    if (weatherData.size != sizeof(OM_HourlyForecast))
    {
        debugLog("Weather data is bad.");
        free(weatherData.buf);
        overwriteSwitch(textDialog);
        showTextDialog(WEATHER_CORRUPTED, true);
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
    forecast.wet_bulb = weatherDataWork->wet_bulb_temperature_2m[smallestDiffIndex];
    forecast.cape = weatherDataWork->cape[smallestDiffIndex];
    forecast.is_day = weatherDataWork->is_day[smallestDiffIndex];
    forecast.uv_index = weatherDataWork->uv_index[smallestDiffIndex];
    forecast.uv_index_clear_sky = weatherDataWork->uv_index_clear_sky[smallestDiffIndex];

    forecast.dew_point = weatherDataWork->dew_point[smallestDiffIndex];
    forecast.snow_depth = weatherDataWork->snow_depth[smallestDiffIndex];
    // forecast.vapour_pressure_deficit = weatherDataWork->vapour_pressure_deficit[smallestDiffIndex];
    // forecast.evapotranspiration = weatherDataWork->evapotranspiration[smallestDiffIndex];
    forecast.lifted_index = weatherDataWork->lifted_index[smallestDiffIndex];
    forecast.convective_inhibition = weatherDataWork->convective_inhibition[smallestDiffIndex];
    forecast.precipitation_amount = weatherDataWork->precipitation_amount[smallestDiffIndex];
    forecast.terrestrial_radiation = weatherDataWork->terrestrial_radiation[smallestDiffIndex];
    // forecast.global_tilted_irradiance = weatherDataWork->global_tilted_irradiance[smallestDiffIndex];
    // forecast.direct_normal_irradiance = weatherDataWork->direct_normal_irradiance[smallestDiffIndex];
    forecast.diffuse_radiation = weatherDataWork->diffuse_radiation[smallestDiffIndex];
    forecast.direct_radiation = weatherDataWork->direct_radiation[smallestDiffIndex];
    // forecast.shortwave_radiation = weatherDataWork->shortwave_radiation[smallestDiffIndex];
    // forecast.total_water_vapour = weatherDataWork->total_water_vapour[smallestDiffIndex];

    // Daily things!
    forecast.daily_time = weatherDataWork->daily_time[0];
    forecast.sunshine = weatherDataWork->sunshine_duration[0];
    forecast.sunrise = weatherDataWork->sunrise[0];
    forecast.sunset = weatherDataWork->sunset[0];

    free(weatherData.buf);
    forecast.fine = true;
    return forecast;
}

// Weather

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
    showChart(forecast.data.temp, OM_WEATHER_MAX_HOURS, WEATHER_CHART_TEMP);
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
    showChart(forecast.data.pressure, OM_WEATHER_MAX_HOURS, WEATHER_CHART_PRESSURE);
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
    showChart(humidity, OM_WEATHER_MAX_HOURS, WEATHER_CHART_HUMIDITY);
    generalSwitch(ChartPlace);
}

void showWeatherCond()
{
    OM_HourlyForecastReturn forecast = generalWeatherGetData();
    if (forecast.fine == false)
    {
        return;
    }
    String weatherCond;
    for (uint8_t i = 0; i < OM_WEATHER_MAX_HOURS; i++)
    {
        weatherCond = weatherCond + weatherConditionIdToStr(forecast.data.weather_code[i]) + "\n";
    }
    weatherCond.remove(weatherCond.length() - 1);
    generalSwitch(textDialog);
    showTextDialog(weatherCond, false, String(WEATHER_CONDITIONS_TITLE));
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
    showChart(clouds, OM_WEATHER_MAX_HOURS, WEATHER_CHART_CLOUDS);
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
    showChart(forecast.data.wind_speed, OM_WEATHER_MAX_HOURS, WEATHER_CHART_WIND_SPEED);
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
    showChart(forecast.data.wind_gust, OM_WEATHER_MAX_HOURS, WEATHER_CHART_WIND_GUSTS);
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
    showChart(vis, OM_WEATHER_MAX_HOURS, WEATHER_CHART_VISIBILITY);
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
    showChart(pop, OM_WEATHER_MAX_HOURS, WEATHER_CHART_PRECIPITATION);
    generalSwitch(ChartPlace);
}

void showUVIndex()
{
    OM_HourlyForecastReturn forecast = generalWeatherGetData();
    if (forecast.fine == false)
    {
        return;
    }
#if DEBUG
    debugLog("Dumping UV index");
    for (int i = 0; i < OM_WEATHER_MAX_HOURS; i++)
    {
        debugLog(String(i) + ": " + String(forecast.data.uv_index[i]));
    }
#endif
    float uv[OM_WEATHER_MAX_HOURS] = {0};
    for (u8_t i = 0; i < OM_WEATHER_MAX_HOURS; i++)
    {
        uv[i] = float(forecast.data.uv_index[i]);
    }
    showChart(uv, OM_WEATHER_MAX_HOURS, WEATHER_CHART_UV_INDEX);
    generalSwitch(ChartPlace);
}

void showUVIndexClearSky()
{
    OM_HourlyForecastReturn forecast = generalWeatherGetData();
    if (forecast.fine == false)
    {
        return;
    }
#if DEBUG
    debugLog("Dumping UV index clear sky");
    for (int i = 0; i < OM_WEATHER_MAX_HOURS; i++)
    {
        debugLog(String(i) + ": " + String(forecast.data.uv_index_clear_sky[i]));
    }
#endif
    float uv_clear[OM_WEATHER_MAX_HOURS] = {0};
    for (u8_t i = 0; i < OM_WEATHER_MAX_HOURS; i++)
    {
        uv_clear[i] = float(forecast.data.uv_index_clear_sky[i]);
    }
    showChart(uv_clear, OM_WEATHER_MAX_HOURS, WEATHER_CHART_UV_INDEX_CLEAR_SKY);
    generalSwitch(ChartPlace);
}

void showWetBulb()
{
    OM_HourlyForecastReturn forecast = generalWeatherGetData();
    if (forecast.fine == false)
    {
        return;
    }
#if DEBUG
    debugLog("Dumping wet bulb temperature");
    for (int i = 0; i < OM_WEATHER_MAX_HOURS; i++)
    {
        debugLog(String(i) + ": " + String(forecast.data.wet_bulb_temperature_2m[i]));
    }
#endif
    float wet_bulb[OM_WEATHER_MAX_HOURS] = {0};
    for (u8_t i = 0; i < OM_WEATHER_MAX_HOURS; i++)
    {
        wet_bulb[i] = float(forecast.data.wet_bulb_temperature_2m[i]);
    }
    showChart(wet_bulb, OM_WEATHER_MAX_HOURS, WEATHER_CHART_WET_BULB);
    generalSwitch(ChartPlace);
}

void showCAPE()
{
    OM_HourlyForecastReturn forecast = generalWeatherGetData();
    if (forecast.fine == false)
    {
        return;
    }
#if DEBUG
    debugLog("Dumping CAPE");
    for (int i = 0; i < OM_WEATHER_MAX_HOURS; i++)
    {
        debugLog(String(i) + ": " + String(forecast.data.cape[i]));
    }
#endif
    float cape[OM_WEATHER_MAX_HOURS] = {0};
    for (u8_t i = 0; i < OM_WEATHER_MAX_HOURS; i++)
    {
        cape[i] = float(forecast.data.cape[i]);
    }
    showChart(cape, OM_WEATHER_MAX_HOURS, WEATHER_CHART_CAPE);
    generalSwitch(ChartPlace);
}

void showDewPoint2m()
{
    OM_HourlyForecastReturn forecast = generalWeatherGetData();
    if (forecast.fine == false)
    {
        return;
    }
#if DEBUG
    debugLog("Dumping dew_point_2m");
    for (int i = 0; i < OM_WEATHER_MAX_HOURS; i++)
    {
        debugLog(String(i) + ": " + String(forecast.data.dew_point[i]));
    }
#endif
    float dew[OM_WEATHER_MAX_HOURS] = {0};
    for (u8_t i = 0; i < OM_WEATHER_MAX_HOURS; i++)
    {
        dew[i] = float(forecast.data.dew_point[i]);
    }
    showChart(dew, OM_WEATHER_MAX_HOURS, WEATHER_CHART_DEW_POINT);
    generalSwitch(ChartPlace);
}

void showPrecipitationAmount()
{
    OM_HourlyForecastReturn forecast = generalWeatherGetData();
    if (forecast.fine == false)
    {
        return;
    }
#if DEBUG
    debugLog("Dumping precipitation amount");
    for (int i = 0; i < OM_WEATHER_MAX_HOURS; i++)
    {
        debugLog(String(i) + ": " + String(forecast.data.precipitation_amount[i]));
    }
#endif
    float precip[OM_WEATHER_MAX_HOURS] = {0};
    for (u8_t i = 0; i < OM_WEATHER_MAX_HOURS; i++)
    {
        precip[i] = float(forecast.data.precipitation_amount[i]);
    }
    showChart(precip, OM_WEATHER_MAX_HOURS, WEATHER_CHART_PRECIPITATION_AMOUNT);
    generalSwitch(ChartPlace);
}

void showSnowDepth()
{
    OM_HourlyForecastReturn forecast = generalWeatherGetData();
    if (forecast.fine == false)
    {
        return;
    }
#if DEBUG
    debugLog("Dumping snow_depth");
    for (int i = 0; i < OM_WEATHER_MAX_HOURS; i++)
    {
        debugLog(String(i) + ": " + String(forecast.data.snow_depth[i]));
    }
#endif
    float snow[OM_WEATHER_MAX_HOURS] = {0};
    for (u8_t i = 0; i < OM_WEATHER_MAX_HOURS; i++)
    {
        snow[i] = float(forecast.data.snow_depth[i]);
    }
    showChart(snow, OM_WEATHER_MAX_HOURS, WEATHER_CHART_SNOW_DEPTH);
    generalSwitch(ChartPlace);
}

/* void showVapourPressureDeficit()
{
    OM_HourlyForecastReturn forecast = generalWeatherGetData();
    if (forecast.fine == false)
    {
        return;
    }
#if DEBUG
    debugLog("Dumping vapour_pressure_deficit");
    for (int i = 0; i < OM_WEATHER_MAX_HOURS; i++)
    {
        debugLog(String(i) + ": " + String(forecast.data.vapour_pressure_deficit[i]));
    }
#endif
    float vpd[OM_WEATHER_MAX_HOURS] = {0};
    for (u8_t i = 0; i < OM_WEATHER_MAX_HOURS; i++)
    {
        vpd[i] = float(forecast.data.vapour_pressure_deficit[i]);
    }
    showChart(vpd, OM_WEATHER_MAX_HOURS, WEATHER_CHART_VAPOUR_PRESSURE_DEFICIT);
    generalSwitch(ChartPlace);
}
*/

/* void showEvapotranspiration()
{
    OM_HourlyForecastReturn forecast = generalWeatherGetData();
    if (forecast.fine == false)
    {
        return;
    }
#if DEBUG
    debugLog("Dumping evapotranspiration");
    for (int i = 0; i < OM_WEATHER_MAX_HOURS; i++)
    {
        debugLog(String(i) + ": " + String(forecast.data.evapotranspiration[i]));
    }
#endif
    float et[OM_WEATHER_MAX_HOURS] = {0};
    for (u8_t i = 0; i < OM_WEATHER_MAX_HOURS; i++)
    {
        et[i] = float(forecast.data.evapotranspiration[i]);
    }
    showChart(et, OM_WEATHER_MAX_HOURS, WEATHER_CHART_EVAPOTRANSPIRATION);
    generalSwitch(ChartPlace);
}
*/

void showLiftedIndex()
{
    OM_HourlyForecastReturn forecast = generalWeatherGetData();
    if (forecast.fine == false)
    {
        return;
    }
#if DEBUG
    debugLog("Dumping lifted index");
    for (int i = 0; i < OM_WEATHER_MAX_HOURS; i++)
    {
        debugLog(String(i) + ": " + String(forecast.data.lifted_index[i]));
    }
#endif
    float li[OM_WEATHER_MAX_HOURS] = {0};
    for (u8_t i = 0; i < OM_WEATHER_MAX_HOURS; i++)
    {
        li[i] = float(forecast.data.lifted_index[i]);
    }
    showChart(li, OM_WEATHER_MAX_HOURS, WEATHER_CHART_LIFTED_INDEX);
    generalSwitch(ChartPlace);
}

void showConvectiveInhibition()
{
    OM_HourlyForecastReturn forecast = generalWeatherGetData();
    if (forecast.fine == false)
    {
        return;
    }
#if DEBUG
    debugLog("Dumping convective inhibition");
    for (int i = 0; i < OM_WEATHER_MAX_HOURS; i++)
    {
        debugLog(String(i) + ": " + String(forecast.data.convective_inhibition[i]));
    }
#endif
    float cin[OM_WEATHER_MAX_HOURS] = {0};
    for (u8_t i = 0; i < OM_WEATHER_MAX_HOURS; i++)
    {
        cin[i] = float(forecast.data.convective_inhibition[i]);
    }
    showChart(cin, OM_WEATHER_MAX_HOURS, WEATHER_CHART_CONVECTIVE_INHIBITION);
    generalSwitch(ChartPlace);
}

void showTerrestrialRadiation()
{
    OM_HourlyForecastReturn forecast = generalWeatherGetData();
    if (forecast.fine == false)
    {
        return;
    }
#if DEBUG
    debugLog("Dumping terrestrial radiation");
    for (int i = 0; i < OM_WEATHER_MAX_HOURS; i++)
    {
        debugLog(String(i) + ": " + String(forecast.data.terrestrial_radiation[i]));
    }
#endif
    float tr[OM_WEATHER_MAX_HOURS] = {0};
    for (u8_t i = 0; i < OM_WEATHER_MAX_HOURS; i++)
    {
        tr[i] = float(forecast.data.terrestrial_radiation[i]);
    }
    showChart(tr, OM_WEATHER_MAX_HOURS, WEATHER_CHART_TERRESTRIAL_RADIATION);
    generalSwitch(ChartPlace);
}

/* void showGlobalTiltedIrradiance()
{
    OM_HourlyForecastReturn forecast = generalWeatherGetData();
    if (forecast.fine == false)
    {
        return;
    }
#if DEBUG
    debugLog("Dumping global_tilted_irradiance");
    for (int i = 0; i < OM_WEATHER_MAX_HOURS; i++)
    {
        debugLog(String(i) + ": " + String(forecast.data.global_tilted_irradiance[i]));
    }
#endif
    float gti[OM_WEATHER_MAX_HOURS] = {0};
    for (u8_t i = 0; i < OM_WEATHER_MAX_HOURS; i++)
    {
        gti[i] = float(forecast.data.global_tilted_irradiance[i]);
    }
    showChart(gti, OM_WEATHER_MAX_HOURS, WEATHER_CHART_GLOBAL_TILTED_IRRADIANCE);
    generalSwitch(ChartPlace);
}
*/

/* void showDirectNormalIrradiance()
{
    OM_HourlyForecastReturn forecast = generalWeatherGetData();
    if (forecast.fine == false)
    {
        return;
    }
#if DEBUG
    debugLog("Dumping direct_normal_irradiance");
    for (int i = 0; i < OM_WEATHER_MAX_HOURS; i++)
    {
        debugLog(String(i) + ": " + String(forecast.data.direct_normal_irradiance[i]));
    }
#endif
    float dni[OM_WEATHER_MAX_HOURS] = {0};
    for (u8_t i = 0; i < OM_WEATHER_MAX_HOURS; i++)
    {
        dni[i] = float(forecast.data.direct_normal_irradiance[i]);
    }
    showChart(dni, OM_WEATHER_MAX_HOURS, WEATHER_CHART_DIRECT_NORMAL_IRRADIANCE);
    generalSwitch(ChartPlace);
}
*/

void showDiffuseRadiation()
{
    OM_HourlyForecastReturn forecast = generalWeatherGetData();
    if (forecast.fine == false)
    {
        return;
    }
#if DEBUG
    debugLog("Dumping diffuse_radiation");
    for (int i = 0; i < OM_WEATHER_MAX_HOURS; i++)
    {
        debugLog(String(i) + ": " + String(forecast.data.diffuse_radiation[i]));
    }
#endif
    float diff[OM_WEATHER_MAX_HOURS] = {0};
    for (u8_t i = 0; i < OM_WEATHER_MAX_HOURS; i++)
    {
        diff[i] = float(forecast.data.diffuse_radiation[i]);
    }
    showChart(diff, OM_WEATHER_MAX_HOURS, WEATHER_CHART_DIFFUSE_RADIATION);
    generalSwitch(ChartPlace);
}

void showDirectRadiation()
{
    OM_HourlyForecastReturn forecast = generalWeatherGetData();
    if (forecast.fine == false)
    {
        return;
    }
#if DEBUG
    debugLog("Dumping direct_radiation");
    for (int i = 0; i < OM_WEATHER_MAX_HOURS; i++)
    {
        debugLog(String(i) + ": " + String(forecast.data.direct_radiation[i]));
    }
#endif
    float direct[OM_WEATHER_MAX_HOURS] = {0};
    for (u8_t i = 0; i < OM_WEATHER_MAX_HOURS; i++)
    {
        direct[i] = float(forecast.data.direct_radiation[i]);
    }
    showChart(direct, OM_WEATHER_MAX_HOURS, WEATHER_CHART_DIRECT_RADIATION);
    generalSwitch(ChartPlace);
}

/* void showShortwaveRadiation()
{
    OM_HourlyForecastReturn forecast = generalWeatherGetData();
    if (forecast.fine == false)
    {
        return;
    }
#if DEBUG
    debugLog("Dumping shortwave_radiation");
    for (int i = 0; i < OM_WEATHER_MAX_HOURS; i++)
    {
        debugLog(String(i) + ": " + String(forecast.data.shortwave_radiation[i]));
    }
#endif
    float sw[OM_WEATHER_MAX_HOURS] = {0};
    for (u8_t i = 0; i < OM_WEATHER_MAX_HOURS; i++)
    {
        sw[i] = float(forecast.data.shortwave_radiation[i]);
    }
    showChart(sw, OM_WEATHER_MAX_HOURS, WEATHER_CHART_SHORTWAVE_RADIATION);
    generalSwitch(ChartPlace);
}
*/

/* void showTotalColumnIntegratedWaterVapour()
{
    OM_HourlyForecastReturn forecast = generalWeatherGetData();
    if (forecast.fine == false)
    {
        return;
    }
#if DEBUG
    debugLog("Dumping total_column_integrated_water_vapour");
    for (int i = 0; i < OM_WEATHER_MAX_HOURS; i++)
    {
        debugLog(String(i) + ": " + String(forecast.data.total_water_vapour[i]));
    }
#endif
    float twcv[OM_WEATHER_MAX_HOURS] = {0};
    for (u8_t i = 0; i < OM_WEATHER_MAX_HOURS; i++)
    {
        twcv[i] = float(forecast.data.total_water_vapour[i]);
    }
    showChart(twcv, OM_WEATHER_MAX_HOURS, WEATHER_CHART_TOTAL_WATER_VAPOUR);
    generalSwitch(ChartPlace);
} */

#endif
