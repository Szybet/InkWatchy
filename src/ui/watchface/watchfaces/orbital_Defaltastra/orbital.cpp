#include "orbital.h"

#if WATCHFACE_ORBITAL

#include "rtcMem.h"
#include <MoonPhase.h>

static MoonPhase moonPhase;

#define DEG2RAD 0.01745329252f

static float normalizeDegrees(float degrees)
{
    while (degrees < 0.0f)
    {
        degrees += 360.0f;
    }
    while (degrees >= 360.0f)
    {
        degrees -= 360.0f;
    }
    return degrees;
}

static int dayOfYear(int year, int month, int day)
{
    static const int monthOffsets[] = {0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334};
    int doy = monthOffsets[month - 1] + day;
    if (month > 2)
    {
        bool leapYear = (year % 4 == 0) && (year % 100 != 0 || year % 400 == 0);
        if (leapYear)
        {
            doy += 1;
        }
    }
    return doy;
}

static float computeSolarEventMinutes(bool sunrise, float latitude, float longitude, int timezoneOffsetHours, int year, int month, int day)
{
    const float zenith = 90.833f;
    const float lngHour = longitude / 15.0f;
    const int n = dayOfYear(year, month, day);
    const float t = n + ((sunrise ? 6.0f : 18.0f) - lngHour) / 24.0f;
    const float meanAnomaly = 0.9856f * t - 3.289f;
    float trueLongitude = meanAnomaly + 1.916f * sinf(meanAnomaly * DEG2RAD) + 0.020f * sinf(2.0f * meanAnomaly * DEG2RAD) + 282.634f;
    trueLongitude = normalizeDegrees(trueLongitude);

    float rightAscension = atanf(0.91764f * tanf(trueLongitude * DEG2RAD)) / DEG2RAD;
    rightAscension = normalizeDegrees(rightAscension);
    const float lQuadrant = floorf(trueLongitude / 90.0f) * 90.0f;
    const float raQuadrant = floorf(rightAscension / 90.0f) * 90.0f;
    rightAscension += lQuadrant - raQuadrant;
    rightAscension /= 15.0f;

    const float sinDeclination = 0.39782f * sinf(trueLongitude * DEG2RAD);
    const float cosDeclination = cosf(asinf(sinDeclination));
    const float cosHourAngle = (cosf(zenith * DEG2RAD) - (sinDeclination * sinf(latitude * DEG2RAD))) / (cosDeclination * cosf(latitude * DEG2RAD));

    if (cosHourAngle > 1.0f || cosHourAngle < -1.0f)
    {
        return -1.0f;
    }

    float hourAngle = sunrise ? 360.0f - (acosf(cosHourAngle) / DEG2RAD) : (acosf(cosHourAngle) / DEG2RAD);
    hourAngle /= 15.0f;

    float localMeanTime = hourAngle + rightAscension - 0.06571f * t - 6.622f;
    float universalTime = localMeanTime - lngHour;
    while (universalTime < 0.0f)
    {
        universalTime += 24.0f;
    }
    while (universalTime >= 24.0f)
    {
        universalTime -= 24.0f;
    }

    float localTime = universalTime + timezoneOffsetHours;
    while (localTime < 0.0f)
    {
        localTime += 24.0f;
    }
    while (localTime >= 24.0f)
    {
        localTime -= 24.0f;
    }

    return localTime * 60.0f;
}

static int daysInMonth(unsigned int year, unsigned int month)
{
    if (month == 2)
    {
        if (year % 4 != 0 || (year % 100 == 0 && year % 400 != 0))
        {
            return 28;
        }
        return 29;
    }
    if (month == 4 || month == 6 || month == 9 || month == 11)
    {
        return 30;
    }
    return 31; // Default case for months with 31 days
}

static uint8_t orbitalMonthForTimeLib(uint8_t month)
{
    // InkWatchy month is typically 0-11, TimeLib expects 1-12.
    if (month <= 11)
    {
        return month + 1;
    }
    return month;
}

static void fillArc2(float start_angle, float end_angle, unsigned int radius, unsigned int width, uint16_t colour, float step)
{
    const unsigned int center_x = 100;
    const unsigned int center_y = 100;

    for (float i = start_angle; i <= end_angle; i += step)
    {
        float cx1 = center_x + (radius - width / 2.0f) * cosf((i - 90.0f) * DEG2RAD);
        float cy1 = center_y + (radius - width / 2.0f) * sinf((i - 90.0f) * DEG2RAD);
        float cx2 = center_x + (radius + width / 2.0f) * cosf((i - 90.0f) * DEG2RAD);
        float cy2 = center_y + (radius + width / 2.0f) * sinf((i - 90.0f) * DEG2RAD);
        dis->drawLine(cx1, cy1, cx2, cy2, colour);
    }
}

static void fillArc3(float start_angle, float end_angle, unsigned int radius, unsigned int width, uint16_t colour, float step)
{
    const unsigned int center_x = 100;
    const unsigned int center_y = 100;

    for (float i = start_angle; i <= end_angle - step; i += step)
    {
        float cx1 = center_x + (radius - width / 2.0f) * cosf((i - 90.0f) * DEG2RAD);
        float cy1 = center_y + (radius - width / 2.0f) * sinf((i - 90.0f) * DEG2RAD);
        float cx2 = center_x + (radius + width / 2.0f) * cosf((i - 90.0f) * DEG2RAD);
        float cy2 = center_y + (radius + width / 2.0f) * sinf((i - 90.0f) * DEG2RAD);
        float cx3 = center_x + (radius - width / 2.0f) * cosf((i - 90.0f + step) * DEG2RAD);
        float cy3 = center_y + (radius - width / 2.0f) * sinf((i - 90.0f + step) * DEG2RAD);
        float cx4 = center_x + (radius + width / 2.0f) * cosf((i - 90.0f + step) * DEG2RAD);
        float cy4 = center_y + (radius + width / 2.0f) * sinf((i - 90.0f + step) * DEG2RAD);

        dis->fillTriangle(cx1, cy1, cx2, cy2, cx3, cy3, colour);
        dis->fillTriangle(cx2, cy2, cx3, cy3, cx4, cy4, colour);
    }
}

static void fillEllipse(int16_t x0, int16_t y0, int16_t rx, int16_t ry, uint16_t color)
{
    int16_t rx2 = rx * rx;
    int16_t ry2 = ry * ry;
    int16_t twoRx2 = 2 * rx2;
    int16_t twoRy2 = 2 * ry2;
    int16_t p;
    int16_t x = 0;
    int16_t y = ry;
    int16_t px = 0;
    int16_t py = twoRx2 * y;

    dis->startWrite();
    dis->writeFastVLine(x0, y0 - y, 2 * y, color);

    p = (int16_t)(ry2 - rx2 * ry + 0.25 * rx2);
    while (px < py)
    {
        x++;
        px += twoRy2;
        if (p < 0)
        {
            p += ry2 + px;
        }
        else
        {
            y--;
            py -= twoRx2;
            p += ry2 + px - py;
        }
        dis->writeFastVLine(x0 + x, y0 - y, 2 * y, color);
        dis->writeFastVLine(x0 - x, y0 - y, 2 * y, color);
    }

    p = (int16_t)(ry2 * (x + 0.5f) * (x + 0.5f) + rx2 * (y - 1) * (y - 1) - rx2 * ry2);
    while (y > 0)
    {
        y--;
        py -= twoRx2;
        if (p > 0)
        {
            p += rx2 - py;
        }
        else
        {
            x++;
            px += twoRy2;
            p += rx2 - py + px;
        }
        dis->writeFastVLine(x0 + x, y0 - y, 2 * y, color);
        dis->writeFastVLine(x0 - x, y0 - y, 2 * y, color);
    }

    dis->endWrite();
}

static void orbitalDrawWatchMinute()
{
    float minuteAngle = 360.0f * (timeRTCLocal.Minute / 60.0f);
    int tick = 360 / 60;

    if (timeRTCLocal.Minute == 0)
    {
        minuteAngle = 360.0f;
    }

    fillArc3(0.0f, minuteAngle, 86, 13, GxEPD_BLACK, 1);
    if (minuteAngle == 360.0f)
    {
        fillArc2(tick, minuteAngle, 86, 13, GxEPD_WHITE, tick);
    }
    else
    {
        fillArc2(tick, minuteAngle - tick, 86, 13, GxEPD_WHITE, tick);
        fillArc2(minuteAngle + tick, 360 - tick, 83, 7, GxEPD_BLACK, tick);
    }
}

static void orbitalDrawWatchHour()
{
    float hourAngle = 360.0f * (timeRTCLocal.Hour / 24.0f);
    int tick = 360 / 24;

    if (timeRTCLocal.Hour == 0)
    {
        hourAngle = 360.0f;
    }

    fillArc3(0.0f, hourAngle, 68, 15, GxEPD_BLACK, 1);
    if (hourAngle == 360.0f)
    {
        fillArc2(tick, hourAngle, 68, 15, GxEPD_WHITE, tick);
    }
    else
    {
        fillArc2(tick, hourAngle - tick, 68, 15, GxEPD_WHITE, tick);
        fillArc2(hourAngle + tick, 360 - tick, 64, 8, GxEPD_BLACK, tick);
    }
}

static void orbitalDrawWatchDay()
{
    float dayAngle = 360.0f * (timeRTCLocal.Day / 31.0f);
    int tick = 360 / daysInMonth(tmYearToCalendar(timeRTCLocal.Year), timeRTCLocal.Month);

    fillArc3(0.0f, dayAngle, 40, 17, GxEPD_BLACK, 2);
    if (dayAngle == 360.0f)
    {
        fillArc2(tick, dayAngle, 40, 17, GxEPD_WHITE, tick);
    }
    else
    {
        fillArc2(tick, dayAngle - tick, 40, 17, GxEPD_WHITE, tick);
        fillArc2(dayAngle + tick, 360 - tick, 39, 9, GxEPD_BLACK, tick);
    }
}

static void orbitalDrawWatchMonth()
{
    float monthAngle = 360.0f * (timeRTCLocal.Month / 12.0f);
    int tick = 360 / 12;

    fillArc3(0.0f, monthAngle, 18, 19, GxEPD_BLACK, 2);
    if (monthAngle == 360.0f)
    {
        fillArc2(tick, monthAngle, 18, 19, GxEPD_WHITE, tick);
    }
    else
    {
        fillArc2(tick, monthAngle - tick, 18, 19, GxEPD_WHITE, tick);
        fillArc2(monthAngle + tick, 360 - tick, 17, 10, GxEPD_BLACK, tick);
    }
}

static void orbitalDrawBattery()
{
    dis->setCursor(158, 12);
    dis->setFont(&FreeSansBold9pt7b);
    dis->println(analogReadMilliVolts(BATT_ADC_PIN) * 2);
    dis->setCursor(170, 28);
    dis->println("mV");
}

static void orbitalDrawMoon()
{
    time_t unixTime = makeTime(timeRTCLocal);
    moonPhase.calculate(unixTime);
    int moonphase = (int)lround(moonPhase.phase * 29.0);

    int radius = 16;
    int moonCenterX = 17;
    int moonCenterY = 183;

    dis->fillCircle(moonCenterX, moonCenterY, radius, GxEPD_BLACK);

    if (moonphase > 0 && moonphase <= 15)
    {
        fillEllipse(moonCenterX - ((((moonphase - 1) / 28.0f) - 0.5f) * radius * 2), moonCenterY, (15 / 14) * (moonphase - 1) + 1, radius, GxEPD_WHITE);
    }
    if (moonphase > 15 && moonphase < 29)
    {
        fillEllipse(moonCenterX - ((((moonphase - 1) / 28.0f) - 0.5f) * radius * 2), moonCenterY, (-15 / 13) * (moonphase - 15) + 16, radius, GxEPD_WHITE);
    }
    dis->drawCircle(moonCenterX, moonCenterY, radius, GxEPD_BLACK);
}

static void orbitalDrawNightTime()
{
    int year = tmYearToCalendar(timeRTCLocal.Year);

    // Prefer weather coordinates if provided in confidential.h, otherwise fall back to ORBITAL_ macros
    float lat = ORBITAL_LAT;
    float lon = ORBITAL_LON;
    if (String(WEATHER_LATIT).length() > 0 && String(WEATHER_LONGTIT).length() > 0)
    {
        lat = String(WEATHER_LATIT).toFloat();
        lon = String(WEATHER_LONGTIT).toFloat();
    }

    // Prefer runtime timezone offset (seconds) if available, convert to hours
    int tzOffsetHours = ORBITAL_GMT_OFFSET;
    if (timeZoneOffset != 0)
    {
        tzOffsetHours = int(timeZoneOffset / 60);
    }

    float sunrise = computeSolarEventMinutes(true, lat, lon, tzOffsetHours, year, timeRTCLocal.Month, timeRTCLocal.Day);
    float sunset = computeSolarEventMinutes(false, lat, lon, tzOffsetHours, year, timeRTCLocal.Month, timeRTCLocal.Day);

    if (sunrise < 0.0f || sunset < 0.0f)
    {
        return;
    }

    fillArc3(sunset / 1440.0f * 360.0f, sunrise / 1440.0f * 360.0f + 360.0f, 55, 2, GxEPD_BLACK, 1);
}

void orbitalDrawWatchFace()
{
    dis->fillScreen(GxEPD_WHITE);
    dis->setTextColor(GxEPD_BLACK);

    orbitalDrawMoon();
    orbitalDrawWatchMinute();
    orbitalDrawWatchHour();
    orbitalDrawWatchDay();
    orbitalDrawWatchMonth();

    dis->setFont(&FreeSansBold9pt7b);
    dis->setCursor(0, 12);
    tmElements_t localTime = timeRTCLocal;
    localTime.Month = orbitalMonthForTimeLib(localTime.Month);
    dis->println(dayShortStr(weekday(makeTime(localTime))));

    dis->setCursor(160, 198);
    dis->println(tmYearToCalendar(timeRTCLocal.Year));

    orbitalDrawBattery();
    orbitalDrawNightTime();
}

static void orbitalDrawFull()
{
    orbitalDrawWatchFace();
}

static void drawTimeBeforeApply()
{
    orbitalDrawFull();
}

static void drawTimeAfterApply(bool forceDraw)
{
    (void)forceDraw;
}

static void drawDay()
{
    orbitalDrawFull();
}

static void drawMonth()
{
    orbitalDrawFull();
}

static void showTimeFull()
{
    orbitalDrawFull();
}

static void initWatchface()
{
    orbitalDrawFull();
}

static void drawBattery()
{
    orbitalDrawFull();
}

static void manageInput(buttonState bt)
{
    if (bt == Menu)
    {
        generalSwitch(mainMenu);
        return;
    }
}

static bool orbital_isModuleEngaged()
{
    return false;
}

const watchfaceDefOne orbitalDef = {
    .drawTimeBeforeApply = drawTimeBeforeApply,
    .drawTimeAfterApply = drawTimeAfterApply,
    .drawDay = drawDay,
    .drawMonth = drawMonth,
    .showTimeFull = showTimeFull,
    .initWatchface = initWatchface,
    .drawBattery = drawBattery,
    .manageInput = manageInput,

    .watchfaceModules = false,
    .watchfaceModSquare = {.size{.w = 0, .h = 0}, .cord{.x = 0, .y = 0}},
    .someDrawingSquare = {.size{.w = 200, .h = 200}, .cord{.x = 0, .y = 0}},
    .isModuleEngaged = orbital_isModuleEngaged,
    .lpCoreScreenPrepareCustom = NULL,
};

#endif
