#include "slate.h"
#include "rtcMem.h"
#include "slate_localization.h"

#if WATCHFACE_SLATE

// Font configuration
#define SLATE_FONT "dogicapixel4"

// Layout constants - adjusted for 12h format
#define SLATE_BORDER_WIDTH 2
#define SLATE_TIME_Y 50
#define SLATE_DATE_Y 90
#define SLATE_BATTERY_Y 120
#define SLATE_TEMP_Y 150
#define SLATE_WEATHER_Y 180

#define SLATE_TIME_NO_WEATHER_Y 50
#define SLATE_DATE_NO_WEATHER_Y 110
#define SLATE_BATTERY_NO_WEATHER_Y 150

// 12-hour format AM/PM positioning
#if WATCHFACE_12H
#define SLATE_AMPM_X 150
#define SLATE_AMPM_Y 70
#define SLATE_AMPM_FONT "dogicapixel4"
#endif

// Check if weather is available (cached for battery optimization)
static bool weatherIsAvailable() {
#if WEATHER_INFO
    // Only check weather once per hour to save battery
    if (rM.slate.lastHourWeatherCheck != timeRTCLocal.Hour) {
        rM.slate.lastHourWeatherCheck = timeRTCLocal.Hour;
        OM_OneHourWeather wData = weatherGetDataHourly(WEATHER_WATCHFACE_HOUR_OFFSET);
        rM.slate.weatherAvailable = wData.fine;
    }
    return rM.slate.weatherAvailable;
#else
    return false;
#endif
}

static void drawTimeBeforeApply()
{
    // Use new 12-hour aware function
    String timeStr = getLocalizedTimeString(timeRTCLocal);
    
    bool hasWeather = weatherIsAvailable();
    int yPos = hasWeather ? SLATE_TIME_Y : SLATE_TIME_NO_WEATHER_Y;
    
    // Clear time area - size 4 font needs more space
    dis->fillRect(SLATE_BORDER_WIDTH + 2, yPos - 45, 196 - 2*SLATE_BORDER_WIDTH, 50, GxEPD_WHITE);
    
    setFont(getFont(SLATE_FONT));
    setTextSize(4);
    writeTextCenterReplaceBack(timeStr, yPos);
    
#if WATCHFACE_12H
    // Show AM/PM in 12-hour mode
    String ampmStr = getLocalizedAMPM(timeRTCLocal);
    setFont(getFont(SLATE_AMPM_FONT));
    setTextSize(1);
    dis->fillRect(SLATE_AMPM_X, SLATE_AMPM_Y - 10, 30, 20, GxEPD_WHITE);
    writeTextReplaceBack(ampmStr, SLATE_AMPM_X, SLATE_AMPM_Y);
#endif
}

static void drawTimeAfterApply(bool forceDraw)
{
    bool hasWeather = weatherIsAvailable();
    
    // Only redraw if day actually changed
    if (rM.slate.lastDay != timeRTCLocal.Day || forceDraw) {
        rM.slate.lastDay = timeRTCLocal.Day;
        
        setFont(getFont(SLATE_FONT));
        setTextSize(2);
        
        String day = String(timeRTCLocal.Day);
        if (day.length() == 1) day = "0" + day;
        
        String month = String(timeRTCLocal.Month + 1);
        if (month.length() == 1) month = "0" + month;
        
        String year = String((timeRTCLocal.Year + 70) % 100);
        if (year.length() == 1) year = "0" + year;
        
        String dayName = getLocalizedDayName(0);
        dayName.toUpperCase();
        
        String dateStr = day + "." + month + "." + year + " | " + dayName;
        
        int dateY = hasWeather ? SLATE_DATE_Y : SLATE_DATE_NO_WEATHER_Y;
        dis->fillRect(SLATE_BORDER_WIDTH + 2, dateY - 20, 196 - 2*SLATE_BORDER_WIDTH, 25, GxEPD_WHITE);
        writeTextCenterReplaceBack(dateStr, dateY);
    }
    
    // Battery - only redraw if level changed
    if (rM.slate.lastBatteryLevel != rM.batteryPercantageWF || forceDraw) {
        rM.slate.lastBatteryLevel = rM.batteryPercantageWF;
        
        String battBar = "[";
        int segments = (rM.batteryPercantageWF + 10) / 20;
        
        for(int i = 0; i < 5; i++) {
            battBar += (i < segments) ? "=" : " ";
        }
        battBar += "]";
        
        int batteryY = hasWeather ? SLATE_BATTERY_Y : SLATE_BATTERY_NO_WEATHER_Y;
        dis->fillRect(SLATE_BORDER_WIDTH + 2, batteryY - 20, 196 - 2*SLATE_BORDER_WIDTH, 25, GxEPD_WHITE);
        setFont(getFont(SLATE_FONT));
        setTextSize(2);
        writeTextCenterReplaceBack(battBar, batteryY);
    }
    
#if WEATHER_INFO
    // Weather - only update if actually available (checked max once per hour)
    if (hasWeather) {
        // Only fetch weather data if we need to update
        OM_OneHourWeather wData = weatherGetDataHourly(WEATHER_WATCHFACE_HOUR_OFFSET);
        
        // Temperature
        dis->fillRect(SLATE_BORDER_WIDTH + 2, SLATE_TEMP_Y - 30, 196 - 2*SLATE_BORDER_WIDTH, 35, GxEPD_WHITE);
        setFont(getFont(SLATE_FONT));
        setTextSize(3);
        int temp = (int)roundf(wData.temp);
        String tempStr = String(temp) + "C";
        writeTextCenterReplaceBack(tempStr, SLATE_TEMP_Y);
        
        // Weather condition
        dis->fillRect(SLATE_BORDER_WIDTH + 2, SLATE_WEATHER_Y - 20, 196 - 2*SLATE_BORDER_WIDTH, 25, GxEPD_WHITE);
        setTextSize(2);
        String condition = getLocalizedWeatherCondition(wData.weather_code);
        writeTextCenterReplaceBack(condition, SLATE_WEATHER_Y);
    } else if (forceDraw) {
        // Clear weather areas on force draw
        dis->fillRect(SLATE_BORDER_WIDTH + 2, SLATE_TEMP_Y - 30, 196 - 2*SLATE_BORDER_WIDTH, 35, GxEPD_WHITE);
        dis->fillRect(SLATE_BORDER_WIDTH + 2, SLATE_WEATHER_Y - 20, 196 - 2*SLATE_BORDER_WIDTH, 25, GxEPD_WHITE);
    }
#else
    if (forceDraw) {
        // Clear weather areas since no weather support
        dis->fillRect(SLATE_BORDER_WIDTH + 2, SLATE_TEMP_Y - 30, 196 - 2*SLATE_BORDER_WIDTH, 35, GxEPD_WHITE);
        dis->fillRect(SLATE_BORDER_WIDTH + 2, SLATE_WEATHER_Y - 20, 196 - 2*SLATE_BORDER_WIDTH, 25, GxEPD_WHITE);
    }
#endif
    
    // Always redraw border to ensure it's intact
    dis->drawRect(0, 0, 200, 200, GxEPD_BLACK);
    dis->drawRect(1, 1, 198, 198, GxEPD_BLACK);
}

static void showTimeFull()
{
    // Use new 12-hour aware function
    String timeStr = getLocalizedTimeString(timeRTCLocal);
    
    bool hasWeather = weatherIsAvailable();
    int yPos = hasWeather ? SLATE_TIME_Y : SLATE_TIME_NO_WEATHER_Y;
    
    // Clear time area - size 4 font needs more space
    dis->fillRect(SLATE_BORDER_WIDTH + 2, yPos - 45, 196 - 2*SLATE_BORDER_WIDTH, 50, GxEPD_WHITE);
    
    setFont(getFont(SLATE_FONT));
    setTextSize(4);
    writeTextCenterReplaceBack(timeStr, yPos);
    
#if WATCHFACE_12H
    // Show AM/PM in 12-hour mode
    String ampmStr = getLocalizedAMPM(timeRTCLocal);
    setFont(getFont(SLATE_AMPM_FONT));
    setTextSize(1);
    dis->fillRect(SLATE_AMPM_X, SLATE_AMPM_Y - 10, 30, 20, GxEPD_WHITE);
    writeTextReplaceBack(ampmStr, SLATE_AMPM_X, SLATE_AMPM_Y);
#endif
}

static void initWatchface()
{
    dis->fillScreen(GxEPD_WHITE);
    readRTC();
    
    // Reset cache on init
    rM.slate.lastHourWeatherCheck = 255;
    rM.slate.lastBatteryLevel = 255;
    rM.slate.lastDay = 255;
    rM.slate.lastMonth = 255;
    
    // Draw border
    dis->drawRect(0, 0, 200, 200, GxEPD_BLACK);
    dis->drawRect(1, 1, 198, 198, GxEPD_BLACK);
    
    bool hasWeather = weatherIsAvailable();
    
    showTimeFull();
    
    // Initial draw of all elements
    setFont(getFont(SLATE_FONT));
    setTextSize(2);
    
    // Date
    String day = String(timeRTCLocal.Day);
    if (day.length() == 1) day = "0" + day;
    
    String month = String(timeRTCLocal.Month + 1);
    if (month.length() == 1) month = "0" + month;
    
    String year = String((timeRTCLocal.Year + 70) % 100);
    if (year.length() == 1) year = "0" + year;
    
    String dayName = getLocalizedDayName(0);
    dayName.toUpperCase();
    
    String dateStr = day + "." + month + "." + year + " | " + dayName;
    
    int dateY = hasWeather ? SLATE_DATE_Y : SLATE_DATE_NO_WEATHER_Y;
    writeTextCenterReplaceBack(dateStr, dateY);
    rM.slate.lastDay = timeRTCLocal.Day;
    rM.slate.lastMonth = timeRTCLocal.Month;
    
    // Battery
    String battBar = "[";
    int segments = (rM.batteryPercantageWF + 10) / 20;
    
    for(int i = 0; i < 5; i++) {
        battBar += (i < segments) ? "=" : " ";
    }
    battBar += "]";
    
    int batteryY = hasWeather ? SLATE_BATTERY_Y : SLATE_BATTERY_NO_WEATHER_Y;
    writeTextCenterReplaceBack(battBar, batteryY);
    rM.slate.lastBatteryLevel = rM.batteryPercantageWF;
    
#if WEATHER_INFO
    if (hasWeather) {
        OM_OneHourWeather wData = weatherGetDataHourly(WEATHER_WATCHFACE_HOUR_OFFSET);
        
        // Temperature
        setTextSize(3);
        int temp = (int)roundf(wData.temp);
        String tempStr = String(temp) + "C";
        writeTextCenterReplaceBack(tempStr, SLATE_TEMP_Y);
        
        // Weather condition
        setTextSize(2);
        String condition = getLocalizedWeatherCondition(wData.weather_code);
        writeTextCenterReplaceBack(condition, SLATE_WEATHER_Y);
    }
#endif
}

static void drawDay()
{
    // Cache check prevents unnecessary redraws
    if (rM.slate.lastDay == timeRTCLocal.Day) return;
    
    bool hasWeather = weatherIsAvailable();
    
    setFont(getFont(SLATE_FONT));
    setTextSize(2);
    
    String day = String(timeRTCLocal.Day);
    if (day.length() == 1) day = "0" + day;
    
    String month = String(timeRTCLocal.Month + 1);
    if (month.length() == 1) month = "0" + month;
    
    String year = String((timeRTCLocal.Year + 70) % 100);
    if (year.length() == 1) year = "0" + year;
    
    String dayName = getLocalizedDayName(0);
    dayName.toUpperCase();
    
    String dateStr = day + "." + month + "." + year + " | " + dayName;
    
    int dateY = hasWeather ? SLATE_DATE_Y : SLATE_DATE_NO_WEATHER_Y;
    dis->fillRect(SLATE_BORDER_WIDTH + 2, dateY - 20, 196 - 2*SLATE_BORDER_WIDTH, 25, GxEPD_WHITE);
    writeTextCenterReplaceBack(dateStr, dateY);
    
    rM.slate.lastDay = timeRTCLocal.Day;
}

static void drawMonth()
{
    // Cache check prevents unnecessary redraws
    if (rM.slate.lastMonth == timeRTCLocal.Month) return;
    
    bool hasWeather = weatherIsAvailable();
    
    setFont(getFont(SLATE_FONT));
    setTextSize(2);
    
    String day = String(timeRTCLocal.Day);
    if (day.length() == 1) day = "0" + day;
    
    String month = String(timeRTCLocal.Month + 1);
    if (month.length() == 1) month = "0" + month;
    
    String year = String((timeRTCLocal.Year + 70) % 100);
    if (year.length() == 1) year = "0" + year;
    
    String dayName = getLocalizedDayName(0);
    dayName.toUpperCase();
    
    String dateStr = day + "." + month + "." + year + " | " + dayName;
    
    int dateY = hasWeather ? SLATE_DATE_Y : SLATE_DATE_NO_WEATHER_Y;
    dis->fillRect(SLATE_BORDER_WIDTH + 2, dateY - 30, 196 - 2*SLATE_BORDER_WIDTH, 30, GxEPD_WHITE);
    writeTextCenterReplaceBack(dateStr, dateY);
    
    rM.slate.lastMonth = timeRTCLocal.Month;
}

static void drawBattery()
{
    // Cache check prevents unnecessary redraws
    if (rM.slate.lastBatteryLevel == rM.batteryPercantageWF) return;
    
    bool hasWeather = weatherIsAvailable();
    
    setFont(getFont(SLATE_FONT));
    setTextSize(2);
    
    String battBar = "[";
    int segments = (rM.batteryPercantageWF + 10) / 20;
    
    for(int i = 0; i < 5; i++) {
        battBar += (i < segments) ? "=" : " ";
    }
    battBar += "]";
    
    int batteryY = hasWeather ? SLATE_BATTERY_Y : SLATE_BATTERY_NO_WEATHER_Y;
    dis->fillRect(SLATE_BORDER_WIDTH + 2, batteryY - 30, 196 - 2*SLATE_BORDER_WIDTH, 30, GxEPD_WHITE);
    writeTextCenterReplaceBack(battBar, batteryY);
    
    rM.slate.lastBatteryLevel = rM.batteryPercantageWF;
}

const watchfaceDefOne slateDef = {
    .drawTimeBeforeApply = drawTimeBeforeApply,
    .drawTimeAfterApply = drawTimeAfterApply,
    .drawDay = drawDay,
    .drawMonth = drawMonth,
    .showTimeFull = showTimeFull,
    .initWatchface = initWatchface,
    .drawBattery = drawBattery,
    .manageInput = [](buttonState bt)
    {
        switch (bt) {
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
    .isModuleEngaged = []() { return false; }
};

#endif