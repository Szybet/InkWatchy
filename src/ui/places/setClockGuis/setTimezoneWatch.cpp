#include "setTimezoneWatch.h"

#if SET_CLOCK_GUI
#include "rtcMem.h"

#define TZ_COUNT 27    // UTC-12 to UTC+14
#define TZ_SELECTED 12 // Default: UTC+0

int tzSelected = TZ_SELECTED;

void timezoneClicked()
{
    String selected = lastMenuSelected;
    selected.remove(0, 3);
    int offset = selected.toInt();
    debugLog("Selected UTC is: " + String(offset) + " bare UTC is: " + lastMenuSelected + " selected is: " + selected);

    setRTCTimeZoneByUtcOffset(offset);

    // tzSelected = offset + 12;
    initSetTimezoneUtcWatch();
}

void initSetTimezoneUtcWatch()
{
    /*
    tzSelected = TZ_SELECTED;
    String timezone = String(rM.posixTimeZone);
    if(strlen(rM.posixTimeZone) <= 6 && timezone.substring(0, 3) == "GMT") {
        timezone.remove(0, 3);
        int offset = timezone.toInt() * -1;
        debugLog("Init offset is: " + String(offset));
        tzSelected = offset + 12;
    }
    if(tzSelected < 0 || tzSelected >= TZ_COUNT) {
        tzSelected = TZ_SELECTED;
    }
    */
    int timezoneOffsetConv = ((timeZoneOffset / 60) / 60) * -1;
    tzSelected = timezoneOffsetConv + 12;

    entryMenu tzEntries[TZ_COUNT];
    for (int i = 0; i < TZ_COUNT; i++)
    {
        int offset = i - 12;
        String label;
        if (offset >= 0)
        {
            label = String("UTC+") + String(offset);
        }
        else
        {
            label = String("UTC") + String(offset);
        }

        ImageDef *choosedImage = &emptyImgPack;
        if (tzSelected == i)
        {
            choosedImage = getImg("cross");
        }

        tzEntries[i].text = label;
        tzEntries[i].image = choosedImage;
        tzEntries[i].function = timezoneClicked;
    }

    currentMenuItem = tzSelected;
    initMenu(tzEntries, TZ_COUNT, SETCLOCK_SELECT_TIMEZONE);
}

void exitSetTimezoneUtcWatch()
{
    // setRTCTimeZoneByUtcOffset already reads it again
    // readRTC();
}

static const char *tzHeaders[] = {
    "AFRICA", "AMERICA", "ANTARCTICA", "ARCTIC", "ASIA", "ATLANTIC",
    "AUSTRALIA", "ETC", "EUROPE", "INDIAN", "PACIFIC"};
#define TZ_HEADER_COUNT (sizeof(tzHeaders) / sizeof(tzHeaders[0]))

void initSetTimezoneContinentWatch()
{
    entryMenu tzEntries[TZ_HEADER_COUNT];

    for (int i = 0; i < TZ_HEADER_COUNT; i++)
    {
        tzEntries[i].text = String(tzHeaders[i]);
        tzEntries[i].image = &emptyImgPack;
        tzEntries[i].function = switchSetTimezoneCityWatchPlace;
    }

    initMenu(tzEntries, TZ_HEADER_COUNT, SETCLOCK_SELECT_TIMEZONE);
}

String *selectedHeader = NULL;
String *zonesToClear = NULL;
struct ZoneList
{
    String *zones;
    int count;
};

void setTimezoneCity()
{
    String finalOlson = *selectedHeader + "/" + lastMenuSelected;
    debugLog("Final olson is: " + finalOlson);
    Olson2POSIX timezoneMagic;
    String posix = timezoneMagic.getPOSIX(finalOlson);
    debugLog("Final posix: " + String(posix));
    uint8_t posixLength = posix.length();
    if (posixLength < POSIX_TIMEZONE_MAX_LENGTH)
    {
        strncpy(rM.posixTimeZone, posix.c_str(), posix.length());
        rM.posixTimeZone[posix.length()] = '\0';
    }
    else
    {
        debugLog("How is your posix timezone so long? BUG BUG contact me");
    }
    readRTC();
    setButton(LongBack);
}

ZoneList listZonesForHeader(String header)
{
    int hlen = header.length();
    int count = 0;

    for (int i = 0; i < sOLSON.length(); i++)
        if (sOLSON[i] == '|')
        {
            int start = i + 1;
            if (sOLSON.startsWith(header, start) && sOLSON[start + hlen] == '/')
                count++;
        }

    String *zones = new String[count];
    zonesToClear = zones;
    int idx = 0;

    for (int i = 0; i < sOLSON.length() && idx < count; i++)
        if (sOLSON[i] == '|')
        {
            int start = i + 1;
            if (sOLSON.startsWith(header, start) && sOLSON[start + hlen] == '/')
            {
                int end = sOLSON.indexOf('|', start);
                zones[idx++] = sOLSON.substring(start + hlen + 1, end);
                i = end;
            }
        }

    return {zones, count};
}

void initSetTimezoneCityWatch()
{
    selectedHeader = new String(lastMenuSelected);
    debugLog("Selected header: " + String(*selectedHeader));
    ZoneList zones = listZonesForHeader(*selectedHeader);
    /*
    for (int i = 0; i < zones.count; i++)
    {
        debugLog("Zone " + String(i) + " is: " + zones.zones[i]);
    }
    */

    entryMenu menuEntries[zones.count];

    for (int i = 0; i < zones.count; i++)
    {
        menuEntries[i].text = zones.zones[i];
        menuEntries[i].image = &emptyImgPack;
        menuEntries[i].function = setTimezoneCity;
    }

    initMenu(menuEntries, zones.count, SETCLOCK_SELECT_TIMEZONE);
}

void exitTimezoneCityWatch()
{
    if (zonesToClear != NULL)
    {
        delete[] zonesToClear;
        zonesToClear = NULL;
    }
    if (selectedHeader != NULL)
    {
        delete selectedHeader;
        selectedHeader = NULL;
    }
}

#endif
