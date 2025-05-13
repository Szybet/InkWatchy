#include "slate.h"
#include "rtcMem.h"

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

// Safe zone width (200 - 2*border - 4*margin)
#define SLATE_SAFE_WIDTH 186

// Smart text clearing that calculates exact width needed
static void clearTextArea(const String& text, int y, int fontSize, int extraHeight = 5) {
    setFont(getFont(SLATE_FONT));
    setTextSize(fontSize);
    
    uint16_t w, h;
    // Make a local copy since getTextBounds expects non-const reference
    String textCopy = text;
    getTextBounds(textCopy, NULL, NULL, &w, &h);
    
    // Add small margin but stay well within borders
    int clearWidth = min(w + 8, SLATE_SAFE_WIDTH);
    int clearX = SLATE_BORDER_WIDTH + 4 + (SLATE_SAFE_WIDTH - clearWidth) / 2;
    
    // For time, we need more vertical space
    int clearHeight = (fontSize == 4) ? h + 20 : h + 10;
    int clearY = y - clearHeight + 5;
    
    dis->fillRect(clearX, clearY, clearWidth, clearHeight + extraHeight, GxEPD_WHITE);
}

// Check if weather is available (cached for battery optimization)
bool weatherIsAvailable() {
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

static void showTimeFull()
{
    String timeStr = getHourMinute(timeRTCLocal);
    
    bool hasWeather = weatherIsAvailable();
    int yPos = hasWeather ? SLATE_TIME_Y : SLATE_TIME_NO_WEATHER_Y;
    
    // Smart clear for time
    clearTextArea(timeStr, yPos, 4, 15);
    
    setFont(getFont(SLATE_FONT));
    setTextSize(4);
    writeTextCenterReplaceBack(timeStr, yPos);
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
                
                String dayName = getDayName(0);
                dayName = dayName.substring(0, 3);
                dayName.toUpperCase();
                
                String dateStr = day + "." + month + "." + year + " | " + dayName;
                
                int dateY = hasWeather ? SLATE_DATE_Y : SLATE_DATE_NO_WEATHER_Y;
                clearTextArea(dateStr, dateY, 2);
                writeTextCenterReplaceBack(dateStr, dateY);
            }
            
            // Battery - only redraw if level changed or layout changed
            if (rM.slate.lastBatteryLevel != rM.batteryPercantageWF || forceDraw) {
                rM.slate.lastBatteryLevel = rM.batteryPercantageWF;
                
                String battBar = "[";
                int segments = (rM.batteryPercantageWF + 10) / 20;
                
                for(int i = 0; i < 5; i++) {
                    battBar += (i < segments) ? "=" : " ";
                }
                battBar += "]";
                
                int batteryY = hasWeather ? SLATE_BATTERY_Y : SLATE_BATTERY_NO_WEATHER_Y;
                clearTextArea(battBar, batteryY, 2);
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
                
                clearTextArea(tempStr, SLATE_TEMP_Y, 3, 5);
                writeTextCenterReplaceBack(tempStr, SLATE_TEMP_Y);
                
                // Weather condition
                setTextSize(2);
                String condition;
                switch(wData.weather_code) {
                    case 0: condition = "Clear Sky"; break;
                    case 1: condition = "Mostly Clear"; break;
                    case 2: condition = "Partly Cloudy"; break;
                    case 3: condition = "Overcast"; break;
                    case 45: condition = "Foggy"; break;
                    case 48: condition = "Heavy Fog"; break;
                    case 51: condition = "Light Drizzle"; break;
                    case 53: condition = "Drizzling"; break;
                    case 55: condition = "Heavy Drizzle"; break;
                    case 56: condition = "Light Freezing"; break;
                    case 57: condition = "Heavy Freezing"; break;
                    case 61: condition = "Light Rain"; break;
                    case 63: condition = "Raining"; break;
                    case 65: condition = "Heavy Rain"; break;
                    case 66: condition = "Freezing Rain"; break;
                    case 67: condition = "Freezing Hard"; break;
                    case 71: condition = "Light Snow"; break;
                    case 73: condition = "Snowing"; break;
                    case 75: condition = "Heavy Snow"; break;
                    case 77: condition = "Snow Grains"; break;
                    case 80: condition = "Light Showers"; break;
                    case 81: condition = "Showering"; break;
                    case 82: condition = "Heavy Showers"; break;
                    case 85: condition = "Snow Showers"; break;
                    case 86: condition = "Heavy Snow"; break;
                    case 95: condition = "Thunderstorm"; break;
                    case 96: condition = "Light Hail"; break;
                    case 99: condition = "Heavy Hail"; break;
                    default: condition = "Unknown"; break;
                }
                
                clearTextArea(condition, SLATE_WEATHER_Y, 2);
                writeTextCenterReplaceBack(condition, SLATE_WEATHER_Y);
            } else if (forceDraw) {
                // Only clear weather areas on force draw
                clearTextArea("", SLATE_TEMP_Y, 3, 25);
                clearTextArea("", SLATE_WEATHER_Y, 2, 25);
            }
#else
            if (forceDraw) {
                // Only clear weather areas since no weather support
                clearTextArea("", SLATE_TEMP_Y, 3, 25);
                clearTextArea("", SLATE_WEATHER_Y, 2, 25);
            }
#endif
            
            // Always redraw border to ensure it's intact
            dis->drawRect(0, 0, 200, 200, GxEPD_BLACK);
            dis->drawRect(1, 1, 198, 198, GxEPD_BLACK);
        }
    },
    .drawDay = []()
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
        
        String dayName = getDayName(0);
        dayName = dayName.substring(0, 3);
        dayName.toUpperCase();
        
        String dateStr = day + "." + month + "." + year + " | " + dayName;
        
        int dateY = hasWeather ? SLATE_DATE_Y : SLATE_DATE_NO_WEATHER_Y;
        clearTextArea(dateStr, dateY, 2);
        writeTextCenterReplaceBack(dateStr, dateY);
        
        rM.slate.lastDay = timeRTCLocal.Day;
    },
    .drawMonth = []()
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
        
        String dayName = getDayName(0);
        dayName = dayName.substring(0, 3);
        dayName.toUpperCase();
        
        String dateStr = day + "." + month + "." + year + " | " + dayName;
        
        int dateY = hasWeather ? SLATE_DATE_Y : SLATE_DATE_NO_WEATHER_Y;
        clearTextArea(dateStr, dateY, 2);
        writeTextCenterReplaceBack(dateStr, dateY);
        
        rM.slate.lastMonth = timeRTCLocal.Month;
    },
    .showTimeFull = showTimeFull,
    .initWatchface = []()
    {
        dis->fillScreen(GxEPD_WHITE);
        readRTC();
        
        // Reset cache on init - Force initial draws by setting invalid values
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
        
        String dayName = getDayName(0);
        dayName = dayName.substring(0, 3);
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
            String condition;
            switch(wData.weather_code) {
                case 0: condition = "Clear Sky"; break;
                case 1: condition = "Mostly Clear"; break;
                case 2: condition = "Partly Cloudy"; break;
                case 3: condition = "Overcast"; break;
                case 45: condition = "Foggy"; break;
                case 48: condition = "Heavy Fog"; break;
                case 51: condition = "Light Drizzle"; break;
                case 53: condition = "Drizzling"; break;
                case 55: condition = "Heavy Drizzle"; break;
                case 56: condition = "Light Freezing"; break;
                case 57: condition = "Heavy Freezing"; break;
                case 61: condition = "Light Rain"; break;
                case 63: condition = "Raining"; break;
                case 65: condition = "Heavy Rain"; break;
                case 66: condition = "Freezing Rain"; break;
                case 67: condition = "Freezing Hard"; break;
                case 71: condition = "Light Snow"; break;
                case 73: condition = "Snowing"; break;
                case 75: condition = "Heavy Snow"; break;
                case 77: condition = "Snow Grains"; break;
                case 80: condition = "Light Showers"; break;
                case 81: condition = "Showering"; break;
                case 82: condition = "Heavy Showers"; break;
                case 85: condition = "Snow Showers"; break;
                case 86: condition = "Heavy Snow"; break;
                case 95: condition = "Thunderstorm"; break;
                case 96: condition = "Light Hail"; break;
                case 99: condition = "Heavy Hail"; break;
                default: condition = "Unknown"; break;
                }
            writeTextCenterReplaceBack(condition, SLATE_WEATHER_Y);
        }
#endif
    },
    .drawBattery = []()
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
        clearTextArea(battBar, batteryY, 2);
        writeTextCenterReplaceBack(battBar, batteryY);
        
        rM.slate.lastBatteryLevel = rM.batteryPercantageWF;
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