#include "slate.h"
#include "rtcMem.h"
#include "slate_localization.h"
#include "localization.h"  // For formatTemperature and getLocalizedDayByIndex

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

// Helper function to get safe clear bounds for text
static void getSafeClearBounds(String text, int textSize, int fontHeight, uint16_t* width, uint16_t* height) {
    setTextSize(textSize);
    setFont(getFont(SLATE_FONT));
    
    uint16_t textWidth, textHeight;
    getTextBounds(text, NULL, NULL, &textWidth, &textHeight);
    
    // Add safety margins - 4 pixels each side for width, 2 pixels top/bottom for height
    *width = textWidth + 8;
    *height = max(textHeight + 4, fontHeight * textSize + 4);
}

// Helper function to clear text area safely
static void clearTextArea(int x, int y, String text, int textSize, int fontHeight) {
    uint16_t clearWidth, clearHeight;
    getSafeClearBounds(text, textSize, fontHeight, &clearWidth, &clearHeight);
    
    // Make sure we don't go outside screen bounds
    int clearX = max(SLATE_BORDER_WIDTH + 2, x - 4);
    int maxClearWidth = 196 - 2*SLATE_BORDER_WIDTH - (clearX - SLATE_BORDER_WIDTH - 2);
    clearWidth = min((int)clearWidth, maxClearWidth);
    
    dis->fillRect(clearX, y - clearHeight + 2, clearWidth, clearHeight, GxEPD_WHITE);
}

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
    // Use localization functions
    String oldTimeStr = getLocalizedTimeString(rM.wFTime);
    String newTimeStr = getLocalizedTimeString(timeRTCLocal);
    
    bool hasWeather = weatherIsAvailable();
    int yPos = hasWeather ? SLATE_TIME_Y : SLATE_TIME_NO_WEATHER_Y;
    bool needsBorderRedraw = false;
    
    // Only clear and redraw if time actually changed
    if (oldTimeStr != newTimeStr) {
        clearTextArea(100, yPos, oldTimeStr, 4, 6);  // Clear old time
        needsBorderRedraw = true;
        
        setFont(getFont(SLATE_FONT));
        setTextSize(4);
        writeTextCenterReplaceBack(newTimeStr, yPos);
    }
    
#if WATCHFACE_12H
    // Show AM/PM in 12-hour mode - only update if hour changed
    if (rM.wFTime.Hour != timeRTCLocal.Hour) {
        String oldAmPm = getLocalizedAMPM(rM.wFTime);
        String newAmPm = getLocalizedAMPM(timeRTCLocal);
        
        if (oldAmPm != newAmPm) {
            clearTextArea(SLATE_AMPM_X, SLATE_AMPM_Y, oldAmPm, 1, 6);  // Clear old AM/PM
            needsBorderRedraw = true;
            
            setFont(getFont(SLATE_AMPM_FONT));
            setTextSize(1);
            writeTextReplaceBack(newAmPm, SLATE_AMPM_X, SLATE_AMPM_Y);
        }
    }
#endif

    // Redraw border if any clearing happened
    if (needsBorderRedraw) {
        dis->drawRect(0, 0, 200, 200, GxEPD_BLACK);
        dis->drawRect(1, 1, 198, 198, GxEPD_BLACK);
    }
}

static void drawTimeAfterApply(bool forceDraw)
{
    bool hasWeather = weatherIsAvailable();
    bool needsBorderRedraw = false;
    
    // Only redraw if day actually changed
    if (rM.slate.lastDay != timeRTCLocal.Day || forceDraw) {
        rM.slate.lastDay = timeRTCLocal.Day;
        
        String day = String(timeRTCLocal.Day);
        if (day.length() == 1) day = "0" + day;
        
        String month = String(timeRTCLocal.Month + 1);
        if (month.length() == 1) month = "0" + month;
        
        String year = String((timeRTCLocal.Year + 70) % 100);
        if (year.length() == 1) year = "0" + year;
        
        // Use localization.h function for day names
        String dayName = getLocalizedDayByIndex(timeRTCLocal.Wday, 0);
        dayName.toUpperCase();
        
        String dateStr = day + "." + month + "." + year + " | " + dayName;
        
        int dateY = hasWeather ? SLATE_DATE_Y : SLATE_DATE_NO_WEATHER_Y;
        
        // Clear previous date with exact bounds
        if (!forceDraw) {
            String oldDay = String(rM.slate.lastDay);
            if (oldDay.length() == 1) oldDay = "0" + oldDay;
            String oldMonth = String(rM.slate.lastMonth + 1);
            if (oldMonth.length() == 1) oldMonth = "0" + oldMonth;
            String oldYear = String((rM.slate.lastMonth + 70) % 100);
            if (oldYear.length() == 1) oldYear = "0" + oldYear;
            String oldDayName = getLocalizedDayByIndex(timeRTCLocal.Wday, -1);
            oldDayName.toUpperCase();
            String oldDateStr = oldDay + "." + oldMonth + "." + oldYear + " | " + oldDayName;
            clearTextArea(100, dateY, oldDateStr, 2, 6);
            needsBorderRedraw = true;
        }
        
        setFont(getFont(SLATE_FONT));
        setTextSize(2);
        writeTextCenterReplaceBack(dateStr, dateY);
    }
    
    // Battery - only redraw if level changed
    if (rM.slate.lastBatteryLevel != rM.batteryPercantageWF || forceDraw) {
        String oldBattBar = "[";
        int oldSegments = (rM.slate.lastBatteryLevel + 10) / 20;
        for(int i = 0; i < 5; i++) {
            oldBattBar += (i < oldSegments) ? "=" : " ";
        }
        oldBattBar += "]";
        
        String newBattBar = "[";
        int newSegments = (rM.batteryPercantageWF + 10) / 20;
        for(int i = 0; i < 5; i++) {
            newBattBar += (i < newSegments) ? "=" : " ";
        }
        newBattBar += "]";
        
        int batteryY = hasWeather ? SLATE_BATTERY_Y : SLATE_BATTERY_NO_WEATHER_Y;
        
        // Clear old battery display
        if (!forceDraw) {
            clearTextArea(100, batteryY, oldBattBar, 2, 6);
            needsBorderRedraw = true;
        }
        
        setFont(getFont(SLATE_FONT));
        setTextSize(2);
        writeTextCenterReplaceBack(newBattBar, batteryY);
        
        rM.slate.lastBatteryLevel = rM.batteryPercantageWF;
    }
    
#if WEATHER_INFO
    // Weather - only update if actually available (checked max once per hour)
    if (hasWeather) {
        // Only fetch weather data if we need to update
        OM_OneHourWeather wData = weatherGetDataHourly(WEATHER_WATCHFACE_HOUR_OFFSET);
        
        // Temperature - using global formatTemperature function
        String tempStr = formatTemperature(wData.temp);
        if (strcmp(rM.slate.lastTemp, tempStr.c_str()) != 0 || forceDraw) {
            if (!forceDraw && strlen(rM.slate.lastTemp) > 0) {
                clearTextArea(100, SLATE_TEMP_Y, String(rM.slate.lastTemp), 3, 6);
                needsBorderRedraw = true;
            }
            
            setFont(getFont(SLATE_FONT));
            setTextSize(3);
            writeTextCenterReplaceBack(tempStr, SLATE_TEMP_Y);
            strncpy(rM.slate.lastTemp, tempStr.c_str(), sizeof(rM.slate.lastTemp) - 1);
            rM.slate.lastTemp[sizeof(rM.slate.lastTemp) - 1] = '\0';
        }
        
        // Weather condition
        String condition = getLocalizedWeatherCondition(wData.weather_code);
        if (strcmp(rM.slate.lastCondition, condition.c_str()) != 0 || forceDraw) {
            if (!forceDraw && strlen(rM.slate.lastCondition) > 0) {
                clearTextArea(100, SLATE_WEATHER_Y, String(rM.slate.lastCondition), 2, 6);
                needsBorderRedraw = true;
            }
            
            setFont(getFont(SLATE_FONT));
            setTextSize(2);
            writeTextCenterReplaceBack(condition, SLATE_WEATHER_Y);
            strncpy(rM.slate.lastCondition, condition.c_str(), sizeof(rM.slate.lastCondition) - 1);
            rM.slate.lastCondition[sizeof(rM.slate.lastCondition) - 1] = '\0';
        }
    } else if (forceDraw) {
        // Clear weather areas on force draw
        clearTextArea(100, SLATE_TEMP_Y, "000°C", 3, 6);
        clearTextArea(100, SLATE_WEATHER_Y, "NO WEATHER", 2, 6);
        needsBorderRedraw = true;
    }
#else
    if (forceDraw) {
        // Clear weather areas since no weather support
        clearTextArea(100, SLATE_TEMP_Y, "000°C", 3, 6);
        clearTextArea(100, SLATE_WEATHER_Y, "NO WEATHER", 2, 6);
        needsBorderRedraw = true;
    }
#endif
    
    // Always redraw border if any clearing happened or if forced
    if (needsBorderRedraw || forceDraw) {
        dis->drawRect(0, 0, 200, 200, GxEPD_BLACK);
        dis->drawRect(1, 1, 198, 198, GxEPD_BLACK);
    }
}

static void showTimeFull()
{
    // Use localization function
    String timeStr = getLocalizedTimeString(timeRTCLocal);
    
    bool hasWeather = weatherIsAvailable();
    int yPos = hasWeather ? SLATE_TIME_Y : SLATE_TIME_NO_WEATHER_Y;
    
    // Clear time area with exact bounds
    clearTextArea(100, yPos, timeStr, 4, 6);
    
    setFont(getFont(SLATE_FONT));
    setTextSize(4);
    writeTextCenterReplaceBack(timeStr, yPos);
    
#if WATCHFACE_12H
    // Show AM/PM in 12-hour mode
    String ampmStr = getLocalizedAMPM(timeRTCLocal);
    clearTextArea(SLATE_AMPM_X, SLATE_AMPM_Y, ampmStr, 1, 6);
    setFont(getFont(SLATE_AMPM_FONT));
    setTextSize(1);
    writeTextReplaceBack(ampmStr, SLATE_AMPM_X, SLATE_AMPM_Y);
#endif

    // Redraw border after clearing
    dis->drawRect(0, 0, 200, 200, GxEPD_BLACK);
    dis->drawRect(1, 1, 198, 198, GxEPD_BLACK);
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
    strcpy(rM.slate.lastTemp, "");     // Initialize empty
    strcpy(rM.slate.lastCondition, ""); // Initialize empty
    
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
    
    // Use localization.h function for day names
    String dayName = getLocalizedDayByIndex(timeRTCLocal.Wday, 0);
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
        
        // Temperature - using global formatTemperature function
        setTextSize(3);
        String tempStr = formatTemperature(wData.temp);
        writeTextCenterReplaceBack(tempStr, SLATE_TEMP_Y);
        strncpy(rM.slate.lastTemp, tempStr.c_str(), sizeof(rM.slate.lastTemp) - 1);
        rM.slate.lastTemp[sizeof(rM.slate.lastTemp) - 1] = '\0';
        
        // Weather condition
        setTextSize(2);
        String condition = getLocalizedWeatherCondition(wData.weather_code);
        writeTextCenterReplaceBack(condition, SLATE_WEATHER_Y);
        strncpy(rM.slate.lastCondition, condition.c_str(), sizeof(rM.slate.lastCondition) - 1);
        rM.slate.lastCondition[sizeof(rM.slate.lastCondition) - 1] = '\0';
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
    
    // Use localization.h function for day names
    String dayName = getLocalizedDayByIndex(timeRTCLocal.Wday, 0);
    dayName.toUpperCase();
    
    String dateStr = day + "." + month + "." + year + " | " + dayName;
    
    int dateY = hasWeather ? SLATE_DATE_Y : SLATE_DATE_NO_WEATHER_Y;
    clearTextArea(100, dateY, dateStr, 2, 6);
    writeTextCenterReplaceBack(dateStr, dateY);
    
    rM.slate.lastDay = timeRTCLocal.Day;
    
    // Redraw border after clearing
    dis->drawRect(0, 0, 200, 200, GxEPD_BLACK);
    dis->drawRect(1, 1, 198, 198, GxEPD_BLACK);
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
    
    // Use localization.h function for day names
    String dayName = getLocalizedDayByIndex(timeRTCLocal.Wday, 0);
    dayName.toUpperCase();
    
    String dateStr = day + "." + month + "." + year + " | " + dayName;
    
    int dateY = hasWeather ? SLATE_DATE_Y : SLATE_DATE_NO_WEATHER_Y;
    clearTextArea(100, dateY, dateStr, 2, 6);
    writeTextCenterReplaceBack(dateStr, dateY);
    
    rM.slate.lastMonth = timeRTCLocal.Month;
    
    // Redraw border after clearing
    dis->drawRect(0, 0, 200, 200, GxEPD_BLACK);
    dis->drawRect(1, 1, 198, 198, GxEPD_BLACK);
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
    clearTextArea(100, batteryY, battBar, 2, 6);
    writeTextCenterReplaceBack(battBar, batteryY);
    
    rM.slate.lastBatteryLevel = rM.batteryPercantageWF;
    
    // Redraw border after clearing
    dis->drawRect(0, 0, 200, 200, GxEPD_BLACK);
    dis->drawRect(1, 1, 198, 198, GxEPD_BLACK);
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