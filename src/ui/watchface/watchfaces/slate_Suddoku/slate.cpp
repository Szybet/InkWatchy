#include "slate.h"
#include "rtcMem.h"
#include <esp_sleep.h>

#if WATCHFACE_SLATE

// Font configuration
#define SLATE_FONT "dogicapixel4"

// Layout constants
#define SLATE_BORDER_WIDTH 2
#define SLATE_TIME_Y 50
#define SLATE_DATE_Y 90
#define SLATE_BATTERY_Y 120
#define SLATE_TEMP_Y 150
#define SLATE_WEATHER_Y 180

#define SLATE_TIME_NO_WEATHER_Y 50
#define SLATE_DATE_NO_WEATHER_Y 110
#define SLATE_BATTERY_NO_WEATHER_Y 150

#define SLATE_CLEAR_WIDTH 186

// RTC data for caching
RTC_DATA_ATTR static struct {
    bool weatherAvailable = false;
    uint8_t lastHourWeatherCheck = 255;  // Force initial check
    uint8_t lastBatteryLevel = 255;      // Force initial draw
    uint8_t lastDay = 255;               // Force initial draw
    uint8_t lastMonth = 255;             // Force initial draw
} slateCache;

// Check if weather is available (cached for battery optimization)
bool weatherIsAvailable() {
#if WEATHER_INFO
    // Only check weather once per hour to save battery
    if (slateCache.lastHourWeatherCheck != timeRTCLocal.Hour) {
        slateCache.lastHourWeatherCheck = timeRTCLocal.Hour;
        OM_OneHourWeather wData = weatherGetDataHourly(WEATHER_WATCHFACE_HOUR_OFFSET);
        slateCache.weatherAvailable = wData.fine;
    }
    return slateCache.weatherAvailable;
#else
    return false;
#endif
}

void showTimeFull()
{
    String timeStr = getHourMinute(timeRTCLocal);
    
    setFont(getFont(SLATE_FONT));
    setTextSize(4);
    
    dis->fillRect(SLATE_BORDER_WIDTH + 5, 10, SLATE_CLEAR_WIDTH, 55, GxEPD_WHITE);
    
    uint16_t w, h;
    getTextBounds(timeStr, NULL, NULL, &w, &h);
    int centerX = (200 - w) / 2;
    
    bool hasWeather = weatherIsAvailable();
    int yPos = hasWeather ? SLATE_TIME_Y : SLATE_TIME_NO_WEATHER_Y;
    writeTextReplaceBack(timeStr, centerX, yPos, GxEPD_BLACK, GxEPD_WHITE);
}

const watchfaceDefOne slateDef = {
    .drawTimeBeforeApply = showTimeFull,
    .drawTimeAfterApply = [](bool forceDraw)
    {
        // Only redraw if hour actually changed or forced
        static uint8_t lastHour = 255;
        bool hourChanged = (timeRTCLocal.Hour != lastHour);
        
        if (hourChanged || forceDraw) {
            lastHour = timeRTCLocal.Hour;
            
            bool hasWeather = weatherIsAvailable();
            
            // Only redraw time if necessary (it's drawn in drawTimeBeforeApply)
            if (forceDraw) {
                showTimeFull();
            }
            
            // Date - only redraw if day changed or layout changed
            if (slateCache.lastDay != timeRTCLocal.Day || forceDraw) {
                slateCache.lastDay = timeRTCLocal.Day;
                
                setFont(getFont(SLATE_FONT));
                setTextSize(2);
                
                String day = String(timeRTCLocal.Day);
                if (day.length() == 1) day = "0" + day;
                
                String month = String(timeRTCLocal.Month + 1);
                if (month.length() == 1) month = "0" + month;
                
                String year = String((timeRTCLocal.Year + 70) % 100);
                if (year.length() == 1) year = "0" + year;
                
                String dayName = getDayName(0);
                dayName = dayName.substring(0, 3);
                dayName.toUpperCase();
                
                String dateStr = day + "." + month + "." + year + " | " + dayName;
                
                int dateY = hasWeather ? SLATE_DATE_Y : SLATE_DATE_NO_WEATHER_Y;
                dis->fillRect(SLATE_BORDER_WIDTH + 5, dateY - 10, SLATE_CLEAR_WIDTH, 20, GxEPD_WHITE);
                writeTextCenterReplaceBack(dateStr, dateY);
            }
            
            // Battery - only redraw if level changed or layout changed
            if (slateCache.lastBatteryLevel != rM.batteryPercantageWF || forceDraw) {
                slateCache.lastBatteryLevel = rM.batteryPercantageWF;
                
                String battBar = "[";
                int segments = (rM.batteryPercantageWF + 10) / 20;
                
                for(int i = 0; i < 5; i++) {
                    battBar += (i < segments) ? "=" : " ";
                }
                battBar += "]";
                
                int batteryY = hasWeather ? SLATE_BATTERY_Y : SLATE_BATTERY_NO_WEATHER_Y;
                dis->fillRect(SLATE_BORDER_WIDTH + 5, batteryY - 10, SLATE_CLEAR_WIDTH, 20, GxEPD_WHITE);
                writeTextCenterReplaceBack(battBar, batteryY);
            }
            
#if WEATHER_INFO
            // Weather - only update if actually available (checked max once per hour)
            if (hasWeather) {
                // Only fetch weather data if we need to update
                OM_OneHourWeather wData = weatherGetDataHourly(WEATHER_WATCHFACE_HOUR_OFFSET);
                
                // Temperature
                setFont(getFont(SLATE_FONT));
                setTextSize(3);
                int temp = (int)roundf(wData.temp);
                String tempStr = String(temp) + "C";
                
                dis->fillRect(SLATE_BORDER_WIDTH + 5, SLATE_TEMP_Y - 15, SLATE_CLEAR_WIDTH, 25, GxEPD_WHITE);
                writeTextCenterReplaceBack(tempStr, SLATE_TEMP_Y);
                
                // Weather condition
                setTextSize(2);
                String condition;
                switch(wData.weather_code) {
                    case 0: condition = "Clear"; break;
                    case 1: condition = "Clear+"; break;
                    case 2: condition = "Part Cloud"; break;
                    case 3: condition = "Cloudy"; break;
                    case 45:
                    case 48: condition = "Foggy"; break;
                    case 51:
                    case 53:
                    case 55: condition = "Drizzle"; break;
                    case 56:
                    case 57: condition = "Cold Rain"; break;
                    case 61:
                    case 63:
                    case 65: condition = "Rainy"; break;
                    case 66:
                    case 67: condition = "Cold Rain"; break;
                    case 71:
                    case 73:
                    case 75:
                    case 77: condition = "Snowy"; break;
                    case 80:
                    case 81:
                    case 82: condition = "Showers"; break;
                    case 85:
                    case 86: condition = "Snow Shwr"; break;
                    case 95: condition = "Thunder"; break;
                    case 96:
                    case 99: condition = "Hail"; break;
                    default: condition = "Unknown"; break;
                }
                
                dis->fillRect(SLATE_BORDER_WIDTH + 5, SLATE_WEATHER_Y - 10, SLATE_CLEAR_WIDTH, 25, GxEPD_WHITE);
                writeTextCenterReplaceBack(condition, SLATE_WEATHER_Y);
            } else if (forceDraw) {
                // Only clear weather areas on force draw
                dis->fillRect(SLATE_BORDER_WIDTH + 5, SLATE_TEMP_Y - 15, SLATE_CLEAR_WIDTH, 25, GxEPD_WHITE);
                dis->fillRect(SLATE_BORDER_WIDTH + 5, SLATE_WEATHER_Y - 10, SLATE_CLEAR_WIDTH, 25, GxEPD_WHITE);
            }
#else
            if (forceDraw) {
                // Only clear weather areas since no weather support
                dis->fillRect(SLATE_BORDER_WIDTH + 5, SLATE_TEMP_Y - 15, SLATE_CLEAR_WIDTH, 25, GxEPD_WHITE);
                dis->fillRect(SLATE_BORDER_WIDTH + 5, SLATE_WEATHER_Y - 10, SLATE_CLEAR_WIDTH, 25, GxEPD_WHITE);
            }
#endif
            
            // Redraw border if force draw (ensures border stays intact)
            if (forceDraw) {
                dis->drawRect(0, 0, 200, 200, GxEPD_BLACK);
                dis->drawRect(1, 1, 198, 198, GxEPD_BLACK);
            }
        }
    },
    .drawDay = []()
    {
        // Cache check prevents unnecessary redraws
        if (slateCache.lastDay == timeRTCLocal.Day) return;
        
        bool hasWeather = weatherIsAvailable();
        
        setFont(getFont(SLATE_FONT));
        setTextSize(2);
        
        String day = String(timeRTCLocal.Day);
        if (day.length() == 1) day = "0" + day;
        
        String month = String(timeRTCLocal.Month + 1);
        if (month.length() == 1) month = "0" + month;
        
        String year = String((timeRTCLocal.Year + 70) % 100);
        if (year.length() == 1) year = "0" + year;
        
        String dayName = getDayName(0);
        dayName = dayName.substring(0, 3);
        dayName.toUpperCase();
        
        String dateStr = day + "." + month + "." + year + " | " + dayName;
        
        int dateY = hasWeather ? SLATE_DATE_Y : SLATE_DATE_NO_WEATHER_Y;
        dis->fillRect(SLATE_BORDER_WIDTH + 5, dateY - 10, SLATE_CLEAR_WIDTH, 20, GxEPD_WHITE);
        writeTextCenterReplaceBack(dateStr, dateY);
        
        slateCache.lastDay = timeRTCLocal.Day;
    },
    .drawMonth = []()
    {
        // Cache check prevents unnecessary redraws
        if (slateCache.lastMonth == timeRTCLocal.Month) return;
        
        bool hasWeather = weatherIsAvailable();
        
        setFont(getFont(SLATE_FONT));
        setTextSize(2);
        
        String day = String(timeRTCLocal.Day);
        if (day.length() == 1) day = "0" + day;
        
        String month = String(timeRTCLocal.Month + 1);
        if (month.length() == 1) month = "0" + month;
        
        String year = String((timeRTCLocal.Year + 70) % 100);
        if (year.length() == 1) year = "0" + year;
        
        String dayName = getDayName(0);
        dayName = dayName.substring(0, 3);
        dayName.toUpperCase();
        
        String dateStr = day + "." + month + "." + year + " | " + dayName;
        
        int dateY = hasWeather ? SLATE_DATE_Y : SLATE_DATE_NO_WEATHER_Y;
        dis->fillRect(SLATE_BORDER_WIDTH + 5, dateY - 10, SLATE_CLEAR_WIDTH, 20, GxEPD_WHITE);
        writeTextCenterReplaceBack(dateStr, dateY);
        
        slateCache.lastMonth = timeRTCLocal.Month;
    },
    .showTimeFull = showTimeFull,
    .initWatchface = []()
    {
        dis->fillScreen(GxEPD_WHITE);
        readRTC();
        
        // Reset cache on init
        slateCache.lastHourWeatherCheck = 255;
        slateCache.lastBatteryLevel = 255;
        slateCache.lastDay = 255;
        slateCache.lastMonth = 255;
        
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
        
        String dayName = getDayName(0);
        dayName = dayName.substring(0, 3);
        dayName.toUpperCase();
        
        String dateStr = day + "." + month + "." + year + " | " + dayName;
        
        int dateY = hasWeather ? SLATE_DATE_Y : SLATE_DATE_NO_WEATHER_Y;
        writeTextCenterReplaceBack(dateStr, dateY);
        slateCache.lastDay = timeRTCLocal.Day;
        slateCache.lastMonth = timeRTCLocal.Month;
        
        // Battery
        String battBar = "[";
        int segments = (rM.batteryPercantageWF + 10) / 20;
        
        for(int i = 0; i < 5; i++) {
            battBar += (i < segments) ? "=" : " ";
        }
        battBar += "]";
        
        int batteryY = hasWeather ? SLATE_BATTERY_Y : SLATE_BATTERY_NO_WEATHER_Y;
        writeTextCenterReplaceBack(battBar, batteryY);
        slateCache.lastBatteryLevel = rM.batteryPercantageWF;
        
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
            String condition;
            switch(wData.weather_code) {
                case 0: condition = "Clear"; break;
                case 1: condition = "Clear+"; break;
                case 2: condition = "Part Cloud"; break;
                case 3: condition = "Cloudy"; break;
                case 45:
                case 48: condition = "Foggy"; break;
                case 51:
                case 53:
                case 55: condition = "Drizzle"; break;
                case 56:
                case 57: condition = "Cold Rain"; break;
                case 61:
                case 63:
                case 65: condition = "Rainy"; break;
                case 66:
                case 67: condition = "Cold Rain"; break;
                case 71:
                case 73:
                case 75:
                case 77: condition = "Snowy"; break;
                case 80:
                case 81:
                case 82: condition = "Showers"; break;
                case 85:
                case 86: condition = "Snow Shwr"; break;
                case 95: condition = "Thunder"; break;
                case 96:
                case 99: condition = "Hail"; break;
                default: condition = "Unknown"; break;
            }
            writeTextCenterReplaceBack(condition, SLATE_WEATHER_Y);
        }
#endif
    },
    .drawBattery = []()
    {
        // Cache check prevents unnecessary redraws
        if (slateCache.lastBatteryLevel == rM.batteryPercantageWF) return;
        
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
        dis->fillRect(SLATE_BORDER_WIDTH + 5, batteryY - 10, SLATE_CLEAR_WIDTH, 20, GxEPD_WHITE);
        writeTextCenterReplaceBack(battBar, batteryY);
        
        slateCache.lastBatteryLevel = rM.batteryPercantageWF;
    },
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

#endif#include "slate.h"
#include "rtcMem.h"
#include <esp_sleep.h>

#if WATCHFACE_SLATE

// Font configuration
#define SLATE_FONT "dogicapixel4"

// Layout constants
#define SLATE_BORDER_WIDTH 2
#define SLATE_TIME_Y 50
#define SLATE_DATE_Y 90
#define SLATE_BATTERY_Y 120
#define SLATE_TEMP_Y 150
#define SLATE_WEATHER_Y 180

#define SLATE_TIME_NO_WEATHER_Y 50
#define SLATE_DATE_NO_WEATHER_Y 110
#define SLATE_BATTERY_NO_WEATHER_Y 150

#define SLATE_CLEAR_WIDTH 190

// RTC data for caching
RTC_DATA_ATTR static struct {
    bool weatherAvailable = false;
    uint8_t lastHourWeatherCheck = 255;  // Force initial check
    uint8_t lastBatteryLevel = 255;      // Force initial draw
    uint8_t lastDay = 255;               // Force initial draw
    uint8_t lastMonth = 255;             // Force initial draw
} slateCache;

// Check if weather is available (cached for battery optimization)
bool weatherIsAvailable() {
#if WEATHER_INFO
    // Only check weather once per hour to save battery
    if (slateCache.lastHourWeatherCheck != timeRTCLocal.Hour) {
        slateCache.lastHourWeatherCheck = timeRTCLocal.Hour;
        OM_OneHourWeather wData = weatherGetDataHourly(WEATHER_WATCHFACE_HOUR_OFFSET);
        slateCache.weatherAvailable = wData.fine;
    }
    return slateCache.weatherAvailable;
#else
    return false;
#endif
}

void showTimeFull()
{
    String timeStr = getHourMinute(timeRTCLocal);
    
    setFont(getFont(SLATE_FONT));
    setTextSize(4);
    
    dis->fillRect(SLATE_BORDER_WIDTH + 5, 10, SLATE_CLEAR_WIDTH, 55, GxEPD_WHITE);
    
    uint16_t w, h;
    getTextBounds(timeStr, NULL, NULL, &w, &h);
    int centerX = (200 - w) / 2;
    
    bool hasWeather = weatherIsAvailable();
    int yPos = hasWeather ? SLATE_TIME_Y : SLATE_TIME_NO_WEATHER_Y;
    writeTextReplaceBack(timeStr, centerX, yPos, GxEPD_BLACK, GxEPD_WHITE);
}

const watchfaceDefOne slateDef = {
    .drawTimeBeforeApply = showTimeFull,
    .drawTimeAfterApply = [](bool forceDraw)
    {
        // Only redraw if hour actually changed or forced
        static uint8_t lastHour = 255;
        bool hourChanged = (timeRTCLocal.Hour != lastHour);
        
        if (hourChanged || forceDraw) {
            lastHour = timeRTCLocal.Hour;
            
            bool hasWeather = weatherIsAvailable();
            
            // Only redraw time if necessary (it's drawn in drawTimeBeforeApply)
            if (forceDraw) {
                showTimeFull();
            }
            
            // Date - only redraw if day changed or layout changed
            if (slateCache.lastDay != timeRTCLocal.Day || forceDraw) {
                slateCache.lastDay = timeRTCLocal.Day;
                
                setFont(getFont(SLATE_FONT));
                setTextSize(2);
                
                String day = String(timeRTCLocal.Day);
                if (day.length() == 1) day = "0" + day;
                
                String month = String(timeRTCLocal.Month + 1);
                if (month.length() == 1) month = "0" + month;
                
                String year = String((timeRTCLocal.Year + 70) % 100);
                if (year.length() == 1) year = "0" + year;
                
                String dayName = getDayName(0);
                dayName = dayName.substring(0, 3);
                dayName.toUpperCase();
                
                String dateStr = day + "." + month + "." + year + " | " + dayName;
                
                int dateY = hasWeather ? SLATE_DATE_Y : SLATE_DATE_NO_WEATHER_Y;
                dis->fillRect(SLATE_BORDER_WIDTH + 5, dateY - 10, SLATE_CLEAR_WIDTH, 20, GxEPD_WHITE);
                writeTextCenterReplaceBack(dateStr, dateY);
            }
            
            // Battery - only redraw if level changed or layout changed
            if (slateCache.lastBatteryLevel != rM.batteryPercantageWF || forceDraw) {
                slateCache.lastBatteryLevel = rM.batteryPercantageWF;
                
                String battBar = "[";
                int segments = (rM.batteryPercantageWF + 10) / 20;
                
                for(int i = 0; i < 5; i++) {
                    battBar += (i < segments) ? "=" : " ";
                }
                battBar += "]";
                
                int batteryY = hasWeather ? SLATE_BATTERY_Y : SLATE_BATTERY_NO_WEATHER_Y;
                dis->fillRect(SLATE_BORDER_WIDTH + 5, batteryY - 10, SLATE_CLEAR_WIDTH, 20, GxEPD_WHITE);
                writeTextCenterReplaceBack(battBar, batteryY);
            }
            
#if WEATHER_INFO
            // Weather - only update if actually available (checked max once per hour)
            if (hasWeather) {
                // Only fetch weather data if we need to update
                OM_OneHourWeather wData = weatherGetDataHourly(WEATHER_WATCHFACE_HOUR_OFFSET);
                
                // Temperature
                setFont(getFont(SLATE_FONT));
                setTextSize(3);
                int temp = (int)roundf(wData.temp);
                String tempStr = String(temp) + "C";
                
                dis->fillRect(SLATE_BORDER_WIDTH + 5, SLATE_TEMP_Y - 15, SLATE_CLEAR_WIDTH, 25, GxEPD_WHITE);
                writeTextCenterReplaceBack(tempStr, SLATE_TEMP_Y);
                
                // Weather condition
                setTextSize(2);
                String condition;
                switch(wData.weather_code) {
                    case 0: condition = "Clear"; break;
                    case 1: condition = "Clear+"; break;
                    case 2: condition = "Part Cloud"; break;
                    case 3: condition = "Cloudy"; break;
                    case 45:
                    case 48: condition = "Foggy"; break;
                    case 51:
                    case 53:
                    case 55: condition = "Drizzle"; break;
                    case 56:
                    case 57: condition = "Cold Rain"; break;
                    case 61:
                    case 63:
                    case 65: condition = "Rainy"; break;
                    case 66:
                    case 67: condition = "Cold Rain"; break;
                    case 71:
                    case 73:
                    case 75:
                    case 77: condition = "Snowy"; break;
                    case 80:
                    case 81:
                    case 82: condition = "Showers"; break;
                    case 85:
                    case 86: condition = "Snow Shwr"; break;
                    case 95: condition = "Thunder"; break;
                    case 96:
                    case 99: condition = "Hail"; break;
                    default: condition = "Unknown"; break;
                }
                
                dis->fillRect(SLATE_BORDER_WIDTH + 5, SLATE_WEATHER_Y - 10, SLATE_CLEAR_WIDTH, 25, GxEPD_WHITE);
                writeTextCenterReplaceBack(condition, SLATE_WEATHER_Y);
            } else if (forceDraw) {
                // Only clear weather areas on force draw
                dis->fillRect(SLATE_BORDER_WIDTH + 5, SLATE_TEMP_Y - 15, SLATE_CLEAR_WIDTH, 25, GxEPD_WHITE);
                dis->fillRect(SLATE_BORDER_WIDTH + 5, SLATE_WEATHER_Y - 10, SLATE_CLEAR_WIDTH, 25, GxEPD_WHITE);
            }
#else
            if (forceDraw) {
                // Only clear weather areas since no weather support
                dis->fillRect(SLATE_BORDER_WIDTH + 5, SLATE_TEMP_Y - 15, SLATE_CLEAR_WIDTH, 25, GxEPD_WHITE);
                dis->fillRect(SLATE_BORDER_WIDTH + 5, SLATE_WEATHER_Y - 10, SLATE_CLEAR_WIDTH, 25, GxEPD_WHITE);
            }
#endif
        }
    },
    .drawDay = []()
    {
        // Cache check prevents unnecessary redraws
        if (slateCache.lastDay == timeRTCLocal.Day) return;
        
        bool hasWeather = weatherIsAvailable();
        
        setFont(getFont(SLATE_FONT));
        setTextSize(2);
        
        String day = String(timeRTCLocal.Day);
        if (day.length() == 1) day = "0" + day;
        
        String month = String(timeRTCLocal.Month + 1);
        if (month.length() == 1) month = "0" + month;
        
        String year = String((timeRTCLocal.Year + 70) % 100);
        if (year.length() == 1) year = "0" + year;
        
        String dayName = getDayName(0);
        dayName = dayName.substring(0, 3);
        dayName.toUpperCase();
        
        String dateStr = day + "." + month + "." + year + " | " + dayName;
        
        int dateY = hasWeather ? SLATE_DATE_Y : SLATE_DATE_NO_WEATHER_Y;
        dis->fillRect(SLATE_BORDER_WIDTH + 5, dateY - 10, SLATE_CLEAR_WIDTH, 20, GxEPD_WHITE);
        writeTextCenterReplaceBack(dateStr, dateY);
        
        slateCache.lastDay = timeRTCLocal.Day;
    },
    .drawMonth = []()
    {
        // Cache check prevents unnecessary redraws
        if (slateCache.lastMonth == timeRTCLocal.Month) return;
        
        bool hasWeather = weatherIsAvailable();
        
        setFont(getFont(SLATE_FONT));
        setTextSize(2);
        
        String day = String(timeRTCLocal.Day);
        if (day.length() == 1) day = "0" + day;
        
        String month = String(timeRTCLocal.Month + 1);
        if (month.length() == 1) month = "0" + month;
        
        String year = String((timeRTCLocal.Year + 70) % 100);
        if (year.length() == 1) year = "0" + year;
        
        String dayName = getDayName(0);
        dayName = dayName.substring(0, 3);
        dayName.toUpperCase();
        
        String dateStr = day + "." + month + "." + year + " | " + dayName;
        
        int dateY = hasWeather ? SLATE_DATE_Y : SLATE_DATE_NO_WEATHER_Y;
        dis->fillRect(SLATE_BORDER_WIDTH + 5, dateY - 10, SLATE_CLEAR_WIDTH, 20, GxEPD_WHITE);
        writeTextCenterReplaceBack(dateStr, dateY);
        
        slateCache.lastMonth = timeRTCLocal.Month;
    },
    .showTimeFull = showTimeFull,
    .initWatchface = []()
    {
        dis->fillScreen(GxEPD_WHITE);
        readRTC();
        
        // Reset cache on init
        slateCache.lastHourWeatherCheck = 255;
        slateCache.lastBatteryLevel = 255;
        slateCache.lastDay = 255;
        slateCache.lastMonth = 255;
        
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
        
        String dayName = getDayName(0);
        dayName = dayName.substring(0, 3);
        dayName.toUpperCase();
        
        String dateStr = day + "." + month + "." + year + " | " + dayName;
        
        int dateY = hasWeather ? SLATE_DATE_Y : SLATE_DATE_NO_WEATHER_Y;
        writeTextCenterReplaceBack(dateStr, dateY);
        slateCache.lastDay = timeRTCLocal.Day;
        slateCache.lastMonth = timeRTCLocal.Month;
        
        // Battery
        String battBar = "[";
        int segments = (rM.batteryPercantageWF + 10) / 20;
        
        for(int i = 0; i < 5; i++) {
            battBar += (i < segments) ? "=" : " ";
        }
        battBar += "]";
        
        int batteryY = hasWeather ? SLATE_BATTERY_Y : SLATE_BATTERY_NO_WEATHER_Y;
        writeTextCenterReplaceBack(battBar, batteryY);
        slateCache.lastBatteryLevel = rM.batteryPercantageWF;
        
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
            String condition;
            switch(wData.weather_code) {
                case 0: condition = "Clear"; break;
                case 1: condition = "Clear+"; break;
                case 2: condition = "Part Cloud"; break;
                case 3: condition = "Cloudy"; break;
                case 45:
                case 48: condition = "Foggy"; break;
                case 51:
                case 53:
                case 55: condition = "Drizzle"; break;
                case 56:
                case 57: condition = "Cold Rain"; break;
                case 61:
                case 63:
                case 65: condition = "Rainy"; break;
                case 66:
                case 67: condition = "Cold Rain"; break;
                case 71:
                case 73:
                case 75:
                case 77: condition = "Snowy"; break;
                case 80:
                case 81:
                case 82: condition = "Showers"; break;
                case 85:
                case 86: condition = "Snow Shwr"; break;
                case 95: condition = "Thunder"; break;
                case 96:
                case 99: condition = "Hail"; break;
                default: condition = "Unknown"; break;
            }
            writeTextCenterReplaceBack(condition, SLATE_WEATHER_Y);
        }
#endif
    },
    .drawBattery = []()
    {
        // Cache check prevents unnecessary redraws
        if (slateCache.lastBatteryLevel == rM.batteryPercantageWF) return;
        
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
        dis->fillRect(SLATE_BORDER_WIDTH + 5, batteryY - 10, SLATE_CLEAR_WIDTH, 20, GxEPD_WHITE);
        writeTextCenterReplaceBack(battBar, batteryY);
        
        slateCache.lastBatteryLevel = rM.batteryPercantageWF;
    },
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
    #include "slate.h"
#include "rtcMem.h"
#include <esp_sleep.h>

#if WATCHFACE_SLATE

// Font configuration
#define SLATE_FONT "dogicapixel4"

// Layout constants
#define SLATE_BORDER_WIDTH 2
#define SLATE_TIME_Y 50
#define SLATE_DATE_Y 90
#define SLATE_BATTERY_Y 120
#define SLATE_TEMP_Y 150
#define SLATE_WEATHER_Y 180

#define SLATE_TIME_NO_WEATHER_Y 50
#define SLATE_DATE_NO_WEATHER_Y 110
#define SLATE_BATTERY_NO_WEATHER_Y 150

#define SLATE_CLEAR_WIDTH 190

// RTC data for caching
RTC_DATA_ATTR static struct {
    bool weatherAvailable = false;
    uint8_t lastHourWeatherCheck = 255;  // Force initial check
    uint8_t lastBatteryLevel = 255;      // Force initial draw
    uint8_t lastDay = 255;               // Force initial draw
    uint8_t lastMonth = 255;             // Force initial draw
} slateCache;

// Check if weather is available (cached for battery optimization)
bool weatherIsAvailable() {
#if WEATHER_INFO
    // Only check weather once per hour to save battery
    if (slateCache.lastHourWeatherCheck != timeRTCLocal.Hour) {
        slateCache.lastHourWeatherCheck = timeRTCLocal.Hour;
        OM_OneHourWeather wData = weatherGetDataHourly(WEATHER_WATCHFACE_HOUR_OFFSET);
        slateCache.weatherAvailable = wData.fine;
    }
    return slateCache.weatherAvailable;
#else
    return false;
#endif
}

void showTimeFull()
{
    String timeStr = getHourMinute(timeRTCLocal);
    
    setFont(getFont(SLATE_FONT));
    setTextSize(4);
    
    dis->fillRect(SLATE_BORDER_WIDTH + 5, 10, SLATE_CLEAR_WIDTH, 55, GxEPD_WHITE);
    
    uint16_t w, h;
    getTextBounds(timeStr, NULL, NULL, &w, &h);
    int centerX = (200 - w) / 2;
    
    bool hasWeather = weatherIsAvailable();
    int yPos = hasWeather ? SLATE_TIME_Y : SLATE_TIME_NO_WEATHER_Y;
    writeTextReplaceBack(timeStr, centerX, yPos, GxEPD_BLACK, GxEPD_WHITE);
}

const watchfaceDefOne slateDef = {
    .drawTimeBeforeApply = showTimeFull,
    .drawTimeAfterApply = [](bool forceDraw)
    {
        // Only redraw if hour actually changed or forced
        static uint8_t lastHour = 255;
        bool hourChanged = (timeRTCLocal.Hour != lastHour);
        
        if (hourChanged || forceDraw) {
            lastHour = timeRTCLocal.Hour;
            
            bool hasWeather = weatherIsAvailable();
            
            // Only redraw time if necessary (it's drawn in drawTimeBeforeApply)
            if (forceDraw) {
                showTimeFull();
            }
            
            // Date - only redraw if day changed or layout changed
            if (slateCache.lastDay != timeRTCLocal.Day || forceDraw) {
                slateCache.lastDay = timeRTCLocal.Day;
                
                setFont(getFont(SLATE_FONT));
                setTextSize(2);
                
                String day = String(timeRTCLocal.Day);
                if (day.length() == 1) day = "0" + day;
                
                String month = String(timeRTCLocal.Month + 1);
                if (month.length() == 1) month = "0" + month;
                
                String year = String((timeRTCLocal.Year + 70) % 100);
                if (year.length() == 1) year = "0" + year;
                
                String dayName = getDayName(0);
                dayName = dayName.substring(0, 3);
                dayName.toUpperCase();
                
                String dateStr = day + "." + month + "." + year + " | " + dayName;
                
                int dateY = hasWeather ? SLATE_DATE_Y : SLATE_DATE_NO_WEATHER_Y;
                dis->fillRect(SLATE_BORDER_WIDTH + 5, dateY - 10, SLATE_CLEAR_WIDTH, 20, GxEPD_WHITE);
                writeTextCenterReplaceBack(dateStr, dateY);
            }
            
            // Battery - only redraw if level changed or layout changed
            if (slateCache.lastBatteryLevel != rM.batteryPercantageWF || forceDraw) {
                slateCache.lastBatteryLevel = rM.batteryPercantageWF;
                
                String battBar = "[";
                int segments = (rM.batteryPercantageWF + 10) / 20;
                
                for(int i = 0; i < 5; i++) {
                    battBar += (i < segments) ? "=" : " ";
                }
                battBar += "]";
                
                int batteryY = hasWeather ? SLATE_BATTERY_Y : SLATE_BATTERY_NO_WEATHER_Y;
                dis->fillRect(SLATE_BORDER_WIDTH + 5, batteryY - 10, SLATE_CLEAR_WIDTH, 20, GxEPD_WHITE);
                writeTextCenterReplaceBack(battBar, batteryY);
            }
            
#if WEATHER_INFO
            // Weather - only update if actually available (checked max once per hour)
            if (hasWeather) {
                // Only fetch weather data if we need to update
                OM_OneHourWeather wData = weatherGetDataHourly(WEATHER_WATCHFACE_HOUR_OFFSET);
                
                // Temperature
                setFont(getFont(SLATE_FONT));
                setTextSize(3);
                int temp = (int)roundf(wData.temp);
                String tempStr = String(temp) + "C";
                
                dis->fillRect(SLATE_BORDER_WIDTH + 5, SLATE_TEMP_Y - 15, SLATE_CLEAR_WIDTH, 25, GxEPD_WHITE);
                writeTextCenterReplaceBack(tempStr, SLATE_TEMP_Y);
                
                // Weather condition
                setTextSize(2);
                String condition;
                switch(wData.weather_code) {
                    case 0: condition = "Clear"; break;
                    case 1: condition = "Clear+"; break;
                    case 2: condition = "Part Cloud"; break;
                    case 3: condition = "Cloudy"; break;
                    case 45:
                    case 48: condition = "Foggy"; break;
                    case 51:
                    case 53:
                    case 55: condition = "Drizzle"; break;
                    case 56:
                    case 57: condition = "Cold Rain"; break;
                    case 61:
                    case 63:
                    case 65: condition = "Rainy"; break;
                    case 66:
                    case 67: condition = "Cold Rain"; break;
                    case 71:
                    case 73:
                    case 75:
                    case 77: condition = "Snowy"; break;
                    case 80:
                    case 81:
                    case 82: condition = "Showers"; break;
                    case 85:
                    case 86: condition = "Snow Shwr"; break;
                    case 95: condition = "Thunder"; break;
                    case 96:
                    case 99: condition = "Hail"; break;
                    default: condition = "Unknown"; break;
                }
                
                dis->fillRect(SLATE_BORDER_WIDTH + 5, SLATE_WEATHER_Y - 10, SLATE_CLEAR_WIDTH, 25, GxEPD_WHITE);
                writeTextCenterReplaceBack(condition, SLATE_WEATHER_Y);
            } else if (forceDraw) {
                // Only clear weather areas on force draw
                dis->fillRect(SLATE_BORDER_WIDTH + 5, SLATE_TEMP_Y - 15, SLATE_CLEAR_WIDTH, 25, GxEPD_WHITE);
                dis->fillRect(SLATE_BORDER_WIDTH + 5, SLATE_WEATHER_Y - 10, SLATE_CLEAR_WIDTH, 25, GxEPD_WHITE);
            }
#else
            if (forceDraw) {
                // Only clear weather areas since no weather support
                dis->fillRect(SLATE_BORDER_WIDTH + 5, SLATE_TEMP_Y - 15, SLATE_CLEAR_WIDTH, 25, GxEPD_WHITE);
                dis->fillRect(SLATE_BORDER_WIDTH + 5, SLATE_WEATHER_Y - 10, SLATE_CLEAR_WIDTH, 25, GxEPD_WHITE);
            }
#endif
        }
    },
    .drawDay = []()
    {
        // Cache check prevents unnecessary redraws
        if (slateCache.lastDay == timeRTCLocal.Day) return;
        
        bool hasWeather = weatherIsAvailable();
        
        setFont(getFont(SLATE_FONT));
        setTextSize(2);
        
        String day = String(timeRTCLocal.Day);
        if (day.length() == 1) day = "0" + day;
        
        String month = String(timeRTCLocal.Month + 1);
        if (month.length() == 1) month = "0" + month;
        
        String year = String((timeRTCLocal.Year + 70) % 100);
        if (year.length() == 1) year = "0" + year;
        
        String dayName = getDayName(0);
        dayName = dayName.substring(0, 3);
        dayName.toUpperCase();
        
        String dateStr = day + "." + month + "." + year + " | " + dayName;
        
        int dateY = hasWeather ? SLATE_DATE_Y : SLATE_DATE_NO_WEATHER_Y;
        dis->fillRect(SLATE_BORDER_WIDTH + 5, dateY - 10, SLATE_CLEAR_WIDTH, 20, GxEPD_WHITE);
        writeTextCenterReplaceBack(dateStr, dateY);
        
        slateCache.lastDay = timeRTCLocal.Day;
    },
    .drawMonth = []()
    {
        // Cache check prevents unnecessary redraws
        if (slateCache.lastMonth == timeRTCLocal.Month) return;
        
        bool hasWeather = weatherIsAvailable();
        
        setFont(getFont(SLATE_FONT));
        setTextSize(2);
        
        String day = String(timeRTCLocal.Day);
        if (day.length() == 1) day = "0" + day;
        
        String month = String(timeRTCLocal.Month + 1);
        if (month.length() == 1) month = "0" + month;
        
        String year = String((timeRTCLocal.Year + 70) % 100);
        if (year.length() == 1) year = "0" + year;
        
        String dayName = getDayName(0);
        dayName = dayName.substring(0, 3);
        dayName.toUpperCase();
        
        String dateStr = day + "." + month + "." + year + " | " + dayName;
        
        int dateY = hasWeather ? SLATE_DATE_Y : SLATE_DATE_NO_WEATHER_Y;
        dis->fillRect(SLATE_BORDER_WIDTH + 5, dateY - 10, SLATE_CLEAR_WIDTH, 20, GxEPD_WHITE);
        writeTextCenterReplaceBack(dateStr, dateY);
        
        slateCache.lastMonth = timeRTCLocal.Month;
    },
    .showTimeFull = showTimeFull,
    .initWatchface = []()
    {
        dis->fillScreen(GxEPD_WHITE);
        readRTC();
        
        // Reset cache on init
        slateCache.lastHourWeatherCheck = 255;
        slateCache.lastBatteryLevel = 255;
        slateCache.lastDay = 255;
        slateCache.lastMonth = 255;
        
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
        
        String dayName = getDayName(0);
        dayName = dayName.substring(0, 3);
        dayName.toUpperCase();
        
        String dateStr = day + "." + month + "." + year + " | " + dayName;
        
        int dateY = hasWeather ? SLATE_DATE_Y : SLATE_DATE_NO_WEATHER_Y;
        writeTextCenterReplaceBack(dateStr, dateY);
        slateCache.lastDay = timeRTCLocal.Day;
        slateCache.lastMonth = timeRTCLocal.Month;
        
        // Battery
        String battBar = "[";
        int segments = (rM.batteryPercantageWF + 10) / 20;
        
        for(int i = 0; i < 5; i++) {
            battBar += (i < segments) ? "=" : " ";
        }
        battBar += "]";
        
        int batteryY = hasWeather ? SLATE_BATTERY_Y : SLATE_BATTERY_NO_WEATHER_Y;
        writeTextCenterReplaceBack(battBar, batteryY);
        slateCache.lastBatteryLevel = rM.batteryPercantageWF;
        
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
            String condition;
            switch(wData.weather_code) {
                case 0: condition = "Clear"; break;
                case 1: condition = "Clear+"; break;
                case 2: condition = "Part Cloud"; break;
                case 3: condition = "Cloudy"; break;
                case 45:
                case 48: condition = "Foggy"; break;
                case 51:
                case 53:
                case 55: condition = "Drizzle"; break;
                case 56:
                case 57: condition = "Cold Rain"; break;
                case 61:
                case 63:
                case 65: condition = "Rainy"; break;
                case 66:
                case 67: condition = "Cold Rain"; break;
                case 71:
                case 73:
                case 75:
                case 77: condition = "Snowy"; break;
                case 80:
                case 81:
                case 82: condition = "Showers"; break;
                case 85:
                case 86: condition = "Snow Shwr"; break;
                case 95: condition = "Thunder"; break;
                case 96:
                case 99: condition = "Hail"; break;
                default: condition = "Unknown"; break;
            }
            writeTextCenterReplaceBack(condition, SLATE_WEATHER_Y);
        }
#endif
    },
    .drawBattery = []()
    {
        // Cache check prevents unnecessary redraws
        if (slateCache.lastBatteryLevel == rM.batteryPercantageWF) return;
        
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
        dis->fillRect(SLATE_BORDER_WIDTH + 5, batteryY - 10, SLATE_CLEAR_WIDTH, 20, GxEPD_WHITE);
        writeTextCenterReplaceBack(battBar, batteryY);
        
        slateCache.lastBatteryLevel = rM.batteryPercantageWF;
    },
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
    .watchfaceModules = false,
    .watchfaceModSquare = {.size{.w = 0, .h = 0}, .cord{.x = 0, .y = 0}},
    .someDrawingSquare = {.size{.w = 0, .h = 0}, .cord{.x = 0, .y = 0}},
    .isModuleEngaged = []() { return false; }
};

#endif