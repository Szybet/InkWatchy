#include "inkField.h"
#include "inkput.h"

#if WATCHFACE_INKFIELD_SZYBET

#define TIME_FONT getFont("inkfield/JackInput40")
#define DATE_FONT getFont("JackInput17")
#define DAY_NAME_FONT getFont("inkfield/Speculum13")
#define MONTH_NAME_FONT getFont("inkfield/Speculum9")

#define DAY_NAME_CORD 13, 87
#define DATE_CORD 8, 113
#define MONTH_NAME_CORD 46, 109
#define MONTH_NUMBER_1_CORD 89, 93  // Not used?
#define MONTH_NUMBER_2_CORD 89, 102 // Not used?
#define TO_DAY_BAR_CORD 136, 68
#define TO_DAY_BAR_SIZE 54, 7
#define BATT_BAR_CORD 136, 83
#define STEPS_BAR_CORD 136, 98

/*
// Even with monospaced font, it differs a bit...
{
  String who = "1234567890:";
  uint16_t ww;
  for (int i = 0; i < who.length(); i++)
  {
    String hh = String(who[i]);
    getTextBounds(hh, NULL, NULL, &ww, NULL);
    debugLog(hh + " " + String(ww));
  }
}

: 1 29
: 2 32
: 3 32
: 4 32
: 5 32
: 6 31
: 7 30
: 8 31
: 9 31
: 0 31
: : 12
*/

#define TIME_LETTERS_SPACING 32 // It differs, see below - so not it's static
#define TIME_CORD_X 11
#define TIME_CORD_Y 53
#define TIME_CORD TIME_CORD_X, TIME_CORD_Y

static void drawTimeBeforeApply()
{
    debugLog("Called");
    setTextSize(1);
    setFont(TIME_FONT);
    debugLog("Getting hour minute for wFTime");
    String oldTime = getHourMinute(wFTime);
    debugLog("Getting hour minute for timeRTCLocal");
    String newTime = getHourMinute(timeRTCLocal);

    for (int i = 0; i < 5; i++)
    {
        if (oldTime[i] != newTime[i])
        {
            String toWrite = String(newTime[i]);
            String oldWrite = String(oldTime[i]);
            String beforeString = oldTime.substring(0, i);
            String afterString = oldTime.substring(0, i + 1);
            // debugLog("beforeString: " + beforeString);
            // debugLog("afterString: " + afterString);

            // uint16_t wBefore;
            // getTextBounds(beforeString, NULL, NULL, &wBefore, NULL);
            // debugLog("wBefore: " + String(wBefore));

            uint16_t wAfter;
            getTextBounds(afterString, NULL, NULL, &wAfter, NULL);
            // debugLog("wAfter: " + String(wAfter));

            uint16_t wToWrite;
            uint16_t hToWrite;
            getTextBounds(oldWrite, NULL, NULL, &wToWrite, &hToWrite);
            // debugLog("wToWrite: " + String(wToWrite));
            // debugLog("hToWrite: " + String(hToWrite));

            uint16_t finalWidthStart = wAfter - wToWrite;
            // debugLog("finalWidthStart: " + String(finalWidthStart));

            // debugLog("Writing to screen: " + toWrite);
            display.fillRect(TIME_CORD_X + finalWidthStart, TIME_CORD_Y - hToWrite, TIME_LETTERS_SPACING, hToWrite, GxEPD_WHITE); // Clear things up
            writeTextReplaceBack(toWrite, TIME_CORD_X + finalWidthStart, TIME_CORD_Y);
        }
    }
}

RTC_DATA_ATTR uint8_t percentOfDay = 0;
RTC_DATA_ATTR uint8_t percentSteps = 0;

RTC_DATA_ATTR uint8_t weatherMinutes = 0;
#define TEMP_CORD 48, 120
#define HUMIDITY_CORD 25, 124
#define PRESSURE_CORD 25, 132
#define PREPICITATION_CORD 25, 140
#define VISIBILITY_CORD 25, 148
#define WIND_SPEED_CORD 25, 156

#define WIND_GUTS_CORD 46, 132
#define CLOUD_COVER_CORD 46, 140
#define SUNRISE_CORD 46, 148
#define SUNSET_CORD 46, 156

#define WEATHER_ICON_CORD 63, 124

#define TEMP_RECT_CORD 49, 114
#define TEMP_RECT_SIZE 19, 7

void weatherDrawHelper(uint16_t value, int max, int min, uint16_t x, uint16_t y)
{
    int percent = (value - min) * 100 / (max - min);

    if (percent < 0)
    {
        percent = 0;
    }
    else if (percent > 99)
    {
        percent = 99;
    }

    writeTextReplaceBack(addZero(String(percent), 2), x, y, GxEPD_BLACK, GxEPD_WHITE, true, 1);
}

String getWeatherIcon(uint8_t weatherCode)
{
    switch (weatherCode)
    {
    case 0:
        return "clear_sky";
        // return "Clear sky";
    case 1:
        return "mainly_clear";
        // return "Mainly clear";
    case 2:
        return "partly_cloud";
        // return "Partly cloudy";
    case 3:
        return "overcast";
        // return "Overcast";
    case 45:
        return "fog";
        // return "Fog";
    case 48:
        return "freezing";
        // return "Depositing rime fog";
    case 51:
        return "drizzle_all";
        // return "Drizzle: Light intensity";
    case 53:
        return "drizzle_all";
        // return "Drizzle: Moderate intensity";
    case 55:
        return "drizzle_all";
        // return "Drizzle: Dense intensity";
    case 56:
        return "freezing";
        // return "Freezing Drizzle: Light intensity";
    case 57:
        return "freezing";
        // return "Freezing Drizzle: Dense intensity";
    case 61:
        return "rain";
        // return "Rain: Slight intensity";
    case 63:
        return "rain";
        // return "Rain: Moderate intensity";
    case 65:
        return "rain";
        // return "Rain: Heavy intensity";
    case 66:
        return "freezing";
        // return "Freezing Rain: Light intensity";
    case 67:
        return "freezing";
        // return "Freezing Rain: Heavy intensity";
    case 71:
        return "snow";
        // return "Snow fall: Slight intensity";
    case 73:
        return "snow";
        // return "Snow fall: Moderate intensity";
    case 75:
        return "snow";
        // return "Snow fall: Heavy intensity";
    case 77:
        return "snow";
        // return "Snow grains";
    case 80:
        return "rain";
        // return "Rain showers: Slight intensity";
    case 81:
        return "rain";
        // return "Rain showers: Moderate intensity";
    case 82:
        return "rain";
        // return "Rain showers: Violent intensity";
    case 85:
        return "snow";
        // return "Snow showers: Slight intensity";
    case 86:
        return "snow";
        // return "Snow showers: Heavy intensity";
    case 95:
        return "thunderstorm";
        // return "Thunderstorm: Slight or moderate";
    case 96:
        return "thunderstorm";
        // return "Thunderstorm with slight hail";
    case 99:
        return "thunderstorm";
        // return "Thunderstorm with heavy hail";
    default:
        return "no_weather_data";
        // return "Unknown weather condition";
    }
}

static void drawTimeAfterApply(bool forceDraw)
{
    // Draw the percentage on the right
    int percentOfDayTmp = calculatePercentageOfDay(wFTime.Hour, wFTime.Minute);
    if (percentOfDay != percentOfDayTmp || forceDraw == true)
    {
        percentOfDay = percentOfDayTmp;
        drawProgressBar(TO_DAY_BAR_CORD, TO_DAY_BAR_SIZE, percentOfDay);
    }

    uint16_t percentStepsTmp = uint16_t(((float)getSteps() / (float)STEPS_GOAL) * 100.0);
    debugLog("percentStepsTmp: " + String(percentStepsTmp));
    if (percentSteps != percentStepsTmp || forceDraw == true)
    {
        percentSteps = percentStepsTmp;
        drawProgressBar(STEPS_BAR_CORD, TO_DAY_BAR_SIZE, percentSteps);
    }

    uint8_t minutes = timeRTCLocal.Minute;
    if (abs(weatherMinutes - minutes) > 25 || forceDraw == true)
    {
        debugLog("Drawing weather in watchface");
        setTextSize(1);
        setFont(getFont("dogicapixel4"));

#if WEATHER_INFO
        OM_OneHourWeather wData = weatherGetDataHourly(WEATHER_WATCHFACE_HOUR_OFFSET);
        if (wData.fine == true)
        {
            // Temp
            display.fillRect(TEMP_RECT_CORD, TEMP_RECT_SIZE, GxEPD_WHITE);
            writeTextReplaceBack(String(uint16_t(roundf(wData.temp))) + "C", TEMP_CORD, GxEPD_BLACK, GxEPD_WHITE, true, 1);
            // Humidity, already %
            weatherDrawHelper(wData.humidity, 99, 0, HUMIDITY_CORD);
            // Pressure, it's in hPa
            weatherDrawHelper(wData.pressure, PRESSURE_HPA_MAX, PRESSURE_HPA_MIN, PRESSURE_CORD);
            // Prepecitation propability, in %
            weatherDrawHelper(wData.precipitation, 99, 0, PREPICITATION_CORD);
            // Visibility, in meters
            weatherDrawHelper(wData.visibility, VISIBILITY_M_MAX, VISIBILITY_M_MIN, VISIBILITY_CORD);
            // Wind speed, km/h
            weatherDrawHelper(wData.wind_speed, WIND_SPEED_MAX, WIND_SPEED_MIN, WIND_SPEED_CORD);
            // Wind guts, km/h
            weatherDrawHelper(wData.wind_gust, WIND_SPEED_MAX, WIND_SPEED_MIN, WIND_GUTS_CORD);
            // Cloud cover, in %
            weatherDrawHelper(wData.cloud_cover, 99, 0, CLOUD_COVER_CORD);
            debugLog("Sunrise: " + String(wData.sunrise));
            // Sunrise
            writeTextReplaceBack(addZero(String(hour(wData.sunrise)), 2), SUNRISE_CORD);
            // Sunset
            writeTextReplaceBack(addZero(String(hour(wData.sunset)), 2), SUNSET_CORD);

            writeImageN(WEATHER_ICON_CORD, getImg("inkfield/" + getWeatherIcon(wData.weather_code)));
        }
        else
        {
            writeTextReplaceBack("ERR", TEMP_CORD, GxEPD_BLACK, GxEPD_WHITE, true, 1);
            writeTextReplaceBack("ER", HUMIDITY_CORD, GxEPD_BLACK, GxEPD_WHITE, true, 1);
            writeTextReplaceBack("ER", PRESSURE_CORD, GxEPD_BLACK, GxEPD_WHITE, true, 1);
            writeTextReplaceBack("ER", PREPICITATION_CORD, GxEPD_BLACK, GxEPD_WHITE, true, 1);
            writeTextReplaceBack("ER", VISIBILITY_CORD, GxEPD_BLACK, GxEPD_WHITE, true, 1);
            writeTextReplaceBack("ER", WIND_SPEED_CORD, GxEPD_BLACK, GxEPD_WHITE, true, 1);
            writeTextReplaceBack("ER", WIND_GUTS_CORD, GxEPD_BLACK, GxEPD_WHITE, true, 1);
            writeTextReplaceBack("ER", CLOUD_COVER_CORD, GxEPD_BLACK, GxEPD_WHITE, true, 1);
            writeTextReplaceBack("ER", SUNRISE_CORD, GxEPD_BLACK, GxEPD_WHITE, true, 1);
            writeTextReplaceBack("ER", SUNSET_CORD, GxEPD_BLACK, GxEPD_WHITE, true, 1);
            writeImageN(WEATHER_ICON_CORD, getImg("inkfield/" + getWeatherIcon(255)));
        }
#else
        writeTextReplaceBack("ERR", TEMP_CORD, GxEPD_BLACK, GxEPD_WHITE, true, 1);
        writeTextReplaceBack("ER", HUMIDITY_CORD, GxEPD_BLACK, GxEPD_WHITE, true, 1);
        writeTextReplaceBack("ER", PRESSURE_CORD, GxEPD_BLACK, GxEPD_WHITE, true, 1);
        writeTextReplaceBack("ER", PREPICITATION_CORD, GxEPD_BLACK, GxEPD_WHITE, true, 1);
        writeTextReplaceBack("ER", VISIBILITY_CORD, GxEPD_BLACK, GxEPD_WHITE, true, 1);
        writeTextReplaceBack("ER", WIND_SPEED_CORD, GxEPD_BLACK, GxEPD_WHITE, true, 1);
        writeTextReplaceBack("ER", WIND_GUTS_CORD, GxEPD_BLACK, GxEPD_WHITE, true, 1);
        writeTextReplaceBack("ER", CLOUD_COVER_CORD, GxEPD_BLACK, GxEPD_WHITE, true, 1);
        writeTextReplaceBack("ER", SUNRISE_CORD, GxEPD_BLACK, GxEPD_WHITE, true, 1);
        writeTextReplaceBack("ER", SUNSET_CORD, GxEPD_BLACK, GxEPD_WHITE, true, 1);
        writeImageN(WEATHER_ICON_CORD, getImg("inkfield/meteor"));
#endif
    }
}

static void showTimeFull()
{
#if LP_CORE
    screenTimeChanged = true;
#endif
    // Now UI
    setTextSize(1);
    setFont(TIME_FONT);
    writeTextReplaceBack(getHourMinute(timeRTCLocal), TIME_CORD);
}

static void initWatchface()
{
    writeImageN(0, 0, getImg("inkfield/watchface"));
    drawPosMarker();
}

static void drawDay()
{
    setFont(DATE_FONT);
    String dayDate = String(wFTime.Day);
    if (dayDate.length() < 2)
    {
        dayDate = "0" + dayDate;
    }
    writeTextReplaceBack(dayDate, DATE_CORD);

    setFont(DAY_NAME_FONT);
    String day = getDayName();
    day.toUpperCase();

    String previousDay = getDayName(-1);
    previousDay.toUpperCase();
    uint16_t wDay;
    uint16_t hDay;
    getTextBounds(previousDay, NULL, NULL, &wDay, &hDay);
    display.fillRect(DAY_NAME_CORD - hDay, wDay + 1, hDay + 1, GxEPD_WHITE); // Clear things up
    writeTextReplaceBack(day, DAY_NAME_CORD);
}

static void drawMonth()
{
    setFont(MONTH_NAME_FONT);
    String month = getMonthName(wFTime.Month);
    month.toUpperCase();
    writeTextReplaceBack(month, MONTH_NAME_CORD);
}

static void drawBattery()
{
    drawProgressBar(BATT_BAR_CORD, TO_DAY_BAR_SIZE, batteryPercantageWF);
}

const watchfaceDefOne inkFieldDef = {
    .drawTimeBeforeApply = drawTimeBeforeApply,
    .drawTimeAfterApply = drawTimeAfterApply,
    .drawDay = drawDay,
    .drawMonth = drawMonth,
    .showTimeFull = showTimeFull,
    .initWatchface = initWatchface,
    .drawBattery = drawBattery,
    .manageInput = inkFieldManageInput,

    .watchfaceModules = true,
    .watchfaceModSquare = {.size{.w = 177, .h = 37}, .cord{.x = 7, .y = 160}},
    .someDrawingSquare = {.size{.w = 200, .h = 139}, .cord{.x = 0, .y = 61}},
    .isModuleEngaged = []()
    {
        if (watchfacePos == MODULE_ENG_POS && positionEngaged == true)
        {
            return true;
        }
        return false;
    }};

#endif