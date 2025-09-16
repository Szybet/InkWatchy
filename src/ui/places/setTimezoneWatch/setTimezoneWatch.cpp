#include "setTimezoneWatch.h"
#include "rtc.h"
#include "../../reUse/menu/menu.h"

// Menu entry for manager registration
entryMenu setTimezoneWatchEntry = {
    "Timezone"};

// Timezone entries
const int tzCount = 27; // UTC-12 to UTC+14
static entryMenu tzEntries[tzCount];

// Selected index (index in tzEntries: 0 => UTC-12, 12 => UTC+0, 26 => UTC+14)
static int tzSelected = 12; // Default: UTC+0

// Forward declarations
static void timezoneClicked();
static void initTzEntries();

// -------------------------------------------------------------------
// Callback invoked when Menu button is pressed on a highlighted item
// This follows the same pattern as watchfaceClicked(): use lastMenuSelected.
// -------------------------------------------------------------------
static void timezoneClicked()
{
    // lastMenuSelected is set by menu.cpp right before calling this function.
    for (int i = 0; i < tzCount; i++)
    {
        if (tzEntries[i].text == lastMenuSelected)
        {
            // Update selection
            tzSelected = i;

            // Compute offset and apply immediately
            int offsetHours = tzSelected - 12;
            timeZoneOffset = offsetHours * 3600; // seconds
            setRTCTimeZoneByUtcOffset(offsetHours);
            readRTC();

            // Rebuild entries so the cross image moves to the newly selected entry
            initTzEntries();
            initMenu(tzEntries, tzCount, "Select Timezone", 1);
            return;
        }
    }
}

// -------------------------------------------------------------------
// Build tzEntries[]: text is label "UTC±N", image is cross for selected item
// function is timezoneClicked for each entry (so Menu presses call it)
// -------------------------------------------------------------------
static void initTzEntries()
{
    int currentOffsetHours = -timeZoneOffset / 3600;

    for (int i = 0; i < tzCount; i++)
    {
        int offset = i - 12; // Convert index to UTC offset
        String label = (offset >= 0) ? "UTC+" + String(offset) : "UTC" + String(offset);

        // Choose image: cross for currently selected timezone, empty otherwise
        ImageDef *choosedImage = &emptyImgPack;
        if (tzSelected == i)
        {
            choosedImage = getImg("cross");
        }

        tzEntries[i].text = label;
        tzEntries[i].image = choosedImage;
        tzEntries[i].function = timezoneClicked;
    }
}

// -------------------------------------------------------------------
// Lifecycle functions
// -------------------------------------------------------------------
void initSetTimezoneWatch()
{
    initTzEntries();

    // Ensure tzSelected is in range
    tzSelected = constrain(tzSelected, 0, tzCount - 1);

    // Initialize menu; initMenu will use global currentMenuItem as the starting highlight
    initMenu(tzEntries, tzCount, "Select Timezone", 1);

    // put highlight on the currently selected item by default
    data.currentButton = tzSelected;
}

void loopSetTimezoneWatch()
{
    // Do NOT call useButton() here — loopMenu() handles physical button events and will
    // call timezoneClicked() when Menu is pressed.
    loopMenu();
}

void exitSetTimezoneWatch()
{
    // Back just exits. All actual timezone changes are already applied immediately in timezoneClicked().
    readRTC();
    slintExit();
}
