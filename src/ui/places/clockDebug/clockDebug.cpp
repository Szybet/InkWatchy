#include "clockDebug.h"

#if DEBUG_MENUS == 1

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
    display.setCursor(0, 1);
    String menuName = "Debug Menu: Clock";
    debugLog(menuName);
    getTextBounds(menuName, NULL, NULL, NULL, &h);
    if (containsBelowChar(menuName) == true)
    {
        h = h + 2;
    }
    maxHeight = h;
    uint16_t currentHeight = maxHeight;
    display.setCursor(0, currentHeight - 3);
    display.print(menuName);

    display.fillRect(0, currentHeight, display.width(), 3, GxEPD_BLACK);
    currentHeight = currentHeight + maxHeight;

    timeClockHeight = currentHeight;
    readRTC();
    centerText(getClockPrecise(), &currentHeight);

    writeLine("Drift between synces:", 0, &currentHeight);
    writeLine(fsGetString(CONF_SECONDS_DRIFT, "Not available"), 0, &currentHeight);
    {
        writeLine("Last sync:", 0, &currentHeight);
        String lastSync = fsGetString(CONF_UNIX_LAST_SYNC, "");
        debugLog("Last sync time: " + lastSync);
        if (lastSync != "")
        {
            lastSync = timeSince(lastSync.toInt());
        }
        else
        {
            lastSync = "Not available";
        }
        writeLine(lastSync, 0, &currentHeight);
    }
    {
        writeLine("Previous sync:", 0, &currentHeight);
        String lastSync = fsGetString(CONF_UNIX_PREVIOUS_SYNC, "");
        debugLog("Previous sync time: " + lastSync);
        if (lastSync != "")
        {
            lastSync = timeSince(lastSync.toInt());
        }
        else
        {
            lastSync = "Not available";
        }
        writeLine(lastSync, 0, &currentHeight);
    }
    {
        writeLine("Last charge:", 0, &currentHeight);
        String lastSync = fsGetString(CONF_UNIX_LAST_CHARGE, "");
        debugLog("Last charge time: " + lastSync);
        if (lastSync != "")
        {
            lastSync = timeSince(lastSync.toInt());
        }
        else
        {
            lastSync = "Not available";
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
        display.fillScreen(GxEPD_WHITE);
        initClockDebug();
    }

    resetSleepDelay();
    delayTask(100);
    disUp();
}

#endif