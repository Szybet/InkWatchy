#include "clockDebug.h"

#if DEBUG_MENUS
#include "rtcMem.h"

uint16_t timeClockLine;

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

void cleanClockFiles() {
    fsRemoveFile("/conf/" + String(CONF_SECONDS_DRIFT));
    fsRemoveFile("/conf/" + String(CONF_UNIX_LAST_SYNC));
    fsRemoveFile("/conf/" + String(CONF_UNIX_PREVIOUS_SYNC));
    fsRemoveFile("/conf/" + String(CONF_UNIX_LAST_CHARGE));
    slintExit();
    initClockDebug();
}

void initClockDebug()
{
    init_general_page(5);
    general_page_set_title(DEBUG_MENU_CLOCK);
    genpage_set_center();

    GeneralPageButton button = GeneralPageButton{DEBUG_CLOCK_REMOVE_FILES, cleanClockFiles};
    general_page_set_buttons(&button, 1);

    readRTC();
    timeClockLine = genpage_add(getClockPrecise().c_str());

    genpage_add(String("Timezone:").c_str());
    genpage_add(String(rM.posixTimeZone).c_str());
    
    if(strlen(TIMEZONE_OLSON) != 0) {
        genpage_add(String(String("Olson timezone: ") + String(TIMEZONE_OLSON)).c_str());
    }
    
    genpage_add(DEBUG_CLOCK_DRIFT_SYNCS);
    genpage_add(fsGetString(CONF_SECONDS_DRIFT, DEBUG_CLOCK_NOT_AVAILABLE).c_str());
    {
        genpage_add(DEBUG_CLOCK_LAST_SYNC);
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
        genpage_add(lastSync.c_str());
    }
    {
        genpage_add(DEBUG_CLOCK_PREVIOUS_SYNC);
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
        genpage_add(lastSync.c_str());
    }
    {
        genpage_add(DEBUG_CLOCK_LAST_CHARGE);
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
        genpage_add(lastSync.c_str());
    }
    general_page_set_main();
}

uint8_t savedSeconds = 0;
void loopClockDebug()
{
    resetSleepDelay();
    
    if(genpage_is_menu() == false) {
        readRTC();
        if (savedSeconds != timeRTCLocal.Second)
        {
            savedSeconds = timeRTCLocal.Second;
            // debugLog("timeRTCLocal.Second: " + String(timeRTCLocal.Second));
            genpage_change(getClockPrecise().c_str(), timeClockLine);
        }
    }

    general_page_set_main();
    slint_loop();
}

#endif