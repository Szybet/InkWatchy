#include "dosy.h"
#include "rtcMem.h"

#if WATCHFACE_DOSY

// Font configuration
#define DOSY_FONT "UbuntuMono10"
#define DOSY_FONT_Y_OFFSET (3)
#define DOSY_FONT_SIZE (1)

// Terminal Layout Constants
#define DOSY_START_X          (2)
#define DOSY_LINE_H           (20)
#define DOSY_LINE_W           (200 - DOSY_START_X)
#define DOSY_HEADER_NAME_Y    (5)
#define DOSY_HEADER_SYS_Y     (DOSY_HEADER_NAME_Y    + DOSY_LINE_H)
#define DOSY_DATE_CMD_Y       (DOSY_HEADER_SYS_Y     + DOSY_LINE_H)
#define DOSY_DATE_RESULT_Y    (DOSY_DATE_CMD_Y       + DOSY_LINE_H)
#define DOSY_TIME_RESULT_Y    (DOSY_DATE_RESULT_Y    + DOSY_LINE_H)
#define DOSY_WEATHER_CMD_Y    (DOSY_TIME_RESULT_Y    + DOSY_LINE_H)
#define DOSY_WEATHER_RESULT_Y (DOSY_WEATHER_CMD_Y    + DOSY_LINE_H)
#define DOSY_BATT_CMD_Y       (DOSY_WEATHER_RESULT_Y + DOSY_LINE_H)
#define DOSY_BATT_RESULT_Y    (DOSY_BATT_CMD_Y       + DOSY_LINE_H)

// Implementation of Dosy. 
// Some codes adapted from the other watchface implementations
namespace DosySpace
{
    // Internal functions for Dosy
    namespace 
    {
        // Custom text drawing utility function for Dosy
        void print_text(String text, int16_t y)
        {
            setFont(getFont(DOSY_FONT));
            setTextSize(DOSY_FONT_SIZE);

            const int fontOffset = DOSY_LINE_H - DOSY_FONT_Y_OFFSET;
            dis->setCursor(DOSY_START_X, y + fontOffset);
            dis->print(text);
            dUChange = true;
        }

        // Erase line
        void erase_line(int16_t y)
        {
            dis->fillRect(DOSY_START_X, y, DOSY_LINE_W, DOSY_LINE_H, SCWhite);
        }

        // Clear display
        void clear()
        {
            dis->fillScreen(SCWhite);
        }

        // Check if weather is available
        bool weather_available()
        {
        #if WEATHER_INFO
            // Only check once per hour to save battery
            if (rM.dosy.lastHourWeatherCheck != timeRTCLocal.Hour)
            {
                rM.dosy.lastHourWeatherCheck = timeRTCLocal.Hour;
                OM_OneHourWeather wData = weatherGetDataHourly(WEATHER_WATCHFACE_HOUR_OFFSET);
                rM.dosy.weatherAvailable = wData.fine;
            }
            return rM.dosy.weatherAvailable;
        #else
            return false;
        #endif
        }

        // Get name of weather code
        String get_weather_code_name(uint8_t weather_code)
        {
            switch (weather_code)
            {
            case 0:
                return "Clear Sky";
            case 1:
                return "Mostly Clear";
            case 2:
                return "Partly Cloudy";
            case 3:
                return "Overcast";
            case 45:
                return "Foggy";
            case 48:
                return "Heavy Fog";
            case 51:
                return "Light Drizzle";
            case 53:
                return "Drizzling";
            case 55:
                return "Heavy Drizzle";
            case 56:
                return "Light Freezing";
            case 57:
                return "Heavy Freezing";
            case 61:
                return "Light Rain";
            case 63:
                return "Raining";
            case 65:
                return "Heavy Rain";
            case 66:
                return "Freezing Rain";
            case 67:
                return "Freezing Hard";
            case 71:
                return "Light Snow";
            case 73:
                return "Snowing";
            case 75:
                return "Heavy Snow";
            case 77:
                return "Snow Grains";
            case 80:
                return "Light Showers";
            case 81:
                return "Showering";
            case 82:
                return "Heavy Showers";
            case 85:
                return "Snow Showers";
            case 86:
                return "Heavy Snow";
            case 95:
                return "Thunderstorm";
            case 96:
                return "Light Hail";
            case 99:
                return "Heavy Hail";
            default:
                return "Unknown";
            }
        }

        // Helper function for draw battery bar with %
        String get_battery_str(uint8_t percentage)
        {
            String battBar = "[";

            int segments;
            if (percentage == 0)
            {
                segments = 0;
            }
            else
            {
                segments = min(5, (percentage + 19) / 20);
            }

            for (int i = 0; i < 5; i++)
            {
                battBar += (i < segments) ? "=" : " ";
            }
            battBar += "] " + String(percentage) + "%";
            return battBar;
        }

        // Helper function for draw Date
        String get_date_str(tmElements_t time_elem)
        {
            String year_str = String((tmYearToCalendar(time_elem.Year)));
            if (year_str.length() == 1)
            {
                year_str = "0" + year_str;
            }

            String month_str = String(time_elem.Month);
            if (month_str.length() == 1)
            {
                month_str = "0" + month_str;
            }

            String day_str = String(time_elem.Day);
            if (day_str.length() == 1)
            {
                day_str = "0" + day_str;
            }

            const char *day_names[] = LANGUAGE_DAY_NAMES;
            int array_idx = time_elem.Wday - 1;
            
            String day_name = String(day_names[array_idx]);
            day_name.toLowerCase();

            return year_str + "-" + month_str + "-" + day_str + " (" + day_name + ")";
        }

        // Helper function for draw Time
        String get_time_str(tmElements_t time_elem)
        {   
        #if WATCHFACE_12H
            int hour12 = time_elem.Hour;
            bool isPM = (hour12 >= 12);
            if (hour12 == 0)
            {
                hour12 = 12;
            }
            else if (hour12 > 12)
            {
                hour12 -= 12;
            }

            String hour_str = String(hour12);
            if (hour_str.length() == 1)
            {
                hour_str = "0" + hour_str;
            }

            String minute_str = String(time_elem.Minute);
            if (minute_str.length() == 1)
            {
                minute_str = "0" + minute_str;
            }

            String ampm_str = isPM ? "PM" : "AM";

            return hour_str + ":" + minute_str + " " + ampm_str;
        #else
            return getHourMinute(timeRTCLocal);
        #endif
        }

        // Draw static prints.
        void draw_static_texts()
        {
            print_text("Welcome to Dosy!", DOSY_HEADER_NAME_Y);
            print_text("All systems ready.", DOSY_HEADER_SYS_Y);
            print_text("DS:~$ date", DOSY_DATE_CMD_Y);
            print_text("DS:~$ curl wttr.in", DOSY_WEATHER_CMD_Y);
            print_text("DS:~$ acpi -b", DOSY_BATT_CMD_Y);
        }

        // Just draw time. RIGHT NOW.
        void draw_time()
        {
            String new_str = get_time_str(timeRTCLocal);
            
            erase_line(DOSY_TIME_RESULT_Y);
            print_text(new_str, DOSY_TIME_RESULT_Y);
        }

        // Just draw date. RIGHT NOW.
        void draw_date()
        {
            String new_str = get_date_str(timeRTCLocal);

            erase_line(DOSY_DATE_RESULT_Y);
            print_text(new_str, DOSY_DATE_RESULT_Y);
        }

        // Just draw weather. RIGHT NOW.
        void draw_weather()
        {
        #if WEATHER_INFO
            if (weather_available())
            {
                OM_OneHourWeather wData = weatherGetDataHourly(WEATHER_WATCHFACE_HOUR_OFFSET);

                int16_t tmp_int = int16_t(round(wData.temp));
                uint8_t weather_code = wData.weather_code;

                String temp_str = formatTemperature(wData.temp);
                String condition = get_weather_code_name(wData.weather_code);
                String weather_result = condition + " " + temp_str;

                erase_line(DOSY_WEATHER_RESULT_Y);
                print_text(weather_result, DOSY_WEATHER_RESULT_Y);
                
                rM.dosy.lastTemp = tmp_int;
                rM.dosy.lastCondition = weather_code;
            }
            else
            {
                erase_line(DOSY_WEATHER_RESULT_Y);
                print_text("NO WEATHER!", DOSY_WEATHER_RESULT_Y);
            }
        #else
            erase_line(DOSY_WEATHER_RESULT_Y);
            print_text("NO WEATHER!", DOSY_WEATHER_RESULT_Y);
        #endif
        }

        // Just draw battery. RIGHT NOW.
        void draw_battery()
        {
            String batt_bar = get_battery_str(rM.batteryPercantageWF);

            erase_line(DOSY_BATT_RESULT_Y);
            print_text(batt_bar, DOSY_BATT_RESULT_Y);
        }

        // Draw dynamic time Line 
        void draw_dynamic_time()
        {
            String old_str = get_time_str(rM.wFTime);
            String new_str = get_time_str(timeRTCLocal);

            if (old_str != new_str)
            {
                erase_line(DOSY_TIME_RESULT_Y);
                print_text(new_str, DOSY_TIME_RESULT_Y);
            }
        }
        
        // Draw dynamic weather line
        void draw_dynamic_weather()
        {
        #if WEATHER_INFO
            if (weather_available())
            {
                OM_OneHourWeather wData = weatherGetDataHourly(WEATHER_WATCHFACE_HOUR_OFFSET);
                
                int16_t tmp_int = int16_t(round(wData.temp));
                uint8_t weather_code = wData.weather_code;

                String temp_str = formatTemperature(wData.temp);
                String condition = get_weather_code_name(wData.weather_code);
                String weather_result = condition + " " + temp_str;
                if (rM.dosy.lastCondition != weather_code || rM.dosy.lastTemp != tmp_int)
                {
                    erase_line(DOSY_WEATHER_RESULT_Y);
                    print_text(weather_result, DOSY_WEATHER_RESULT_Y);
                
                    rM.dosy.lastTemp = tmp_int;
                    rM.dosy.lastCondition = weather_code;
                }
            }
            else
            {
                erase_line(DOSY_WEATHER_RESULT_Y);
                print_text("NO WEATHER!", DOSY_WEATHER_RESULT_Y);
            }
        #else
            erase_line(DOSY_WEATHER_RESULT_Y);
            print_text("NO WEATHER!", DOSY_WEATHER_RESULT_Y);
        #endif
        }
    }

    // Functions for watchfaceDefOne

    void drawTimeBeforeApply()
    {
        draw_dynamic_time();
    }

    void drawTimeAfterApply(bool forceDraw)
    {
        if (forceDraw)
        {
            draw_static_texts();
            draw_weather();
        }
        else
        {
            draw_dynamic_weather();
        }
    }

    void showTimeFull()
    {
        draw_time();
    }

    void initWatchface()
    {
        readRTC();

        // Reset all cache
        rM.dosy.lastHourWeatherCheck = 255;
        rM.dosy.lastTemp = 0;
        rM.dosy.lastCondition = 255;

        // Draw all
        clear();
        draw_static_texts();
        draw_time();
        draw_date();
        draw_weather();
        draw_battery();
    }

    void drawDay()
    {
        draw_date();
    }

    void drawBattery()
    {
        draw_battery();
    }
}

const watchfaceDefOne dosyDef = {
    .drawTimeBeforeApply = DosySpace::drawTimeBeforeApply,
    .drawTimeAfterApply = DosySpace::drawTimeAfterApply,
    .drawDay = DosySpace::drawDay,
    .drawMonth = []() {}, // Month has already been drawn by drawDay
    .showTimeFull = DosySpace::showTimeFull,
    .initWatchface = DosySpace::initWatchface,
    .drawBattery = DosySpace::drawBattery,
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
    .isModuleEngaged = []()
    { return false; },
    .lpCoreScreenPrepareCustom = NULL,
};
#endif
