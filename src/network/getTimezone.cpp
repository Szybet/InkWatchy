#include "getTimezone.h"

// This should be executed after NTP sync
void syncTimezone()
{   
    // We could here also strlen(posixTimeZone) == 0 but what if we move?
    if (strlen(TIMEZONE_OLSON) == 0 && strlen(TIMEZONE_POSIX) == 0)
    {
        Olson2POSIX timezoneMagic;

        debugLog("Starting getting timezone");
        while(WiFi.status() != WL_CONNECTED) {
            delayTask(700);
            debugLog("Waiting for wifi to get connected status, current status: " + wifiStatus());
        }
        debugLog("So, wifi current status: " + wifiStatus());

        // This is a start of a 20K task, very large - could be smaller
        timezoneMagic.beginOlsonFromWeb();

        while(timezoneMagic.gotOlsonFromWeb() == false && timezoneMagic.getOlsonWebError() == 0) {
            delayTask(700);
            debugLog("Waiting for olson sync to finish...");
        }

        if (timezoneMagic.gotOlsonFromWeb() == true)
        {
            timezoneMagic.endOlsonFromWeb();
            String olson = timezoneMagic.getCurrentOlson();
            debugLog("Got timezone: " + olson);
            timezoneMagic.setOlsonTimeZone(olson);
            String posix = timezoneMagic.getCurrentPOSIX();
            debugLog("Got posix from olson: " + posix);
            uint8_t posixLength = posix.length();
            if (posixLength < POSIX_TIMEZONE_MAX_LENGTH)
            {
                strncpy(posixTimeZone, posix.c_str(), posix.length());
                posixTimeZone[posix.length() - 1] = '\0';
            }
            else
            {
                debugLog("How is your posix timezone so long? BUG BUG contact me");
            }
        }
        else
        {
            debugLog("Failed to get timezone, http code: " + String(timezoneMagic.getOlsonWebError()));
        }
    }
    else
    {
        debugLog("Skipping timezone request, it's already set, somewhere");
    }
    timeZoneApply();
}
