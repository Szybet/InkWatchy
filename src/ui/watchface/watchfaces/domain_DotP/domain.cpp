#include "domain.h"
#include "dotput.h"
#include "domainWeather.h"
#include "domain_localization.h"
#include "rtcMem.h"

#if WATCHFACE_DOMAIN_DOTP

/*

Note, this is a full rip-off of Szybet's inkField, 
with a few other things added in for maximalist fun. 
Almost all functionality is the same as with inkField.

Subject to haptic and frenetic updates
#define FLOW_STATE 1

Might be better if SCREEN_FADING_AWAY_2_WORKAROUND=1

modifications:
+ most images redone (weather, moon, watchface)
+ dogicafont modified
  -> dot removed from 0
  -> width of most special characters reduced to 7 (5 pixels)
  -> ! looks like !!! (used for extreme aqi values)
+ aqi values with 7 severities, displayed as + = ~ - x * !
  -> first value (EU) is represented by the AQI bar
+ aqi bar displaying EU AQI out of 120, with tiny indicators
+ uv index bar (out of 12) with line indicator for clear sky uv
+ thermometre using real temp, ranging from -43.5 to +43.5. 
  -> this is just how it looked best for pixel markers, 
     also covers the actual range of the hell i live in (Canada).
+ changed inkField temp indicator to apparent temp
+ day/night indicator, designed to cutely fit with weather
+ expanded weathercode images to almost all available codes
+ wet bulb temperature, feels out of place/oversized
+ CAPE index, divided by 10. if you ever see 999, accept fate
+ Sunlight minutes in the day

- removed moon stats (they're neat but who honestly cares except hippies?)
- removed month and day bars, kept day info to let moonphase work correctly lol

*/

#define TIME_FONT getFont("inkfield/JackInput40")
#define DATE_FONT getFont("JackInput17")
#define DAY_NAME_FONT getFont("inkfield/Speculum13")
#define MONTH_NAME_FONT getFont("inkfield/Speculum9")

#define DAY_NAME_CORD 7, 88
#define DATE_CORD 7, 113
#define MONTH_NAME_CORD 39, 102
#define MONTH_NUMBER_CORD 39, 110
#define GENERAL_BAR_SIZE 54, 10

#define THERMO_BAR_SIZE 5, 29
#define THERMO_BAR_CORD 82, 82

#define AQI_BAR_CORD 136, 64
#define UV_BAR_CORD 136, 64 + 15
#define BATT_BAR_CORD 136, 64 + 15 + 15
#define STEPS_BAR_CORD 136, 64 + 15 + 15 + 15

#define TEMPS_TEXT_CORD 136, 117
#define TEMPS_BAR_CORD_X 136 + 21
#define TEMPS_BAR_CORD_Y 64 + 15 + 15 + 15
#define TEMPS_BAR_SIZE 33, 10

#define TIME_LETTERS_SPACING 36 // It differs, see above - so not it's static
#define TIME_CORD_X 11
#define TIME_CORD_Y 53
#define TIME_CORD TIME_CORD_X, TIME_CORD_Y

// Helper function to get time string based on WATCHFACE_12H setting
String getDomainTimeString(tmElements_t timeEl) {
#if WATCHFACE_12H
    // 12-hour format without AM/PM
    int hour = timeEl.Hour;
    if (hour == 0) hour = 12;
    else if (hour > 12) hour -= 12;
    
    String hourStr = String(hour);
    if (hourStr.length() == 1) hourStr = "0" + hourStr;  // Fixed: use "0" instead of " "
    
    String minuteStr = String(timeEl.Minute);
    if (minuteStr.length() == 1) minuteStr = "0" + minuteStr;
    
    return hourStr + ":" + minuteStr;
#else
    // 24-hour format
    String hourStr = String(timeEl.Hour);
    if (hourStr.length() == 1) hourStr = "0" + hourStr;
    
    String minuteStr = String(timeEl.Minute);
    if (minuteStr.length() == 1) minuteStr = "0" + minuteStr;
    
    return hourStr + ":" + minuteStr;
#endif
}

static void drawTimeBeforeApply()
{
    debugLog("Called");
    setTextSize(1);
    setFont(TIME_FONT);
    debugLog("Getting hour minute for rM.wFTime");
    String oldTime = getDomainTimeString(rM.wFTime);
    debugLog("Getting hour minute for timeRTCLocal");
    String newTime = getDomainTimeString(timeRTCLocal);

    for (int i = 0; i < 5; i++)
    {
        if (oldTime[i] != newTime[i])
        {
            String toWrite = String(newTime[i]);
            String oldWrite = String(oldTime[i]);
            String beforeString = oldTime.substring(0, i);
            String afterString = oldTime.substring(0, i + 1);

            uint16_t wAfter;
            getTextBounds(afterString, NULL, NULL, &wAfter, NULL);

            uint16_t wToWrite;
            uint16_t hToWrite;
            getTextBounds(oldWrite, NULL, NULL, &wToWrite, &hToWrite);

            uint16_t finalWidthStart = wAfter - wToWrite;

            dis->fillRect(TIME_CORD_X + finalWidthStart, TIME_CORD_Y - hToWrite, TIME_LETTERS_SPACING, hToWrite, SCWhite); // Clear things up
            writeTextReplaceBack(toWrite, TIME_CORD_X + finalWidthStart, TIME_CORD_Y);
        }
    }
}

static void drawTimeAfterApply(bool forceDraw)
{
    // keeping dayBar functions from inkField becuase moonphase wants it smh
    if (rM.domain.dayBar != timeRTCLocal.Day || forceDraw == true)
    {
        rM.domain.dayBar = timeRTCLocal.Day;

        domainDrawMoon();
    }

    // UV INDEX + UV INDEX CLEAR SKY
    OM_OneHourWeather wData = weatherGetDataHourly(0);
    if (wData.fine) {
        // Cap UV values at 12
        float uv_index = min(wData.uv_index, 12.0f);
        float uv_clear = min(wData.uv_index_clear_sky, 12.0f);
        
        // Calculate percentages
        uint8_t uv_percent = static_cast<uint8_t>((uv_index / 12.0f) * 100);
        uint8_t uv_clear_percent = static_cast<uint8_t>((uv_clear / 12.0f) * 100);
        
        // Draw main UV bar
        drawProgressBar(UV_BAR_CORD, GENERAL_BAR_SIZE, uv_percent);
        
        // Draw clear sky indicator if different value from UV Index
        // idk how to avoid needing to hardpoint x and y here
        if (uv_index != uv_clear) {
            int barWidth = 54; // Width from GENERAL_BAR_SIZE
            int x = 136; // X from UV_BAR_CORD
            int y = 79;  // Y from UV_BAR_CORD
            int lineX = x + (uv_clear_percent * barWidth) / 100;
            dis->drawFastVLine(lineX, y, 10, SCBlack);
        }

                float currentTemp = wData.temp;
        // Calculate the percentage fill for the thermometer bar
        float tempRange = 87.0; // from -43.5 to 43.5
        float tempOffset = 43.5; // offset to make the range start from 0
        float tempValue = currentTemp + tempOffset;
        float tempPercent = (tempValue / tempRange) * 100.0;

        // Ensure the percentage is within valid bounds
        tempPercent = constrain(tempPercent, 0, 100);

        drawTempBar(THERMO_BAR_CORD, THERMO_BAR_SIZE, tempPercent);
    } else {
        // Fallback: Show empty bar if no data
        drawProgressBar(UV_BAR_CORD, GENERAL_BAR_SIZE, 0);
         // Fallback, set at 1% due to possible rounding error, shouldn't be visible
        drawTempBar(THERMO_BAR_CORD, THERMO_BAR_SIZE, 1);
    }

// AIR QUALITY INDEX, EU STANDARDS
OM_OneHourAirQuality aq = airQualityGetDataHourly(WEATHER_WATCHFACE_HOUR_OFFSET);
uint8_t aqiPercent = 0;
if (aq.fine) {
    aqiPercent = (uint8_t)constrain((aq.european_aqi * 100) / 120, 0, 100);
}
drawProgressBar(AQI_BAR_CORD, GENERAL_BAR_SIZE, aqiPercent);

    // STEPS
#if ACC_ENABLED
    uint16_t percentStepsTmp = uint16_t(((float)getSteps() / (float)STEPS_GOAL) * 100.0);
    debugLog("percentStepsTmp: " + String(percentStepsTmp));
    if (rM.domain.percentSteps != percentStepsTmp || forceDraw == true)
    {
        rM.domain.percentSteps = percentStepsTmp;
        drawProgressBar(STEPS_BAR_CORD, GENERAL_BAR_SIZE, rM.domain.percentSteps);
    }
#else
#if TEMP_CHECKS_ENABLED
    uint16_t tempsPercents = ((rM.previousTemp - TEMP_MINIMUM) * 100) / (TEMP_MAXIMUM - TEMP_MINIMUM);
    debugLog("tempsPercents: " + String(tempsPercents));
    if (rM.domain.showedTemp != tempsPercents || forceDraw == true)
    {
        rM.domain.showedTemp = tempsPercents;
        setTextSize(1);
        setFont(getFont("dogicapixel_dotp4"));
        writeTextReplaceBack("   ", TEMPS_TEXT_CORD);
        // Use formatTemperature for device temperature (not weather)
        String tempStr = formatTemperature(rM.previousTemp);
        if(tempStr.length() > 3) {
            tempStr = tempStr.substring(0, 3);
        }
        writeTextReplaceBack(tempStr, TEMPS_TEXT_CORD);
        drawProgressBar(TEMPS_BAR_CORD_X, TEMPS_BAR_CORD_Y, TEMPS_BAR_SIZE, rM.domain.showedTemp);
    }
#endif
#endif

// 
    uint16_t weatherMinutes = timeRTCLocal.Minute + (60 * timeRTCLocal.Hour);
    if (abs(rM.domain.weatherMinutes - weatherMinutes) > 25|| forceDraw == true)
    {
        rM.domain.weatherMinutes = weatherMinutes;
        domainDrawWeather();
        drawProgressBar(AQI_BAR_CORD, GENERAL_BAR_SIZE, aqiPercent);
domainDrawMoon();
    }
}

static void showTimeFull()
{
#if LP_CORE
    screenTimeChanged = true;
#endif
    // Show time in format based on WATCHFACE_12H setting (but no AM/PM)
    setTextSize(1);
    setFont(TIME_FONT);
    writeTextReplaceBack(getDomainTimeString(timeRTCLocal), TIME_CORD);
}

/*
in config.h, add this:
#define PROPAGANDA = 0
to disable the slogan 美帝必败
"American Imperialism Will Fall"
(it's from Fallout 76) */
static void initWatchface()
{
#if defined(PROPAGANDA) && (PROPAGANDA == 1)
    #if ACC_ENABLED
        writeImageN(0, 0, getImg("domain/watchfaceLib"));
    #else
        writeImageN(0, 0, getImg("domain/watchfaceLibNoSteps"));
    #endif
#else
    #if ACC_ENABLED
        writeImageN(0, 0, getImg("domain/watchface"));
    #else
        writeImageN(0, 0, getImg("domain/watchfaceNoSteps"));
    #endif
#endif
    domainDrawPosMarker();
}

static void drawDay()
{
    setFont(DATE_FONT);
    String dayDate = String(rM.wFTime.Day);
    if (dayDate.length() < 2)
    {
        dayDate = "0" + dayDate;
    }
    writeTextReplaceBack(dayDate, DATE_CORD);

    setFont(DAY_NAME_FONT);
    // Use localization.h function for day names
    String day = getLocalizedDayByIndex(timeRTCLocal.Wday, 0); // Current day
    day.toUpperCase();

    String previousDay = getLocalizedDayByIndex(timeRTCLocal.Wday, -1); // Previous day
    previousDay.toUpperCase();
    uint16_t wDay;
    uint16_t hDay;
    getTextBounds(previousDay, NULL, NULL, &wDay, &hDay);
    dis->fillRect(DAY_NAME_CORD - hDay, wDay + 1, hDay + 1, SCWhite); // Clear things up
    writeTextReplaceBack(day, DAY_NAME_CORD);
}

static void drawMonth()
{
    setFont(MONTH_NAME_FONT);
    // Use localized month names
    String month = getLocalizedMonthName(rM.wFTime.Month);
    month.toUpperCase();
    writeTextReplaceBack(month, MONTH_NAME_CORD);

    String realMonthNumber = String(rM.wFTime.Month + 1);
    if (realMonthNumber.length() == 1)
    {
        realMonthNumber = "0" + realMonthNumber;
    }

    setFont(getFont("dogicapixel_dotp4"));
   String monthWithBrackets = "(" + realMonthNumber + ")";
   writeTextReplaceBack(monthWithBrackets, MONTH_NUMBER_CORD, SCBlack, SCWhite, true, 1, 1);
}

static void drawBattery()
{
    drawProgressBar(BATT_BAR_CORD, GENERAL_BAR_SIZE, rM.batteryPercantageWF);
}

const watchfaceDefOne domainDef = {
    .drawTimeBeforeApply = drawTimeBeforeApply,
    .drawTimeAfterApply = drawTimeAfterApply,
    .drawDay = drawDay,
    .drawMonth = drawMonth,
    .showTimeFull = showTimeFull,
    .initWatchface = initWatchface,
    .drawBattery = drawBattery,
    .manageInput = domainManageInput,

    .watchfaceModules = true,
    .watchfaceModSquare = {.size{.w = 177, .h = 37}, .cord{.x = 7, .y = 160}},
    .someDrawingSquare = {.size{.w = 200, .h = 139}, .cord{.x = 0, .y = 61}},
    .isModuleEngaged = []()
    {
        if (rM.domain.watchfacePos == MODULE_ENG_POS && rM.domain.positionEngaged == true)
        {
            return true;
        }
        return false;
    }};

#endif