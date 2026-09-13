#include "starfield.h"

#if WATCHFACE_STARFIELD

#include <math.h>
#include "Dusk2Dawn.h"
#include "moonPhaser.h"
#include "Seven_Segment10pt7b.h"
#include "icons.h"
#include "confidential.h"
#include "rtcMem.h"

// In the original Starfield watchface there was no temperature.
// Kept in code and hidden for now; set to 1 if you wish to enable temperature display in the future.
#define STARFIELD_SHOW_TEMPERATURE 0

#if STARFIELD_SHOW_TEMPERATURE
#include "DSEG7_Classic_Bold_25.h"
#include "DSEG7_Classic_Regular_39.h"
#endif

// Restores any rectangular region directly from the background 'field' bitmap in PROGMEM
static void restoreFieldRegion(int rx, int ry, int rw, int rh)
{
    if (rx < 0 || ry < 0 || rx + rw > 200 || ry + rh > 200) return;
    for (int y = ry; y < ry + rh; y++)
    {
        int rowStart = y * 25;
        for (int x = rx; x < rx + rw; x++)
        {
            uint8_t b = pgm_read_byte(&field[rowStart + (x / 8)]);
            bool isBlack = (b & (0x80 >> (x % 8))) != 0;
            dis->drawPixel(x, y, isBlack ? SCBlack : SCWhite);
        }
    }
}

static void drawDigit(int x, int y, int digit)
{
    if (digit < 0 || digit > 9) return;
    const unsigned char *digits[] = {dd_0, dd_1, dd_2, dd_3, dd_4, dd_5, dd_6, dd_7, dd_8, dd_9};
    dis->drawBitmap(x, y, digits[digit], 16, 25, SCBlack, SCWhite);
}

static void drawSmallDigit(int x, int y, int digit) 
{
    if (digit < 0 || digit > 9) return;
    const unsigned char* smallDigits[] = {num_0, num_1, num_2, num_3, num_4, num_5, num_6, num_7, num_8, num_9};
    dis->drawBitmap(x, y, smallDigits[digit], 3, 5, SCBlack, SCWhite);
}

static void drawLargeDigit(int x, int y, int digit)
{
    if (digit < 0 || digit > 9) return;
    const unsigned char* largeDigits[] = {fd_0, fd_1, fd_2, fd_3, fd_4, fd_5, fd_6, fd_7, fd_8, fd_9};
    dis->drawBitmap(x, y, largeDigits[digit], 33, 53, SCBlack, SCWhite);
}

static void drawField()
{
    dis->drawBitmap(0, 0, field, 200, 200, SCBlack);
}

static void starfieldShowTimeFull()
{
    long totalMinutes = timeRTCLocal.Hour * 60 + timeRTCLocal.Minute;
    int hour = totalMinutes / 60;
    bool is24H = !WATCHFACE_12H;
    bool isPm = (hour >= 12);

    if (!is24H) {
        if (hour == 0) hour = 12;
        else if (hour > 12) hour -= 12;
        dis->drawBitmap(7, 60, isPm ? pm : am, 25, 9, SCBlack, SCWhite);
        rM.starfield.lastIsPm = isPm;
    }

    int minute = totalMinutes % 60;
    uint8_t hourTens = hour >= 10 ? hour / 10 : 0;
    uint8_t hourUnits = hour % 10;
    uint8_t minuteTens = minute >= 10 ? minute / 10 : 0;
    uint8_t minuteUnits = minute % 10;

    drawLargeDigit(11, 5, hourTens);
    drawLargeDigit(55, 5, hourUnits);
    drawLargeDigit(111, 5, minuteTens);
    drawLargeDigit(155, 5, minuteUnits);

    rM.starfield.lastHourTens = hourTens;
    rM.starfield.lastHourUnits = hourUnits;
    rM.starfield.lastMinuteTens = minuteTens;
    rM.starfield.lastMinuteUnits = minuteUnits;
}

static void starfieldDrawTimeBeforeApply()
{
    long totalMinutes = timeRTCLocal.Hour * 60 + timeRTCLocal.Minute;
    int hour = totalMinutes / 60;
    bool is24H = !WATCHFACE_12H;
    bool isPm = (hour >= 12);

    if (!is24H) {
        if (hour == 0) hour = 12;
        else if (hour > 12) hour -= 12;
        if (isPm != rM.starfield.lastIsPm) {
            dis->drawBitmap(7, 60, isPm ? pm : am, 25, 9, SCBlack, SCWhite);
            rM.starfield.lastIsPm = isPm;
        }
    }

    int minute = totalMinutes % 60;
    uint8_t hourTens = hour >= 10 ? hour / 10 : 0;
    uint8_t hourUnits = hour % 10;
    uint8_t minuteTens = minute >= 10 ? minute / 10 : 0;
    uint8_t minuteUnits = minute % 10;

    if (minuteUnits != rM.starfield.lastMinuteUnits) {
        drawLargeDigit(155, 5, minuteUnits);
        rM.starfield.lastMinuteUnits = minuteUnits;
    }
    if (minuteTens != rM.starfield.lastMinuteTens) {
        drawLargeDigit(111, 5, minuteTens);
        rM.starfield.lastMinuteTens = minuteTens;
    }
    if (hourUnits != rM.starfield.lastHourUnits) {
        drawLargeDigit(55, 5, hourUnits);
        rM.starfield.lastHourUnits = hourUnits;
    }
    if (hourTens != rM.starfield.lastHourTens) {
        drawLargeDigit(11, 5, hourTens);
        rM.starfield.lastHourTens = hourTens;
    }
}

static void updateSunriseSunset()
{
    int sunrise = 360;  // 06:00 default fallback
    int sunset = 1080;  // 18:00 default fallback

    if (strlen(WEATHER_LATIT) > 0 && strlen(WEATHER_LONGTIT) > 0)
    {
        float lat = String(WEATHER_LATIT).toFloat();
        float lon = String(WEATHER_LONGTIT).toFloat();
        float tzHours = -((float)timeZoneOffset / 3600.0f);

        int32_t day = timeRTCLocal.Day;
        int32_t month = timeRTCLocal.Month + 1; // 1-indexed (1-12)
        int year = tmYearToCalendar(timeRTCLocal.Year);

        Dusk2Dawn location(lat, lon, tzHours);
        int calcSunrise = location.sunrise(year, month, day, false);
        int calcSunset = location.sunset(year, month, day, false);

        if (calcSunrise >= 0 && calcSunset >= 0 && calcSunset > calcSunrise)
        {
            sunrise = calcSunrise;
            sunset = calcSunset;
        }
    }

    rM.starfield.sunriseMinutes = sunrise;
    rM.starfield.sunsetMinutes = sunset;
}

static void starfieldDrawDay()
{
    // 1. Calendar / Date
    dis->setFont(&Seven_Segment10pt7b);
    dis->setTextSize(1);
    dis->setTextColor(SCBlack);

    int day = timeRTCLocal.Day;
    int dayOfWeek = timeRTCLocal.Wday;
    int month = timeRTCLocal.Month; // 0-indexed in InkWatchy (0 = Jan)
    int year = tmYearToCalendar(timeRTCLocal.Year);

    // Restore background for variable-width text areas
    restoreFieldRegion(15, 70, 68, 18);
    restoreFieldRegion(45, 96, 36, 16);

    String dayOfWeekName = dayStr(dayOfWeek);
    if (dayOfWeekName.length() >= 3)
    {
        dayOfWeekName = dayOfWeekName.substring(0, dayOfWeekName.length() - 3);
    }
    int16_t x1, y1;
    uint16_t textWidth, textHeight;
    dis->getTextBounds(dayOfWeekName, 5, 85, &x1, &y1, &textWidth, &textHeight);
    if (dayOfWeek == 4) // Wednesday
    {
        textWidth -= 5;
    }
    dis->setCursor(76 - textWidth, 86);
    dis->print(dayOfWeekName);

    String monthName = monthShortStr(month + 1); // 1-indexed for monthShortStr
    dis->getTextBounds(monthName, 60, 110, &x1, &y1, &textWidth, &textHeight);
    dis->setCursor(79 - textWidth, 110);
    dis->print(monthName);

    int dayTens = day / 10;
    int dayUnits = day % 10;
    int yearThousands = year / 1000;
    int tempYear = year % 1000;
    int yearHundreds = tempYear / 100;
    tempYear %= 100;
    int yearTens = tempYear / 10;
    int yearUnits = tempYear % 10;

    drawDigit(8, 95, dayTens);
    drawDigit(29, 95, dayUnits);
    drawDigit(8, 129, yearThousands);
    drawDigit(29, 129, yearHundreds);
    drawDigit(50, 129, yearTens);
    drawDigit(71, 129, yearUnits);

    // 2. Sunrise / Sunset Times Calculation
    updateSunriseSunset();

    int sunriseHour = rM.starfield.sunriseMinutes / 60;
    int sunriseMinute = rM.starfield.sunriseMinutes % 60;
    int sunsetHour = rM.starfield.sunsetMinutes / 60;
    int sunsetMinute = rM.starfield.sunsetMinutes % 60;

    bool Use24HourClock = !WATCHFACE_12H;
    if (!Use24HourClock) {
        if (sunriseHour == 0) sunriseHour = 12;
        else if (sunriseHour > 12) sunriseHour -= 12;

        if (sunsetHour == 0) sunsetHour = 12;
        else if (sunsetHour > 12) sunsetHour -= 12;
    }

    int sunsetHourTens = sunsetHour / 10;
    int sunsetHourUnits = sunsetHour % 10;
    int sunsetMinuteTens = sunsetMinute / 10;
    int sunsetMinuteUnits = sunsetMinute % 10;

    int sunriseHourTens = sunriseHour / 10;
    int sunriseHourUnits = sunriseHour % 10;
    int sunriseMinuteTens = sunriseMinute / 10;
    int sunriseMinuteUnits = sunriseMinute % 10;

    drawSmallDigit(116, 67, sunsetHourTens);
    drawSmallDigit(120, 67, sunsetHourUnits);
    drawSmallDigit(128, 67, sunsetMinuteTens);
    drawSmallDigit(132, 67, sunsetMinuteUnits);
    drawSmallDigit(116, 137, sunriseHourTens);
    drawSmallDigit(120, 137, sunriseHourUnits);
    drawSmallDigit(128, 137, sunriseMinuteTens);
    drawSmallDigit(132, 137, sunriseMinuteUnits);

    // 3. Moon Phase
    moonPhaser moonP;
    double hour = timeRTCLocal.Hour + (timeRTCLocal.Minute / 60.0);
    moonData_t moon = moonP.getPhase(year, month + 1, day, hour);

    int angle = moon.angle;
    double percentLit = moon.percentLit;

    const unsigned char *waxingBitmaps[] = {luna1, luna12, luna11, luna10, luna9, luna8, luna7};
    const unsigned char *waningBitmaps[] = {luna1, luna2, luna3, luna4, luna5, luna6, luna7};
    const unsigned char **bitmaps = (angle <= 180) ? waxingBitmaps : waningBitmaps;

    int index;
    if (percentLit < 0.1) index = 0;
    else if (percentLit < 0.25) index = 1;
    else if (percentLit < 0.4) index = 2;
    else if (percentLit < 0.6) index = 3;
    else if (percentLit < 0.75) index = 4;
    else if (percentLit < 0.9) index = 5;
    else index = 6;

    if (index != rM.starfield.lastMoonIndex)
    {
        restoreFieldRegion(131, 74, 61, 61);
        dis->drawBitmap(131, 74, bitmaps[index], 61, 61, SCBlack);
        rM.starfield.lastMoonIndex = index;
    }
}

static void starfieldDrawBattery()
{
    int maxWidth = 37;
    uint32_t batteryPercent = rM.bat.percentage;
    if (batteryPercent > 100) batteryPercent = 100;
    int batteryLevel = (batteryPercent * maxWidth) / 100;

    if (batteryLevel < maxWidth)
    {
        restoreFieldRegion(155 + batteryLevel, 169, maxWidth - batteryLevel, 15);
    }
    if (batteryLevel > 0)
    {
        dis->fillRect(155, 169, batteryLevel, 15, SCBlack);
    }
}

#if STARFIELD_SHOW_TEMPERATURE
#if WEATHER_INFO
static const unsigned char* getStarfieldWeatherIcon(uint8_t wmoCode)
{
    switch (wmoCode)
    {
    case 0:
        return sunny;
    case 1:
    case 2:
        return cloudsun;
    case 3:
        return cloudy;
    case 45:
    case 48:
        return atmosphere;
    case 51:
    case 53:
    case 55:
    case 56:
    case 57:
        return drizzle;
    case 61:
    case 63:
    case 65:
    case 66:
    case 67:
    case 80:
    case 81:
    case 82:
        return rain;
    case 71:
    case 73:
    case 75:
    case 77:
    case 85:
    case 86:
        return snow;
    case 95:
    case 96:
    case 99:
        return thunderstorm;
    default:
        return sunny;
    }
}
#endif

static void drawWeather()
{
#if WEATHER_INFO
    OM_OneHourWeather wData = weatherGetDataHourly(WEATHER_WATCHFACE_HOUR_OFFSET);
    if (!wData.fine)
    {
        return;
    }

    int temperature = int(round(getDisplayTemperature(wData.temp)));

    dis->setFont(&DSEG7_Classic_Regular_39);
    int16_t x1, y1;
    uint16_t w, h;
    dis->getTextBounds(String(temperature), 0, 0, &x1, &y1, &w, &h);

    if (159 - w - x1 > 87)
    {
        dis->setCursor(159 - w - x1, 150);
    }
    else
    {
        dis->setFont(&DSEG7_Classic_Bold_25);
        dis->getTextBounds(String(temperature), 0, 0, &x1, &y1, &w, &h);
        dis->setCursor(159 - w - x1, 136);
    }

    dis->println(temperature);
    dis->drawBitmap(165, 110, (WATCHFACE_TEMP_TYPE == CELSIUS) ? celsius : fahrenheit, 26, 20, SCBlack);

    const unsigned char *weatherIcon = getStarfieldWeatherIcon(wData.weather_code);
    if (weatherIcon != nullptr)
    {
        dis->drawBitmap(145, 158, weatherIcon, 48, 32, SCBlack);
    }
#endif
}
#endif

static void starfieldDrawTimeAfterApply(bool forceDraw)
{
    // 1. Steps
    uint32_t stepCount = getSteps();
    if (stepCount != rM.starfield.lastSteps || forceDraw)
    {
        rM.starfield.lastSteps = stepCount;
        uint32_t stepGoal = STEPS_GOAL;
        if (stepGoal == 0) stepGoal = 10000;
        uint32_t stepBarLength = 61 * stepCount / stepGoal;
        if (stepBarLength > 61) stepBarLength = 61;

        if (stepBarLength < 61)
        {
            restoreFieldRegion(131 + stepBarLength, 148, 61 - stepBarLength, 9);
        }
        if (stepBarLength > 0)
        {
            dis->fillRect(131, 148, stepBarLength, 9, SCBlack);
        }

        uint32_t tempSteps = stepCount;
        int stepThousands = tempSteps / 10000;
        tempSteps %= 10000;
        int stepThousandsUnits = tempSteps / 1000;
        tempSteps %= 1000;
        int stepHundreds = tempSteps / 100;
        tempSteps %= 100;
        int stepTens = tempSteps / 10;
        int stepUnits = tempSteps % 10;

        drawDigit(8, 165, stepThousands);
        drawDigit(29, 165, stepThousandsUnits);
        drawDigit(50, 165, stepHundreds);
        drawDigit(71, 165, stepTens);
        drawDigit(92, 165, stepUnits);
    }

    // 2. Sun Position
    long currentTimeInMinutes = timeRTCLocal.Hour * 60 + timeRTCLocal.Minute;
    int sunPosition = 0;
    int sunrise = rM.starfield.sunriseMinutes;
    int sunset = rM.starfield.sunsetMinutes;
    if (sunset > sunrise)
    {
        sunPosition = (currentTimeInMinutes - sunrise) * 60 / (sunset - sunrise);
        if (currentTimeInMinutes > sunset) {
            sunPosition = 60;
        } else if (currentTimeInMinutes < sunrise) {
            sunPosition = 0;
        }
    }

    if (sunPosition != rM.starfield.lastSunPosition || forceDraw)
    {
        if (!forceDraw && rM.starfield.lastSunPosition >= 0)
        {
            restoreFieldRegion(110, 132 - rM.starfield.lastSunPosition, 3, 5);
        }
        dis->drawBitmap(110, 132 - sunPosition, arr, 3, 5, SCBlack);
        rM.starfield.lastSunPosition = sunPosition;
    }

    // 3. WiFi
    bool isWifiConnected = (WiFi.status() == WL_CONNECTED);
    if (isWifiConnected != rM.starfield.lastWifiStatus || forceDraw)
    {
        rM.starfield.lastWifiStatus = isWifiConnected;
        dis->drawBitmap(118, 168, isWifiConnected ? wifi : wifioff, 25, 18, SCBlack, SCWhite);
    }

#if STARFIELD_SHOW_TEMPERATURE
    drawWeather();
#endif
}

static void starfieldInitWatchface()
{
    dis->fillScreen(SCWhite);
    dis->setTextColor(SCBlack);
    drawField();

    // Ensure sunrise and sunset minutes are calculated before drawTimeAfterApply runs in wManageOneDrawAll
    updateSunriseSunset();

    // Reset RTC cache to force-draw all components during wManageOneDrawAll
    rM.starfield.lastHourTens = 255;
    rM.starfield.lastHourUnits = 255;
    rM.starfield.lastMinuteTens = 255;
    rM.starfield.lastMinuteUnits = 255;
    rM.starfield.lastSteps = 0xFFFFFFFF;
    rM.starfield.lastSunPosition = -1;
    rM.starfield.lastMoonIndex = -1;
    rM.starfield.lastWifiStatus = !WiFi.isConnected();
}

const watchfaceDefOne starfieldDef = {
    .drawTimeBeforeApply = starfieldDrawTimeBeforeApply,
    .drawTimeAfterApply = starfieldDrawTimeAfterApply,
    .drawDay = starfieldDrawDay,
    .drawMonth = []() {},
    .showTimeFull = starfieldShowTimeFull,
    .initWatchface = starfieldInitWatchface,
    .drawBattery = starfieldDrawBattery,
    .manageInput = [](buttonState bt)
    {
        switch (bt)
        {
        case Menu:
            generalSwitch(mainMenu);
            break;
#if LONG_BACK_FULL_REFRESH
        case LongBack:
            debugLog("Long back - full refresh");
            updateDisplay(FULL_UPDATE);
            break;
#endif
        default:
            break;
        }
    },
    .watchfaceModules = false,
    .watchfaceModSquare = {.size{.w = 0, .h = 0}, .cord{.x = 0, .y = 0}},
    .someDrawingSquare = {.size{.w = 0, .h = 0}, .cord{.x = 0, .y = 0}},
    .isModuleEngaged = []() { return false; },
    .lpCoreScreenPrepareCustom = NULL,
};

#endif