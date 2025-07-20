#include "slate.h"
#include "rtcMem.h"
#include "slate_localization.h"

#if WATCHFACE_SLATE

// Font configuration
#define SLATE_FONT "dogicapixel4"
#define SLATE_BORDER_WIDTH 2

// Layout constants - with weather
#define SLATE_TIME_Y 50
#define SLATE_DATE_Y 90
#define SLATE_BATTERY_Y 120
#define SLATE_TEMP_Y 150
#define SLATE_WEATHER_Y 180

// Layout constants - without weather
#define SLATE_TIME_NO_WEATHER_Y 60
#define SLATE_DATE_NO_WEATHER_Y 110
#define SLATE_BATTERY_NO_WEATHER_Y 150

// 12-hour format AM/PM positioning
#if WATCHFACE_12H
#define SLATE_AMPM_X 150
#define SLATE_AMPM_Y 70
#define SLATE_AMPM_FONT "dogicapixel4"
#endif

// Forward declarations
static void drawTimeAfterApply(bool forceDraw);

// Global state for border redraw
static bool needsBorderRedraw = false;

// Helper to schedule border redraw
static void scheduleBorderRedraw() {
    needsBorderRedraw = true;
}

// Helper to redraw border if needed
static void redrawBorderIfNeeded() {
    if (needsBorderRedraw) {
        dis->drawRect(0, 0, 200, 200, GxEPD_BLACK);
        dis->drawRect(1, 1, 198, 198, GxEPD_BLACK);
        needsBorderRedraw = false;
    }
}

// Check if weather is available
static bool isWeatherAvailable() {
#if WEATHER_INFO
    // Only check once per hour to save battery
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

// Get Y positions based on weather availability
static int getTimeY() { 
    return isWeatherAvailable() ? SLATE_TIME_Y : SLATE_TIME_NO_WEATHER_Y; 
}

static int getDateY() { 
    return isWeatherAvailable() ? SLATE_DATE_Y : SLATE_DATE_NO_WEATHER_Y; 
}

static int getBatteryY() { 
    return isWeatherAvailable() ? SLATE_BATTERY_Y : SLATE_BATTERY_NO_WEATHER_Y; 
}

// Format battery string with new formula
static String formatBatteryBar(uint8_t percentage) {
    String battBar = "[";
    
    // New formula: each segment = exactly 20%
    int segments;
    if (percentage == 0) {
        segments = 0;  // Show empty battery only at exactly 0%
    } else {
        segments = min(5, (percentage + 19) / 20);
    }
    
    for (int i = 0; i < 5; i++) {
        battBar += (i < segments) ? "=" : " ";
    }
    battBar += "]";
    return battBar;
}

// Get text dimensions
static void getTextDimensions(String text, int textSize, uint16_t *width, uint16_t *height) {
    setTextSize(textSize);
    setFont(getFont(SLATE_FONT));
    
    uint16_t textWidth, textHeight;
    getTextBounds(text, NULL, NULL, &textWidth, &textHeight);
    
    *width = textWidth;
    *height = textHeight;
}

// Clear centered text area
static void clearCenteredText(int y, int textSize, int estimatedWidth, int height) {
    int clearX = (200 - estimatedWidth) / 2 - 4;
    int clearWidth = estimatedWidth + 8;
    
    // Ensure we stay within screen bounds
    clearX = max(SLATE_BORDER_WIDTH + 1, clearX);
    int maxX = 198 - SLATE_BORDER_WIDTH;
    if (clearX + clearWidth > maxX) {
        clearWidth = maxX - clearX;
    }
    
    dis->fillRect(clearX, y - height + 2, clearWidth, height, GxEPD_WHITE);
    scheduleBorderRedraw();
}

static void drawTimeBeforeApply() {
    String oldTimeStr = getLocalizedTimeString(rM.wFTime);
    String newTimeStr = getLocalizedTimeString(timeRTCLocal);
    
    if (oldTimeStr != newTimeStr) {
        uint16_t w, h;
        getTextDimensions(oldTimeStr, 4, &w, &h);
        clearCenteredText(getTimeY(), 4, w, h);
        
        setFont(getFont(SLATE_FONT));
        setTextSize(4);
        writeTextCenterReplaceBack(newTimeStr, getTimeY());
    }

#if WATCHFACE_12H
    if (rM.wFTime.Hour != timeRTCLocal.Hour) {
        String oldAmPm = getLocalizedAMPM(rM.wFTime);
        String newAmPm = getLocalizedAMPM(timeRTCLocal);
        
        if (oldAmPm != newAmPm) {
            uint16_t w, h;
            getTextDimensions(oldAmPm, 1, &w, &h);
            dis->fillRect(SLATE_AMPM_X - 2, SLATE_AMPM_Y - h, w + 4, h + 2, GxEPD_WHITE);
            scheduleBorderRedraw();
            
            setFont(getFont(SLATE_AMPM_FONT));
            setTextSize(1);
            writeTextReplaceBack(newAmPm, SLATE_AMPM_X, SLATE_AMPM_Y);
        }
    }
#endif
}

static void drawTimeAfterApply(bool forceDraw) {
    // Date
    if (rM.slate.lastDay != timeRTCLocal.Day || forceDraw) {
        rM.slate.lastDay = timeRTCLocal.Day;
        
        String day = String(timeRTCLocal.Day);
        if (day.length() == 1) day = "0" + day;
        
        String month = String(timeRTCLocal.Month + 1);
        if (month.length() == 1) month = "0" + month;
        
        String year = String((timeRTCLocal.Year + 70) % 100);
        if (year.length() == 1) year = "0" + year;
        
        String dayName = getLocalizedDayByIndex(timeRTCLocal.Wday, 0);
        dayName.toUpperCase();
        
        String dateStr = day + "." + month + "." + year + " | " + dayName;
        
        if (!forceDraw) {
            uint16_t w, h;
            getTextDimensions(dateStr, 2, &w, &h);
            clearCenteredText(getDateY(), 2, w, h);
        }
        
        setFont(getFont(SLATE_FONT));
        setTextSize(2);
        writeTextCenterReplaceBack(dateStr, getDateY());
    }
    
    // Battery - ALWAYS draw on force draw or if changed
    if (rM.slate.lastBatteryLevel != rM.batteryPercantageWF || forceDraw) {
        String battBar = formatBatteryBar(rM.batteryPercantageWF);
        
        if (!forceDraw && rM.slate.lastBatteryLevel != 255) {
            uint16_t w, h;
            getTextDimensions(battBar, 2, &w, &h);
            clearCenteredText(getBatteryY(), 2, w, h);
        }
        
        setFont(getFont(SLATE_FONT));
        setTextSize(2);
        writeTextCenterReplaceBack(battBar, getBatteryY());
        
        rM.slate.lastBatteryLevel = rM.batteryPercantageWF;
    }

#if WEATHER_INFO
    if (isWeatherAvailable()) {
        OM_OneHourWeather wData = weatherGetDataHourly(WEATHER_WATCHFACE_HOUR_OFFSET);
        
        // Temperature
        String tempStr = formatTemperature(wData.temp);
        if (strcmp(rM.slate.lastTemp, tempStr.c_str()) != 0 || forceDraw) {
            if (!forceDraw && strlen(rM.slate.lastTemp) > 0) {
                uint16_t w, h;
                getTextDimensions(String(rM.slate.lastTemp), 3, &w, &h);
                clearCenteredText(SLATE_TEMP_Y, 3, w, h);
            }
            
            setFont(getFont(SLATE_FONT));
            setTextSize(3);
            writeTextCenterReplaceBack(tempStr, SLATE_TEMP_Y);
            strncpy(rM.slate.lastTemp, tempStr.c_str(), sizeof(rM.slate.lastTemp) - 1);
        }
        
        // Weather condition
        String condition = getLocalizedWeatherCondition(wData.weather_code);
        if (strcmp(rM.slate.lastCondition, condition.c_str()) != 0 || forceDraw) {
            if (!forceDraw && strlen(rM.slate.lastCondition) > 0) {
                uint16_t w, h;
                getTextDimensions(String(rM.slate.lastCondition), 2, &w, &h);
                clearCenteredText(SLATE_WEATHER_Y, 2, w, h);
            }
            
            setFont(getFont(SLATE_FONT));
            setTextSize(2);
            writeTextCenterReplaceBack(condition, SLATE_WEATHER_Y);
            strncpy(rM.slate.lastCondition, condition.c_str(), sizeof(rM.slate.lastCondition) - 1);
        }
    }
#endif
    
    redrawBorderIfNeeded();
}

static void showTimeFull() {
    String timeStr = getLocalizedTimeString(timeRTCLocal);
    
    uint16_t w, h;
    getTextDimensions(timeStr, 4, &w, &h);
    clearCenteredText(getTimeY(), 4, w, h);
    
    setFont(getFont(SLATE_FONT));
    setTextSize(4);
    writeTextCenterReplaceBack(timeStr, getTimeY());

#if WATCHFACE_12H
    String ampmStr = getLocalizedAMPM(timeRTCLocal);
    getTextDimensions(ampmStr, 1, &w, &h);
    dis->fillRect(SLATE_AMPM_X - 2, SLATE_AMPM_Y - h, w + 4, h + 2, GxEPD_WHITE);
    scheduleBorderRedraw();
    
    setFont(getFont(SLATE_AMPM_FONT));
    setTextSize(1);
    writeTextReplaceBack(ampmStr, SLATE_AMPM_X, SLATE_AMPM_Y);
#endif
    
    redrawBorderIfNeeded();
}

static void initWatchface() {
    dis->fillScreen(GxEPD_WHITE);
    readRTC();
    
    // Reset all cache
    rM.slate.lastHourWeatherCheck = 255;
    rM.slate.lastBatteryLevel = 255;
    rM.slate.lastDay = 255;
    rM.slate.lastMonth = 255;
    strcpy(rM.slate.lastTemp, "");
    strcpy(rM.slate.lastCondition, "");
    needsBorderRedraw = false;
    
    // Draw border first
    dis->drawRect(0, 0, 200, 200, GxEPD_BLACK);
    dis->drawRect(1, 1, 198, 198, GxEPD_BLACK);
    
    // Draw time
    setFont(getFont(SLATE_FONT));
    setTextSize(4);
    String timeStr = getLocalizedTimeString(timeRTCLocal);
    writeTextCenterReplaceBack(timeStr, getTimeY());
    
#if WATCHFACE_12H
    setFont(getFont(SLATE_AMPM_FONT));
    setTextSize(1);
    String ampmStr = getLocalizedAMPM(timeRTCLocal);
    writeTextReplaceBack(ampmStr, SLATE_AMPM_X, SLATE_AMPM_Y);
#endif
    
    // Draw all other elements
    drawTimeAfterApply(true);
}

static void drawDay() {
    if (rM.slate.lastDay == timeRTCLocal.Day) return;
    drawTimeAfterApply(false);
}

static void drawMonth() {
    if (rM.slate.lastMonth == timeRTCLocal.Month) return;
    rM.slate.lastMonth = timeRTCLocal.Month;
    drawTimeAfterApply(false);
}

static void drawBattery() {
    if (rM.slate.lastBatteryLevel == rM.batteryPercantageWF) return;
    
    String battBar = formatBatteryBar(rM.batteryPercantageWF);
    
    uint16_t w, h;
    getTextDimensions(battBar, 2, &w, &h);
    clearCenteredText(getBatteryY(), 2, w, h);
    
    setFont(getFont(SLATE_FONT));
    setTextSize(2);
    writeTextCenterReplaceBack(battBar, getBatteryY());
    
    rM.slate.lastBatteryLevel = rM.batteryPercantageWF;
    redrawBorderIfNeeded();
}

const watchfaceDefOne slateDef = {
    .drawTimeBeforeApply = drawTimeBeforeApply,
    .drawTimeAfterApply = drawTimeAfterApply,
    .drawDay = drawDay,
    .drawMonth = drawMonth,
    .showTimeFull = showTimeFull,
    .initWatchface = initWatchface,
    .drawBattery = drawBattery,
    .manageInput = [](buttonState bt) {
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