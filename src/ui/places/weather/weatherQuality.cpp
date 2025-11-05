#include "weatherQuality.h"
#include "hardware/rtc/rtc.h"
#include "other/debug/debug.h"
#include "ui/functionsUi.h"

#if WEATHER_INFO

#if WEATHER_AIR_ENABLED
OM_OneHourAirQuality airQualityGetDataHourly(uint8_t hourOffset)
{
    String unixTimeAirQuality = String(simplifyUnix(getUnixTime(timeRTCLocal)));
    debugLog("Getting air quality for unix: " + unixTimeAirQuality);
    bufSize airData = fsGetBlob(unixTimeAirQuality, String(AIR_QUALITY_HOURLY_DIR) + "/");
    debugLog("Air quality size is: " + String(airData.size) + " While is should be: " + String(sizeof(OM_AirQualityForecast)));
    OM_OneHourAirQuality airForecast = {0};

    if (airData.size != sizeof(OM_AirQualityForecast))
    {
        debugLog("Air quality data is bad.");
        free(airData.buf);
        airForecast.fine = false;
        return airForecast;
    }
    OM_AirQualityForecast *airDataWork = (OM_AirQualityForecast *)airData.buf;
    int64_t unixNow = getUnixTime(timeRTCUTC0) + (60 * 60) * hourOffset; // Using utc0 as the stored data from open meteo uses utc0 unix timestamps for time
    int64_t smallestDiffUnix = 0;
    uint8_t smallestDiffIndex = 0;
    for (int i = 0; i < OM_AIR_QUALITY_MAX_HOURS; i++)
    {
        if (llabs(unixNow - (int64_t)airDataWork->hourly_time[i]) < llabs(unixNow - smallestDiffUnix))
        {
            smallestDiffUnix = airDataWork->hourly_time[i];
            smallestDiffIndex = i;
        }
        debugLog("Size of OM_AirQualityForecast: " + String(sizeof(OM_AirQualityForecast)));
        //   debugLog("Current time: " + String(unixNow) + " choosed time: " + String(airDataWork->hourly_time[i]) + " diff: " + String(llabs(unixNow - airDataWork->hourly_time[i])));
    }
    debugLog("FINAL time: " + String(unixNow) + " choosed time: " + String(airDataWork->hourly_time[smallestDiffIndex]) + " diff: " + String(llabs(unixNow - airDataWork->hourly_time[smallestDiffIndex])) + " so it's index is: " + String(smallestDiffIndex));

    airForecast.european_aqi = airDataWork->european_aqi[smallestDiffIndex];
    airForecast.us_aqi = airDataWork->us_aqi[smallestDiffIndex];
    airForecast.european_aqi_pm2_5 = airDataWork->european_aqi_pm2_5[smallestDiffIndex];
    airForecast.european_aqi_pm10 = airDataWork->european_aqi_pm10[smallestDiffIndex];
    airForecast.european_aqi_nitrogen_dioxide = airDataWork->european_aqi_nitrogen_dioxide[smallestDiffIndex];
    airForecast.european_aqi_ozone = airDataWork->european_aqi_ozone[smallestDiffIndex];
    airForecast.european_aqi_sulphur_dioxide = airDataWork->european_aqi_sulphur_dioxide[smallestDiffIndex];

    airForecast.pm2_5 = airDataWork->pm2_5[smallestDiffIndex];
    airForecast.pm10 = airDataWork->pm10[smallestDiffIndex];
    airForecast.carbon_monoxide = airDataWork->carbon_monoxide[smallestDiffIndex];
    airForecast.carbon_dioxide = airDataWork->carbon_dioxide[smallestDiffIndex];
    airForecast.nitrogen_dioxide = airDataWork->nitrogen_dioxide[smallestDiffIndex];
    airForecast.sulphur_dioxide = airDataWork->sulphur_dioxide[smallestDiffIndex];
    airForecast.ozone = airDataWork->ozone[smallestDiffIndex];
    airForecast.aerosol_optical_depth = airDataWork->aerosol_optical_depth[smallestDiffIndex];
    airForecast.dust = airDataWork->dust[smallestDiffIndex];
    airForecast.methane = airDataWork->methane[smallestDiffIndex];
    airForecast.formaldehyde = airDataWork->formaldehyde[smallestDiffIndex];
    airForecast.glyoxal = airDataWork->glyoxal[smallestDiffIndex];
    //    airForecast.sea_salt_aerosol = airDataWork->sea_salt_aerosol[smallestDiffIndex];
    airForecast.nitrogen_monoxide = airDataWork->nitrogen_monoxide[smallestDiffIndex];
    airForecast.peroxyacyl_nitrates = airDataWork->peroxyacyl_nitrates[smallestDiffIndex];

    free(airData.buf);
    airForecast.fine = true;
    return airForecast;
}
#else
OM_OneHourAirQuality airQualityGetDataHourly(uint8_t hourOffset)
{
    OM_OneHourAirQuality airForecast = {0};
    airForecast.fine = false;
    return airForecast;
}
#endif

#if WEATHER_AIR_ENABLED
char airQualityDayChoosed[12];

void initAirQualityDateMenu()
{
    int days = 0;
    // Count air quality data files
    File root = LittleFS.open(AIR_QUALITY_HOURLY_DIR);
    if (!root)
    {
        debugLog("Failed to open air quality directory");
        overwriteSwitch(textDialog);
        showTextDialog((AIR_QUALITY_NOT_AVAILABLE), true);
        return;
    }

    if (!root.isDirectory())
    {
        debugLog("Not a directory: " + String(AIR_QUALITY_HOURLY_DIR));
        root.close();
        overwriteSwitch(textDialog);
        showTextDialog((AIR_QUALITY_NOT_AVAILABLE), true);
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
        showTextDialog((AIR_QUALITY_NOT_AVAILABLE), true);
        return;
    }

    // Get and sort dates
    long daysUnixList[days] = {0};
    root = LittleFS.open(AIR_QUALITY_HOURLY_DIR);
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

    // Find the current day
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

    // Reorder the list with current day first
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
        buttons[i] = {unixToDate(daysUnixList[i]), &emptyImgPack, switchAirQualityConditionMenu};
    }

    initMenu(buttons, days, (AIR_QUALITY_SELECT_DATE), 1);
}

void initAirQualityConditionMenu()
{
    debugLog("initAirQualityConditionMenu called");
    // Check if the last menu item name is a date
    int dayIndex = lastMenuSelected.indexOf(".");
    // if (lastMenuSelected.length() >= 5 && lastMenuSelected[2] == '.' && lastMenuSelected[5] == '.')
    if (dayIndex != -1)
    {
        lastMenuSelected.toCharArray(airQualityDayChoosed, 12);
        airQualityDayChoosed[11] = '\0';
    }
    else if (String(airQualityDayChoosed).indexOf(".") == -1)
    {
        debugLog("Error finding date for air quality condition");
        overwriteSwitch(textDialog);
        showTextDialog(AIR_QUALITY_DATE_WRONG, true);
        return;
    }

    // EU AQI
    // US AQI
    // Particulate Matter smaller than 2.5 micrometres
    // Particulate Matter smaller than 10 micrometres
    // Nitrogen Dioxide
    // Ozone
    // Sulphur Dioxide

    entryMenu buttons[21] = {
        {AIR_MENU_EUAQI, &emptyImgPack, showEuropeanAQI},
        {AIR_MENU_USAQI, &emptyImgPack, showUS_AQI},
        {AIR_MENU_EU_AQI_PM2_5, &emptyImgPack, showEUAQI_PM2_5},
        {AIR_MENU_EU_AQI_PM10, &emptyImgPack, showEUAQI_PM10},
        {AIR_MENU_EU_AQI_NO2, &emptyImgPack, showEUAQI_NO2},
        {AIR_MENU_EU_AQI_O3, &emptyImgPack, showEUAQI_O3},
        {AIR_MENU_EU_AQI_SO2, &emptyImgPack, showEUAQI_SO2},
        {AIR_MENU_PM2_5, &emptyImgPack, showPM2_5},
        {AIR_MENU_PM10, &emptyImgPack, showPM10},
        {AIR_MENU_CARBON_MONOXIDE, &emptyImgPack, showCarbonMonoxide},
        {AIR_MENU_CARBON_DIOXIDE, &emptyImgPack, showCarbonDioxide},
        {AIR_MENU_NITROGEN_DIOXIDE, &emptyImgPack, showNitrogenDioxide},
        {AIR_MENU_SULPHUR_DIOXIDE, &emptyImgPack, showSulphurDioxide},
        {AIR_MENU_OZONE, &emptyImgPack, showOzone},
        {AIR_MENU_AEROSOL_OPTICAL_DEPTH, &emptyImgPack, showAerosolOpticalDepth},
        {AIR_MENU_DUST, &emptyImgPack, showDust},
        {AIR_MENU_METHANE, &emptyImgPack, showMethane},
        {AIR_MENU_FORMALDEHYDE, &emptyImgPack, showFormaldehyde},
        {AIR_MENU_GLYOXAL, &emptyImgPack, showGlyoxal},
        //   {AIR_MENU_SEA_SALT_AEROSOL, &emptyImgPack, showSeaSaltAerosol},
        {AIR_MENU_NITROGEN_MONOXIDE, &emptyImgPack, showNitrogenMonoxide},
        {AIR_MENU_PEROXYACYL_NITRATES, &emptyImgPack, showPeroxyacylNitrates}};
    initMenu(buttons, 21, AIR_QUALITY_STAT_TITLE, 1);
}

OM_AirQualityForecastReturn generalAirQualityGetData()
{
    String airQualityDay = String(airQualityDayChoosed);
    debugLog("Showing air quality for day: " + airQualityDay);

    bufSize airData = fsGetBlob(String(dateToUnix(airQualityDay)), String(AIR_QUALITY_HOURLY_DIR) + "/");
    debugLog("Air quality size is: " + String(airData.size) + " While is should be: " + String(sizeof(OM_AirQualityForecast)));
    OM_AirQualityForecastReturn airForecast = {};
    if (airData.size != sizeof(OM_AirQualityForecast))
    {
        debugLog("Air quality data is bad.");
        free(airData.buf);
        overwriteSwitch(textDialog);
        showTextDialog(AIR_QUALITY_CORRUPTED, true);
        airForecast.fine = false;
        return airForecast;
    }
    memcpy(&airForecast.data, airData.buf, airData.size);
    free(airData.buf);

    airForecast.fine = true;
    return airForecast;
}

void showEuropeanAQI()
{
    OM_AirQualityForecastReturn airForecast = generalAirQualityGetData();
    if (airForecast.fine == false)
    {
        return;
    }
#if DEBUG
    debugLog("Dumping EU AQI");
    for (int i = 0; i < OM_AIR_QUALITY_MAX_HOURS; i++)
    {
        debugLog(String(i) + ": " + String(airForecast.data.european_aqi[i]));
    }
#endif
    float european_aqi[OM_AIR_QUALITY_MAX_HOURS] = {0};
    for (u8_t i = 0; i < OM_AIR_QUALITY_MAX_HOURS; i++)
    {
        european_aqi[i] = float(airForecast.data.european_aqi[i]);
    }
    showChart(european_aqi, OM_AIR_QUALITY_MAX_HOURS, AIR_CHART_EUAQI);
    generalSwitch(ChartPlace);
}

void showUS_AQI()
{
    OM_AirQualityForecastReturn airForecast = generalAirQualityGetData();
    if (airForecast.fine == false)
    {
        return;
    }
#if DEBUG
    debugLog("Dumping US AQI");
    for (int i = 0; i < OM_AIR_QUALITY_MAX_HOURS; i++)
    {
        debugLog(String(i) + ": " + String(airForecast.data.us_aqi[i]));
    }
#endif
    float us_aqi[OM_AIR_QUALITY_MAX_HOURS] = {0};
    for (u8_t i = 0; i < OM_AIR_QUALITY_MAX_HOURS; i++)
    {
        us_aqi[i] = float(airForecast.data.us_aqi[i]);
    }
    showChart(us_aqi, OM_AIR_QUALITY_MAX_HOURS, AIR_CHART_USAQI);
    generalSwitch(ChartPlace);
}

void showEUAQI_PM2_5()
{
    OM_AirQualityForecastReturn airForecast = generalAirQualityGetData();
    if (airForecast.fine == false)
    {
        return;
    }
#if DEBUG
    debugLog("Dumping EU AQI PM 2.5");
    for (int i = 0; i < OM_AIR_QUALITY_MAX_HOURS; i++)
    {
        debugLog(String(i) + ": " + String(airForecast.data.european_aqi_pm2_5[i]));
    }
#endif
    float european_aqi_pm2_5[OM_AIR_QUALITY_MAX_HOURS] = {0};
    for (u8_t i = 0; i < OM_AIR_QUALITY_MAX_HOURS; i++)
    {
        european_aqi_pm2_5[i] = float(airForecast.data.european_aqi_pm2_5[i]);
    }
    showChart(european_aqi_pm2_5, OM_AIR_QUALITY_MAX_HOURS, AIR_CHART_EU_AQI_PM2_5);
    generalSwitch(ChartPlace);
}

void showEUAQI_PM10()
{
    OM_AirQualityForecastReturn airForecast = generalAirQualityGetData();
    if (airForecast.fine == false)
    {
        return;
    }
#if DEBUG
    debugLog("Dumping EU AQI PM 10");
    for (int i = 0; i < OM_AIR_QUALITY_MAX_HOURS; i++)
    {
        debugLog(String(i) + ": " + String(airForecast.data.european_aqi_pm10[i]));
    }
#endif
    float european_aqi_pm10[OM_AIR_QUALITY_MAX_HOURS] = {0};
    for (u8_t i = 0; i < OM_AIR_QUALITY_MAX_HOURS; i++)
    {
        european_aqi_pm10[i] = float(airForecast.data.european_aqi_pm10[i]);
    }
    showChart(european_aqi_pm10, OM_AIR_QUALITY_MAX_HOURS, AIR_CHART_EU_AQI_PM10);
    generalSwitch(ChartPlace);
}

void showEUAQI_NO2()
{
    OM_AirQualityForecastReturn airForecast = generalAirQualityGetData();
    if (airForecast.fine == false)
    {
        return;
    }
#if DEBUG
    debugLog("Dumping EU AQI NO2");
    for (int i = 0; i < OM_AIR_QUALITY_MAX_HOURS; i++)
    {
        debugLog(String(i) + ": " + String(airForecast.data.european_aqi_nitrogen_dioxide[i]));
    }
#endif
    float european_aqi_nitrogen_dioxide[OM_AIR_QUALITY_MAX_HOURS] = {0};
    for (u8_t i = 0; i < OM_AIR_QUALITY_MAX_HOURS; i++)
    {
        european_aqi_nitrogen_dioxide[i] = float(airForecast.data.european_aqi_nitrogen_dioxide[i]);
    }
    showChart(european_aqi_nitrogen_dioxide, OM_AIR_QUALITY_MAX_HOURS, AIR_CHART_EU_AQI_NO2);
    generalSwitch(ChartPlace);
}

void showEUAQI_O3()
{
    OM_AirQualityForecastReturn airForecast = generalAirQualityGetData();
    if (airForecast.fine == false)
    {
        return;
    }
#if DEBUG
    debugLog("Dumping EU AQI Ozone");
    for (int i = 0; i < OM_AIR_QUALITY_MAX_HOURS; i++)
    {
        debugLog(String(i) + ": " + String(airForecast.data.european_aqi_ozone[i]));
    }
#endif
    float european_aqi_ozone[OM_AIR_QUALITY_MAX_HOURS] = {0};
    for (u8_t i = 0; i < OM_AIR_QUALITY_MAX_HOURS; i++)
    {
        european_aqi_ozone[i] = float(airForecast.data.european_aqi_ozone[i]);
    }
    showChart(european_aqi_ozone, OM_AIR_QUALITY_MAX_HOURS, AIR_CHART_EU_AQI_O3);
    generalSwitch(ChartPlace);
}

void showEUAQI_SO2()
{
    OM_AirQualityForecastReturn airForecast = generalAirQualityGetData();
    if (airForecast.fine == false)
    {
        return;
    }
#if DEBUG
    debugLog("Dumping EU AQI SO2");
    for (int i = 0; i < OM_AIR_QUALITY_MAX_HOURS; i++)
    {
        debugLog(String(i) + ": " + String(airForecast.data.european_aqi_sulphur_dioxide[i]));
    }
#endif
    float european_aqi_sulphur_dioxide[OM_AIR_QUALITY_MAX_HOURS] = {0};
    for (u8_t i = 0; i < OM_AIR_QUALITY_MAX_HOURS; i++)
    {
        european_aqi_sulphur_dioxide[i] = float(airForecast.data.european_aqi_sulphur_dioxide[i]);
    }
    showChart(european_aqi_sulphur_dioxide, OM_AIR_QUALITY_MAX_HOURS, AIR_CHART_EU_AQI_SO2);
    generalSwitch(ChartPlace);
}

void showPM2_5()
{
    OM_AirQualityForecastReturn airForecast = generalAirQualityGetData();
    if (airForecast.fine == false)
    {
        return;
    }
#if DEBUG
    debugLog("Dumping PM2.5");
    for (int i = 0; i < OM_AIR_QUALITY_MAX_HOURS; i++)
    {
        debugLog(String(i) + ": " + String(airForecast.data.pm2_5[i]));
    }
#endif
    float pm2_5[OM_AIR_QUALITY_MAX_HOURS] = {0};
    for (u8_t i = 0; i < OM_AIR_QUALITY_MAX_HOURS; i++)
    {
        pm2_5[i] = float(airForecast.data.pm2_5[i]);
    }
    showChart(pm2_5, OM_AIR_QUALITY_MAX_HOURS, AIR_CHART_PM2_5);
    generalSwitch(ChartPlace);
}

void showPM10()
{
    OM_AirQualityForecastReturn airForecast = generalAirQualityGetData();
    if (airForecast.fine == false)
    {
        return;
    }
#if DEBUG
    debugLog("Dumping PM10");
    for (int i = 0; i < OM_AIR_QUALITY_MAX_HOURS; i++)
    {
        debugLog(String(i) + ": " + String(airForecast.data.pm10[i]));
    }
#endif
    float pm10[OM_AIR_QUALITY_MAX_HOURS] = {0};
    for (u8_t i = 0; i < OM_AIR_QUALITY_MAX_HOURS; i++)
    {
        pm10[i] = float(airForecast.data.pm10[i]);
    }
    showChart(pm10, OM_AIR_QUALITY_MAX_HOURS, AIR_CHART_PM10);
    generalSwitch(ChartPlace);
}

void showCarbonMonoxide()
{
    OM_AirQualityForecastReturn airForecast = generalAirQualityGetData();
    if (airForecast.fine == false)
    {
        return;
    }
#if DEBUG
    debugLog("Dumping Carbon Monoxide");
    for (int i = 0; i < OM_AIR_QUALITY_MAX_HOURS; i++)
    {
        debugLog(String(i) + ": " + String(airForecast.data.carbon_monoxide[i]));
    }
#endif
    float carbon_monoxide[OM_AIR_QUALITY_MAX_HOURS] = {0};
    for (u8_t i = 0; i < OM_AIR_QUALITY_MAX_HOURS; i++)
    {
        carbon_monoxide[i] = float(airForecast.data.carbon_monoxide[i]);
    }
    showChart(carbon_monoxide, OM_AIR_QUALITY_MAX_HOURS, AIR_CHART_CARBON_MONOXIDE);
    generalSwitch(ChartPlace);
}

void showCarbonDioxide()
{
    OM_AirQualityForecastReturn airForecast = generalAirQualityGetData();
    if (airForecast.fine == false)
    {
        return;
    }
#if DEBUG
    debugLog("Dumping Carbon Dioxide");
    for (int i = 0; i < OM_AIR_QUALITY_MAX_HOURS; i++)
    {
        debugLog(String(i) + ": " + String(airForecast.data.carbon_dioxide[i]));
    }
#endif
    float carbon_dioxide[OM_AIR_QUALITY_MAX_HOURS] = {0};
    for (u8_t i = 0; i < OM_AIR_QUALITY_MAX_HOURS; i++)
    {
        carbon_dioxide[i] = float(airForecast.data.carbon_dioxide[i]);
    }
    showChart(carbon_dioxide, OM_AIR_QUALITY_MAX_HOURS, AIR_CHART_CARBON_DIOXIDE);
    generalSwitch(ChartPlace);
}

void showNitrogenDioxide()
{
    OM_AirQualityForecastReturn airForecast = generalAirQualityGetData();
    if (airForecast.fine == false)
    {
        return;
    }
#if DEBUG
    debugLog("Dumping Nitrogen Dioxide");
    for (int i = 0; i < OM_AIR_QUALITY_MAX_HOURS; i++)
    {
        debugLog(String(i) + ": " + String(airForecast.data.nitrogen_dioxide[i]));
    }
#endif
    float nitrogen_dioxide[OM_AIR_QUALITY_MAX_HOURS] = {0};
    for (u8_t i = 0; i < OM_AIR_QUALITY_MAX_HOURS; i++)
    {
        nitrogen_dioxide[i] = float(airForecast.data.nitrogen_dioxide[i]);
    }
    showChart(nitrogen_dioxide, OM_AIR_QUALITY_MAX_HOURS, AIR_CHART_NITROGEN_DIOXIDE);
    generalSwitch(ChartPlace);
}

void showSulphurDioxide()
{
    OM_AirQualityForecastReturn airForecast = generalAirQualityGetData();
    if (airForecast.fine == false)
    {
        return;
    }
#if DEBUG
    debugLog("Dumping Sulphur Dioxide");
    for (int i = 0; i < OM_AIR_QUALITY_MAX_HOURS; i++)
    {
        debugLog(String(i) + ": " + String(airForecast.data.sulphur_dioxide[i]));
    }
#endif
    float sulphur_dioxide[OM_AIR_QUALITY_MAX_HOURS] = {0};
    for (u8_t i = 0; i < OM_AIR_QUALITY_MAX_HOURS; i++)
    {
        sulphur_dioxide[i] = float(airForecast.data.sulphur_dioxide[i]);
    }
    showChart(sulphur_dioxide, OM_AIR_QUALITY_MAX_HOURS, AIR_CHART_SULPHUR_DIOXIDE);
    generalSwitch(ChartPlace);
}

void showOzone()
{
    OM_AirQualityForecastReturn airForecast = generalAirQualityGetData();
    if (airForecast.fine == false)
    {
        return;
    }
#if DEBUG
    debugLog("Dumping Ozone");
    for (int i = 0; i < OM_AIR_QUALITY_MAX_HOURS; i++)
    {
        debugLog(String(i) + ": " + String(airForecast.data.ozone[i]));
    }
#endif
    float ozone[OM_AIR_QUALITY_MAX_HOURS] = {0};
    for (u8_t i = 0; i < OM_AIR_QUALITY_MAX_HOURS; i++)
    {
        ozone[i] = float(airForecast.data.ozone[i]);
    }
    showChart(ozone, OM_AIR_QUALITY_MAX_HOURS, AIR_CHART_OZONE);
    generalSwitch(ChartPlace);
}

void showAerosolOpticalDepth()
{
    OM_AirQualityForecastReturn airForecast = generalAirQualityGetData();
    if (airForecast.fine == false)
    {
        return;
    }
#if DEBUG
    debugLog("Dumping Aerosol Optical Depth");
    for (int i = 0; i < OM_AIR_QUALITY_MAX_HOURS; i++)
    {
        debugLog(String(i) + ": " + String(airForecast.data.aerosol_optical_depth[i]));
    }
#endif
    float aerosol_optical_depth[OM_AIR_QUALITY_MAX_HOURS] = {0};
    for (u8_t i = 0; i < OM_AIR_QUALITY_MAX_HOURS; i++)
    {
        aerosol_optical_depth[i] = float(airForecast.data.aerosol_optical_depth[i]);
    }
    showChart(aerosol_optical_depth, OM_AIR_QUALITY_MAX_HOURS, AIR_CHART_AEROSOL_OPTICAL_DEPTH);
    generalSwitch(ChartPlace);
}

void showDust()
{
    OM_AirQualityForecastReturn airForecast = generalAirQualityGetData();
    if (airForecast.fine == false)
    {
        return;
    }
#if DEBUG
    debugLog("Dumping Dust");
    for (int i = 0; i < OM_AIR_QUALITY_MAX_HOURS; i++)
    {
        debugLog(String(i) + ": " + String(airForecast.data.dust[i]));
    }
#endif
    float dust[OM_AIR_QUALITY_MAX_HOURS] = {0};
    for (u8_t i = 0; i < OM_AIR_QUALITY_MAX_HOURS; i++)
    {
        dust[i] = float(airForecast.data.dust[i]);
    }
    showChart(dust, OM_AIR_QUALITY_MAX_HOURS, AIR_CHART_DUST);
    generalSwitch(ChartPlace);
}

void showMethane()
{
    OM_AirQualityForecastReturn airForecast = generalAirQualityGetData();
    if (airForecast.fine == false)
    {
        return;
    }
#if DEBUG
    debugLog("Dumping Methane");
    for (int i = 0; i < OM_AIR_QUALITY_MAX_HOURS; i++)
    {
        debugLog(String(i) + ": " + String(airForecast.data.methane[i]));
    }
#endif
    float methane[OM_AIR_QUALITY_MAX_HOURS] = {0};
    for (u8_t i = 0; i < OM_AIR_QUALITY_MAX_HOURS; i++)
    {
        methane[i] = float(airForecast.data.methane[i]);
    }
    showChart(methane, OM_AIR_QUALITY_MAX_HOURS, AIR_CHART_METHANE);
    generalSwitch(ChartPlace);
}

void showFormaldehyde()
{
    OM_AirQualityForecastReturn airForecast = generalAirQualityGetData();
    if (airForecast.fine == false)
    {
        return;
    }
#if DEBUG
    debugLog("Dumping Formaldehyde");
    for (int i = 0; i < OM_AIR_QUALITY_MAX_HOURS; i++)
    {
        debugLog(String(i) + ": " + String(airForecast.data.formaldehyde[i]));
    }
#endif
    float formaldehyde[OM_AIR_QUALITY_MAX_HOURS] = {0};
    for (u8_t i = 0; i < OM_AIR_QUALITY_MAX_HOURS; i++)
    {
        formaldehyde[i] = float(airForecast.data.formaldehyde[i]);
    }
    showChart(formaldehyde, OM_AIR_QUALITY_MAX_HOURS, AIR_CHART_FORMALDEHYDE);
    generalSwitch(ChartPlace);
}

void showGlyoxal()
{
    OM_AirQualityForecastReturn airForecast = generalAirQualityGetData();
    if (airForecast.fine == false)
    {
        return;
    }
#if DEBUG
    debugLog("Dumping Glyoxal");
    for (int i = 0; i < OM_AIR_QUALITY_MAX_HOURS; i++)
    {
        debugLog(String(i) + ": " + String(airForecast.data.glyoxal[i]));
    }
#endif
    float glyoxal[OM_AIR_QUALITY_MAX_HOURS] = {0};
    for (u8_t i = 0; i < OM_AIR_QUALITY_MAX_HOURS; i++)
    {
        glyoxal[i] = float(airForecast.data.glyoxal[i]);
    }
    showChart(glyoxal, OM_AIR_QUALITY_MAX_HOURS, AIR_CHART_GLYOXAL);
    generalSwitch(ChartPlace);
}

/* void showSeaSaltAerosol()
{
    OM_AirQualityForecastReturn airForecast = generalAirQualityGetData();
    if (airForecast.fine == false)
    {
        return;
    }
#if DEBUG
    debugLog("Dumping Sea Salt Aerosol");
    for (int i = 0; i < OM_AIR_QUALITY_MAX_HOURS; i++)
    {
        debugLog(String(i) + ": " + String(airForecast.data.sea_salt_aerosol[i]));
    }
#endif
    float sea_salt_aerosol[OM_AIR_QUALITY_MAX_HOURS] = {0};
    for (u8_t i = 0; i < OM_AIR_QUALITY_MAX_HOURS; i++)
    {
        sea_salt_aerosol[i] = float(airForecast.data.sea_salt_aerosol[i]);
    }
    showChart(sea_salt_aerosol, OM_AIR_QUALITY_MAX_HOURS, AIR_CHART_SEA_SALT_AEROSOL);
    generalSwitch(ChartPlace);
} */

void showNitrogenMonoxide()
{
    OM_AirQualityForecastReturn airForecast = generalAirQualityGetData();
    if (airForecast.fine == false)
    {
        return;
    }
#if DEBUG
    debugLog("Dumping Nitrogen Monoxide");
    for (int i = 0; i < OM_AIR_QUALITY_MAX_HOURS; i++)
    {
        debugLog(String(i) + ": " + String(airForecast.data.nitrogen_monoxide[i]));
    }
#endif
    float nitrogen_monoxide[OM_AIR_QUALITY_MAX_HOURS] = {0};
    for (u8_t i = 0; i < OM_AIR_QUALITY_MAX_HOURS; i++)
    {
        nitrogen_monoxide[i] = float(airForecast.data.nitrogen_monoxide[i]);
    }
    showChart(nitrogen_monoxide, OM_AIR_QUALITY_MAX_HOURS, AIR_CHART_NITROGEN_MONOXIDE);
    generalSwitch(ChartPlace);
}

void showPeroxyacylNitrates()
{
    OM_AirQualityForecastReturn airForecast = generalAirQualityGetData();
    if (airForecast.fine == false)
    {
        return;
    }
#if DEBUG
    debugLog("Dumping Peroxyacyl Nitrates");
    for (int i = 0; i < OM_AIR_QUALITY_MAX_HOURS; i++)
    {
        debugLog(String(i) + ": " + String(airForecast.data.peroxyacyl_nitrates[i]));
    }
#endif
    float peroxyacyl_nitrates[OM_AIR_QUALITY_MAX_HOURS] = {0};
    for (u8_t i = 0; i < OM_AIR_QUALITY_MAX_HOURS; i++)
    {
        peroxyacyl_nitrates[i] = float(airForecast.data.peroxyacyl_nitrates[i]);
    }
    showChart(peroxyacyl_nitrates, OM_AIR_QUALITY_MAX_HOURS, AIR_CHART_PEROXYACYL_NITRATES);
    generalSwitch(ChartPlace);
}

#endif
#endif
