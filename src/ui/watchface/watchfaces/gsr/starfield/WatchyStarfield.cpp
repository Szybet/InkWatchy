#include "minimalDefines.h"
#if GSR_WATCHFACES && GSR_STARFIELD

#include "WatchyStarfield.h"

// change it to your location
// latitude, longitude, timezone
// Currently set to Wellington, NZ
#define LOC String(WEATHER_LATIT).toFloat(), String(WEATHER_LONGTIT).toFloat(), 0

WatchyGSR watchyGSR;
moonPhaser moonP;

bool IsTwentyFourHour()
{
    return !watchyGSR.IsAM() && !watchyGSR.IsPM();
}

void WatchyStarfield::handleButtonPress(uint8_t SwitchNumber)
{
    //log_e("Button Pressed");
}

void drawDigit(int x, int y, int digit)
{
    const unsigned char *digits[] = {dd_0, dd_1, dd_2, dd_3, dd_4, dd_5, dd_6, dd_7, dd_8, dd_9};
    WatchyGSR::display.drawBitmap(x, y, digits[digit], 16, 25, watchyGSR.ForeColor());
}

void drawSmallDigit(int x, int y, int digit) 
{
    const unsigned char* smallDigits[] = {num_0, num_1, num_2, num_3, num_4, num_5, num_6, num_7, num_8, num_9};
    WatchyGSR::display.drawBitmap(x, y, smallDigits[digit], 3, 5, watchyGSR.ForeColor());
}

void drawLargeDigit(int x, int y, int digit)
{
    const unsigned char* largeDigits[] = {fd_0, fd_1, fd_2, fd_3, fd_4, fd_5, fd_6, fd_7, fd_8, fd_9};
    WatchyGSR::display.drawBitmap(x, y, largeDigits[digit], 33, 53, watchyGSR.ForeColor());
}

void WatchyStarfield::drawWatchFace()
{
    WatchyGSR::display.fillScreen(watchyGSR.BackColor());
    WatchyGSR::display.setTextColor(watchyGSR.ForeColor());

    drawField();
    drawTime();
    drawDate();
    drawSteps();
    drawBattery();

    bool isWifiConnected = watchyGSR.WiFiStatus() == WL_CONNECTED;
    WatchyGSR::display.drawBitmap(118, 168, isWifiConnected ? wifi : wifioff, 25, 18, watchyGSR.ForeColor());

    //drawWeather();

    drawMoon();
    drawSun();
}

void WatchyStarfield::drawTime()
{
    WatchyGSR::display.setFont(&DSEG7_Classic_Bold_53);
    WatchyGSR::display.setCursor(6, 53 + 5);

    long totalMinutes = WatchTime.Local.Hour * 60 + WatchTime.Local.Minute;
    int hour = totalMinutes / 60;

    bool Use24HourClock = IsTwentyFourHour();

    if (!Use24HourClock && hour >= 12) {
        WatchyGSR::display.fillRect(7, 60, 25, 9, watchyGSR.BackColor());
        WatchyGSR::display.drawBitmap(7, 60, pm, 25, 9, watchyGSR.ForeColor());
    } else if (!Use24HourClock && hour < 12) {
        WatchyGSR::display.fillRect(7, 60, 25, 9, watchyGSR.BackColor());
        WatchyGSR::display.drawBitmap(7, 60, am, 25, 9, watchyGSR.ForeColor());
    }

    if (!Use24HourClock && hour > 12) {
        hour -= 12;
    }

    int minute = totalMinutes % 60;
    int hourTens = hour >= 10 ? hour / 10 : 0;
    int hourUnits = hour % 10;
    int minuteTens = minute >= 10 ? minute / 10 : 0;
    int minuteUnits = minute % 10;

    drawLargeDigit(11, 5, hourTens);
    drawLargeDigit(55, 5, hourUnits);
    drawLargeDigit(111, 5, minuteTens);
    drawLargeDigit(155, 5, minuteUnits);
}

void WatchyStarfield::drawDate()
{
    WatchyGSR::display.setFont(&Seven_Segment10pt7b);

    int16_t x1, y1;
    uint16_t textWidth, textHeight;

    int day = WatchTime.Local.Day;
    int dayOfWeek = WatchTime.Local.Wday;
    int month = WatchTime.Local.Month;
    int year = WatchTime.Local.Year + WatchyGSR::SRTC.getLocalYearOffset();

    String dayOfWeekName = dayStr(dayOfWeek + 1); // offset by 1 to correct for 0-indexing
    dayOfWeekName = dayOfWeekName.substring(0, dayOfWeekName.length() - 3);
    WatchyGSR::display.getTextBounds(dayOfWeekName, 5, 85, &x1, &y1, &textWidth, &textHeight);
    if (WatchTime.Local.Wday == 4)
    {
        textWidth -= 5;
    }
    WatchyGSR::display.setCursor(76 - textWidth, 86);
    WatchyGSR::display.println(dayOfWeekName);

    String monthName = monthShortStr(month + 1); // offset by 1 to correct for 0-indexing
    WatchyGSR::display.getTextBounds(monthName, 60, 110, &x1, &y1, &textWidth, &textHeight);
    WatchyGSR::display.setCursor(79 - textWidth, 110);
    WatchyGSR::display.println(monthName);

    int dayTens = day / 10;
    int dayUnits = day % 10;
    int yearThousands = year / 1000;
    year %= 1000;
    int yearHundreds = year / 100;
    year %= 100;
    int yearTens = year / 10;
    int yearUnits = year % 10;

    drawDigit(8, 95, dayTens);
    drawDigit(29, 95, dayUnits);
    drawDigit(8, 129, yearThousands);
    drawDigit(29, 129, yearHundreds);
    drawDigit(50, 129, yearTens);
    drawDigit(71, 129, yearUnits);
}

void WatchyStarfield::drawSteps()
{
    uint32_t stepCount = watchyGSR.CurrentStepCount();

    uint32_t stepBarLength = 61 * stepCount / 10000;
    if (stepBarLength > 61)
    {
        stepBarLength = 61;
    }

    WatchyGSR::display.fillRect(131, 148, stepBarLength, 9, watchyGSR.ForeColor());

    int stepThousands = stepCount / 10000;
    stepCount %= 10000;
    int stepThousandsUnits = stepCount / 1000;
    stepCount %= 1000;
    int stepHundreds = stepCount / 100;
    stepCount %= 100;
    int stepTens = stepCount / 10;
    int stepUnits = stepCount % 10;

    drawDigit(8, 165, stepThousands);
    drawDigit(29, 165, stepThousandsUnits);
    drawDigit(50, 165, stepHundreds);
    drawDigit(71, 165, stepTens);
    drawDigit(92, 165, stepUnits);
}

int mapBatteryLevel(float voltage, float minVoltage, float maxVoltage, int minLevel, int maxLevel)
{
    if (voltage <= minVoltage)
    {
        return minLevel;
    }
    else if (voltage >= maxVoltage)
    {
        return maxLevel;
    }
    else
    {
        return (int)((voltage - minVoltage) * (maxLevel - minLevel) / (maxVoltage - minVoltage) + minLevel);
    }
}

void WatchyStarfield::drawBattery()
{
    float VBAT = WatchyGSR::getBatteryVoltage();
    int maxWidth = 37;

#ifdef ARDUINO_ESP32S3_DEV
    float MaxVBAT = 3.9;
    float MinVBAT = 3.6;
#else
    float MaxVBAT = 4.1;
    float MinVBAT = 3.8;
#endif

    int batteryLevel = mapBatteryLevel(VBAT, MinVBAT, MaxVBAT, 0, maxWidth);

    WatchyGSR::display.fillRect(155, 169, batteryLevel, 15, watchyGSR.ForeColor());
}

void WatchyStarfield::drawField()
{
    WatchyGSR::display.drawBitmap(0, 0, field, 200, 200, watchyGSR.ForeColor());
}

void WatchyStarfield::drawMoon()
{
    moonData_t moon; // variable to receive the data

    int32_t day = WatchTime.Local.Day;
    int32_t month = WatchTime.Local.Month;
    int year = WatchTime.Local.Year + WatchyGSR::SRTC.getLocalYearOffset();
    double hour = WatchTime.Local.Hour + 0.1;

    moon = moonP.getPhase(year, month, day, hour);

    int angle = moon.angle;
    double percentLit = moon.percentLit;

    // Waxing: 0-180
    // Waning: 180-360
    // WatchyGSR::display.setCursor(100, 74);

    const unsigned char *waxingBitmaps[] = {luna1, luna12, luna11, luna10, luna9, luna8, luna7};
    const unsigned char *waningBitmaps[] = {luna1, luna2, luna3, luna4, luna5, luna6, luna7};

    const unsigned char **bitmaps = (angle <= 180) ? waxingBitmaps : waningBitmaps;

    int index;
    if (percentLit < 0.1)
        index = 0;
    else if (percentLit < 0.25)
        index = 1;
    else if (percentLit < 0.4)
        index = 2;
    else if (percentLit < 0.6)
        index = 3;
    else if (percentLit < 0.75)
        index = 4;
    else if (percentLit < 0.9)
        index = 5;
    else
        index = 6;

    WatchyGSR::display.drawBitmap(131, 74, bitmaps[index], 61, 61, watchyGSR.ForeColor());
}

void WatchyStarfield::drawSun()
{
    Dusk2Dawn location(LOC);
    int32_t day = WatchTime.Local.Day;
    int32_t month = WatchTime.Local.Month;
    int year = WatchTime.Local.Year + WatchyGSR::SRTC.getLocalYearOffset();
    int sunrise = location.sunrise(year, month, day, false);
    int sunset = location.sunset(year, month, day, false);

    long currentTimeInMinutes = WatchTime.Local.Hour * 60 + WatchTime.Local.Minute;
    int sunPosition = (currentTimeInMinutes - sunrise) * 60 / (sunset - sunrise);
    if (currentTimeInMinutes > sunset) {
        sunPosition = 60;
    } else if (currentTimeInMinutes < sunrise) {
        sunPosition = 0;
    }
    WatchyGSR::display.drawBitmap(110, 132 - sunPosition, arr, 3, 5, watchyGSR.ForeColor());

    int sunriseHour = sunrise / 60;
    int sunriseMinute = sunrise % 60;
    int sunsetHour = sunset / 60;
    int sunsetMinute = sunset % 60;
    
    bool Use24HourClock = IsTwentyFourHour();

    if (!Use24HourClock && sunriseHour > 12) {
        sunriseHour -= 12;
    }
    
    if (!Use24HourClock && sunsetHour > 12) {
        sunsetHour -= 12;
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
}

void WatchyStarfield::drawWeather()
{
    int temperature = watchyGSR.GetWeatherTemperature();
    // int weatherConditionCode = watchyGSR.GetWeatherID();

    WatchyGSR::display.setFont(&DSEG7_Classic_Regular_39);
    int16_t x1, y1;
    uint16_t w, h;
    WatchyGSR::display.getTextBounds(String(temperature), 0, 0, &x1, &y1, &w, &h);

    if (159 - w - x1 > 87)
    {
        WatchyGSR::display.setCursor(159 - w - x1, 150);
    }
    else
    {
        WatchyGSR::display.setFont(&DSEG7_Classic_Bold_25);
        WatchyGSR::display.getTextBounds(String(temperature), 0, 0, &x1, &y1, &w, &h);
        WatchyGSR::display.setCursor(159 - w - x1, 136);
    }

    WatchyGSR::display.println(temperature);
    WatchyGSR::display.drawBitmap(165, 110, watchyGSR.IsMetric() ? celsius : fahrenheit, 26, 20, watchyGSR.ForeColor());

    // InkWatchy uses open meteo
    /*
    const unsigned char *weatherIcon;

    // https://openweathermap.org/weather-conditions
    if (weatherConditionCode > 801)
        weatherIcon = cloudy; // Cloudy
    else if (weatherConditionCode == 801)
        weatherIcon = cloudsun; // Few Clouds
    else if (weatherConditionCode == 800)
        weatherIcon = sunny; // Clear
    else if (weatherConditionCode >= 700)
        weatherIcon = atmosphere; // Atmosphere
    else if (weatherConditionCode >= 600)
        weatherIcon = snow; // Snow
    else if (weatherConditionCode >= 500)
        weatherIcon = rain; // Rain
    else if (weatherConditionCode >= 300)
        weatherIcon = drizzle; // Drizzle
    else if (weatherConditionCode >= 200)
        weatherIcon = thunderstorm; // Thunderstorm
    else
        return;
    WatchyGSR::display.drawBitmap(145, 158, weatherIcon, 48, 32, watchyGSR.ForeColor());
    */
}
#endif