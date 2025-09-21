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
    initSetTimezoneWatch();
}

void initSetTimezoneWatch()
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

void exitSetTimezoneWatch()
{
    // setRTCTimeZoneByUtcOffset already reads it again
    // readRTC();
}

#endif