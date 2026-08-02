#include "cyber.h"
#include "cyberHelpers.h"
#include "cyberWeather.h"
#include "rtcMem.h"

#if WATCHFACE_CYBER


void clearTimeCyber()
{
    return;
}

// Formats time string according to 12H/24H setting from config.h
String getCyberLocalizedTimeString(tmElements_t timeEl)
{
#if WATCHFACE_12H
    String time24 = getHourMinute(timeEl);
    return convertTo12HourFormat(time24);
#else
    return getHourMinute(timeEl);
#endif
}

static void cDrawTimeDigits() 
{
    String newTime = getCyberLocalizedTimeString(timeRTCLocal);
    writeImageN(48,34, getImg("cyber/"+String(newTime[0])));
    writeImageN(83,34, getImg("cyber/"+String(newTime[1])));
    writeImageN(126,34, getImg("cyber/"+String(newTime[3])));
    writeImageN(161,34, getImg("cyber/"+String(newTime[4])));
}
static void drawTimeBeforeApply()
{
    cDrawTimeDigits();
    uint16_t weatherMinutes = timeRTCLocal.Minute + (60 * timeRTCLocal.Hour);
    if (abs(rM.cyber.weatherMinutes - weatherMinutes) > 25)
    {
        rM.cyber.weatherMinutes = weatherMinutes;
        cyberDrawWeather();
    }
    cyberDrawMoon();
}

static void showTimeFull()
{
    cDrawTimeDigits();
}

static void initWatchface()
{
    writeImageN(0, 0, getImg("cyber/watchface"));
    
    //drawPosMarkerCyber();
}


#define BATTERY_START_X 22
#define BATTERY_END_X 176
#define BATTERY_START_Y 5
#define BATTERY_HEIGHT 7
#define BATTERY_CHUNK_X 22
#define BATTERY_CHUNK_Y 17
static void drawBattery()
{
    uint16_t maxWidth = BATTERY_END_X - BATTERY_START_X;
    uint16_t battPct = rM.bat.percentage;
    // Clear
    uint16_t lineY = BATTERY_START_Y + BATTERY_HEIGHT;
    dis->fillRect(BATTERY_START_X, BATTERY_START_Y, maxWidth, BATTERY_HEIGHT, SCWhite);
    dis->drawLine(BATTERY_START_X, lineY, BATTERY_END_X-1, lineY, SCWhite);
    dis->drawLine(BATTERY_START_X, lineY+1, BATTERY_END_X-2, lineY+1, SCWhite);
    dis->drawLine(BATTERY_START_X, lineY+2, BATTERY_END_X-3, lineY+2, SCWhite);
    dis->fillRect(177, 0, 23, 16, SCWhite);
    dis->fillRect(169, 16, 31, 11, SCWhite);
    dis->fillRect(5, 1, 11, 15, SCWhite);
    writeImageN(21, 0, getImg("cyber/batterytop"));

    uint16_t width = map(battPct, 0, 100, BATTERY_START_X, maxWidth);
    uint16_t endX = BATTERY_START_X+width; 
    dis->fillRect(BATTERY_START_X, BATTERY_START_Y, width, BATTERY_HEIGHT, SCBlack);
    dis->drawLine(BATTERY_START_X, lineY, endX-1, lineY, SCBlack);
    dis->drawLine(BATTERY_START_X, lineY+1, endX-2, lineY+1, SCBlack);
    dis->drawLine(BATTERY_START_X, lineY+2, endX-3, lineY+2, SCBlack);

    // Numeric Level
    setTextSize(1);
    setFont(getFont("cyber/RajdhaniB8"));
    dis->setCursor(198-cyberRightTextXOffset(String(battPct)),24);
    dis->print(battPct);
    
    //a little flair
    String rnd = String(betterRandom(0,9));
    dis->setCursor(10-cyberCenterTextXOffset(rnd), 13);
    dis->print(rnd);

    // Voltage as text
    String voltage = String(rM.bat.curV);
    while (voltage.length() > 4)
    {
        voltage.remove(voltage.length() - 1);
    }
    setFont(getFont("cyber/RajdhaniR6"));
    dis->setCursor(199-cyberRightTextXOffset(voltage),10);
    dis->print(voltage);

    // calculate 20% segments
    float_t subPct = 0;
    if(battPct > 80) {
        subPct = battPct - 80;
        writeImageN(BATTERY_CHUNK_X, BATTERY_CHUNK_Y ,getImg("cyber/battchunkfull"));
    } else if(battPct > 60 && battPct <= 80) {
        subPct = battPct - 60;
        writeImageN(BATTERY_CHUNK_X, BATTERY_CHUNK_Y ,getImg("cyber/battchunk80"));
    }
    else if(battPct > 40 && battPct <= 60) {
        subPct = battPct - 40;
        writeImageN(BATTERY_CHUNK_X, BATTERY_CHUNK_Y ,getImg("cyber/battchunk60"));
    }
    else if(battPct > 20 && battPct <= 40) {
        subPct = battPct - 20;
        writeImageN(BATTERY_CHUNK_X, BATTERY_CHUNK_Y ,getImg("cyber/battchunk40"));
    } else {
        subPct = battPct;
        writeImageN(BATTERY_CHUNK_X, BATTERY_CHUNK_Y ,getImg("cyber/battchunk20"));
    }
    
    float_t sP = (1-(subPct/20)) * maxWidth;
    dis->fillRect(BATTERY_END_X+2-sP, 0, sP, 3, SCWhite);
}

static void drawTimeAfterApply(bool forceDraw)
{
    uint16_t weatherMinutes = timeRTCLocal.Minute + (60 * timeRTCLocal.Hour);
    if (abs(rM.cyber.weatherMinutes - weatherMinutes) > 25 || forceDraw == true)
    {
        rM.cyber.weatherMinutes = weatherMinutes;
        cyberDrawWeather();
    }
    if (rM.cyber.dayTime != timeRTCLocal.Day || forceDraw == true)
    {
        rM.cyber.dayTime = timeRTCLocal.Day;
        cyberDrawMoon();
    }

    // WIFI Status
    if(WiFi.status() == WL_CONNECTED) {
        writeImageN(0,69,getImg("cyber/wifion"));
    } else {
        writeImageN(0,69,getImg("cyber/wifioff"));
    
    }
}

String getDayByIndexCyber(int dayOfWeek, int offset = 0)
{
    
    // static const String dayNames[] = {
    //     "Sunday",
    //     "Monday",
    //     "Tuesday",
    //     "Wednesday",
    //     "Thursday",
    //     "Friday",
    //     "Saturday",
    // };

    // Apply offset and wrap around
    int dayIndex = (dayOfWeek + offset + 6) % 7;

    if (dayIndex >= 0 && dayIndex < 7)
    {
        return getLocalizedDayByIndex(dayIndex, offset);
    }
    return "---";
}

static void drawDay()
{
    // Weekday
    dis->fillRect(42,75,149,11,SCWhite);
    setTextSize(1);
    dis->setCursor(43,82);
    setFont(getFont("cyber/RajdhaniR6"));
    dis->print(getDayByIndexCyber(timeRTCLocal.Wday));
    
    // Date
    String dateString = "20" + String((timeRTCLocal.Year + 70) % 100) + ".";

    uint8_t month = rM.wFTime.Month + 1;
    debugLog("MONTH "+String(month));
    if(month < 10) {
        dateString += "0";
    }
    dateString += String(month) + ".";
    if (rM.wFTime.Day < 10)
    {
        dateString += "0";
    }
    dateString += String(rM.wFTime.Day);
    uint16_t offset = cyberRightTextXOffset(dateString);
    dis->setCursor(188-offset, 82);
    dis->print(dateString);
}

const watchfaceDefOne cyberDefOne = {
    .drawTimeBeforeApply = drawTimeBeforeApply,
    .drawTimeAfterApply = drawTimeAfterApply,
    .drawDay = drawDay,
    .drawMonth = []() {},
    .showTimeFull = showTimeFull,
    .initWatchface = initWatchface,
    .drawBattery = drawBattery,
    .manageInput = cyberManageInput,

    .watchfaceModules = true,
    .watchfaceModSquare = {.size{.w = 177, .h = 37}, .cord{.x = 7, .y = 160}},
    .someDrawingSquare = {.size{.w = 200, .h = 139}, .cord{.x = 0, .y = 61}},
    .isModuleEngaged = []()
    {
        if (rM.cyber.watchfacePos == MODULE_ENG_POS && rM.cyber.positionEngaged == true)
        {
            return true;
        }
        return false; },
    .lpCoreScreenPrepareCustom = clearTimeCyber,
    .lpCoreFile = LP_CORE_FILE_DEFAULT,
};

#endif
