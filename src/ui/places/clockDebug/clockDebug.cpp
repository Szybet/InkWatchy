#include "clockDebug.h"
#include "localization.h"

#if DEBUG_MENUS

uint16_t timeClockHeight;

String zB(String str)
{
    while (str.length() < 2)
    {
        str = "0" + str;
    }
    return str;
}

String getClockPrecise()
{
    return zB(String(timeRTCLocal.Hour)) + ":" + zB(String(timeRTCLocal.Minute)) + ":" + zB(String(timeRTCLocal.Second));
}

String timeSince(int64_t unixTime)
{
    int64_t currentTime = getUnixTime(timeRTCUTC0);
    int64_t elapsedTime = currentTime - unixTime;

    int64_t days = elapsedTime / (24 * 3600);
    elapsedTime %= (24 * 3600);
    int64_t hours = elapsedTime / 3600;
    elapsedTime %= 3600;
    int64_t minutes = elapsedTime / 60;
    int64_t seconds = elapsedTime % 60;

    String output = String(days) + "d " + String(hours) + "h " + String(minutes) + "m " + String(seconds) + "s";
    return output;
}

void initClockDebug()
{
    uint16_t h;
    setFont(&FreeSansBold9pt7b);
    setTextSize(1);
    dis->setCursor(0, 1);
    String menuName = DEBUG_MENU_CLOCK;
    debugLog(menuName);
    getTextBounds(menuName, NULL, NULL, NULL, &h);
    if (containsBelowChar(menuName) == true)
    {
        h = h + 2;
    }
    maxHeight = h;
    uint16_t currentHeight = maxHeight;
    dis->setCursor(0, currentHeight - 3);
    dis->print(menuName);

    dis->fillRect(0, currentHeight, dis->width(), 3, GxEPD_BLACK);
    currentHeight = currentHeight + maxHeight;

    timeClockHeight = currentHeight;
    readRTC();
    centerText(getClockPrecise(), &currentHeight);

    writeLine(DEBUG_CLOCK_DRIFT_SYNCS, 0, &currentHeight);
    writeLine(fsGetString(CONF_SECONDS_DRIFT, DEBUG_CLOCK_NOT_AVAILABLE), 0, &currentHeight);
    {
        writeLine(DEBUG_CLOCK_LAST_SYNC, 0, &currentHeight);
        String lastSync = fsGetString(CONF_UNIX_LAST_SYNC, "");
        debugLog("Last sync time: " + lastSync);
        if (lastSync != "")
        {
            lastSync = timeSince(lastSync.toInt());
        }
        else
        {
            lastSync = DEBUG_CLOCK_NOT_AVAILABLE;
        }
        writeLine(lastSync, 0, &currentHeight);
    }
    {
        writeLine(DEBUG_CLOCK_PREVIOUS_SYNC, 0, &currentHeight);
        String lastSync = fsGetString(CONF_UNIX_PREVIOUS_SYNC, "");
        debugLog("Previous sync time: " + lastSync);
        if (lastSync != "")
        {
            lastSync = timeSince(lastSync.toInt());
        }
        else
        {
            lastSync = DEBUG_CLOCK_NOT_AVAILABLE;
        }
        writeLine(lastSync, 0, &currentHeight);
    }
    {
        writeLine(DEBUG_CLOCK_LAST_CHARGE, 0, &currentHeight);
        String lastSync = fsGetString(CONF_UNIX_LAST_CHARGE, "");
        debugLog("Last charge time: " + lastSync);
        if (lastSync != "")
        {
            lastSync = timeSince(lastSync.toInt());
        }
        else
        {
            lastSync = DEBUG_CLOCK_NOT_AVAILABLE;
        }
        writeLine(lastSync, 0, &currentHeight);
    }

    resetSleepDelay(SLEEP_EVERY_MS);
    disUp(true);
}

uint8_t savedSeconds = 0;
void loopClockDebug()
{
    readRTC();
    if (savedSeconds != timeRTCLocal.Second)
    {
        savedSeconds = timeRTCLocal.Second;
        // debugLog("timeRTCLocal.Second: " + String(timeRTCLocal.Second));
        writeTextCenterReplaceBack(getClockPrecise(), timeClockHeight);
        dUChange = true;
    }

    buttonState btn = useButton();
    if(btn == Up) {
        fsRemoveFile("/conf/" + String(CONF_SECONDS_DRIFT));
        fsRemoveFile("/conf/" + String(CONF_UNIX_LAST_SYNC));
        fsRemoveFile("/conf/" + String(CONF_UNIX_PREVIOUS_SYNC));
        fsRemoveFile("/conf/" + String(CONF_UNIX_LAST_CHARGE));
        dis->fillScreen(GxEPD_WHITE);
        initClockDebug();
    }

    resetSleepDelay();
    delayTask(100);
    disUp();
}

#endif