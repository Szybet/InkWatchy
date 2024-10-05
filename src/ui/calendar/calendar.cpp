#include "calendar.h"

#if CALENDAR

#define DATE_BYTES 11 // unix is 10 bytes + \n

void switchCalendarMenu()
{
    generalSwitch(calendar);
}

JsonDocument getCalendarJson(String date)
{
    String dateFile = fsGetString(date, "", "/calendar/");
    // debugLog("Date file: " + dateFile);

    JsonDocument jsonDoc;
    DeserializationError error = deserializeJson(jsonDoc, dateFile);

    if (error.code() != DeserializationError::Ok)
    {
        debugLog("Failed to parse json");
        // Welp
    }

    // JsonArray array = jsonDoc.as<JsonArray>();

    // String test = array[0]["name"].as<String>();
    // debugLog("Test: " + test);
    // debugLog("Array size: " + String(array.size()));
    return jsonDoc;
}

#define STR_DATE_BYTES 11 // + plus \0
char datePathSaved[STR_DATE_BYTES] = {0};
void initCalendar()
{
    debugLog("lastMenuSelected: " + lastMenuSelected);
    // debugLog("lastMenuSelected.indexOf dot: " + String(lastMenuSelected.indexOf(".")));
    // debugLog("lastMenuSelected.lastIndexOf: " + String(lastMenuSelected.lastIndexOf(".")));
    // debugLog("lastMenuSelected.indexOf: " + String(lastMenuSelected.indexOf(" ")));

    if (lastMenuSelected.indexOf(".") == 2 && lastMenuSelected.lastIndexOf(".") == 5 && lastMenuSelected.indexOf(" ") == 10)
    {
        debugLog("Previous menu item was a date");
        String date = lastMenuSelected.substring(0, 10);
        date.toCharArray(datePathSaved, STR_DATE_BYTES); // Save
        lastMenuSelected = "";
        JsonDocument doc = getCalendarJson(date);
        JsonArray array = doc.as<JsonArray>();
        debugLog("Array size outside: " + String(array.size()));
        int arraySize = array.size();
        entryMenu buttons[arraySize];
        for (int i = 0; i < arraySize; i++)
        {

            String name = array[i]["name"].as<String>();
            uint64_t startUnix = array[i]["start_time"].as<String>().toInt();
            String start = getHourMinuteUnix(startUnix);
            uint64_t endUnix = array[i]["end_time"].as<String>().toInt();
            String end = getHourMinuteUnix(endUnix);
            String info = start + "-" + end + " " + String(i) + ": " + name;
            debugLog("Final info: " + info);
            buttons[i] = {info, &emptyImgPack, initCalendar};
        }
        initMenu(buttons, arraySize, date, 1);
        generalSwitch(calendarMenu);
    }
    else if (lastMenuSelected.indexOf(":") == 2 && lastMenuSelected.indexOf("-") == 5)
    {
        debugLog("Got back from selecting a event!");
        // 09:15-10:50 0:
        int id = 0;
        int space = lastMenuSelected.indexOf(" ");
        for (int8_t i = 3; i >= 0; i--)
        {
            String maybeId = lastMenuSelected.substring(space, space + i);
            if (maybeId.indexOf(":") == -1)
            {
                id = maybeId.toInt();
                break;
            }
        }
        lastMenuSelected = "";
        debugLog("Found id: " + String(id));
        String retrPath = String(datePathSaved);
        debugLog("retrPath: " + String(retrPath));
        JsonDocument doc = getCalendarJson(retrPath);
        JsonArray array = doc.as<JsonArray>();
        generalSwitch(textDialog);
        String str[1] = {array[id]["description"].as<String>()};
        textPage("", str, 1);
    }
    else
    {
        debugLog("Launching init calendar menu");
        initCalendarMenu();
    }
}

void initCalendarMenu()
{
    generalSwitch(calendarMenu);
    String buf = fsGetString("index.txt", "", "/calendar/");
    if (buf.length() <= 0)
    {
        debugLog("Failed to read index");
        // TODO error
        return;
    }
    int dates = buf.length() / DATE_BYTES;
    debugLog("dates: " + String(dates));
    debugLog("buf.size: " + String(buf.length()));
    entryMenu buttons[dates];
    int64_t currentTime = getUnixTime(timeRTCLocal);
    int64_t smallestDifference = 9223372036854775807;
    int indexOfCurrTime = 0;
    for (int i = 0; i < dates; i++)
    {
        int stringIndex = (i * DATE_BYTES);
        String retrUnix = buf.substring(stringIndex, stringIndex + DATE_BYTES - 1);
        debugLog("Got unix: " + retrUnix);
        int64_t newUnix = retrUnix.toInt();
        int diff = abs(currentTime - smallestDifference);
        if (diff < smallestDifference)
        {
            smallestDifference = diff;
            indexOfCurrTime = stringIndex;
        }
    }
    debugLog("Current time: " + String(currentTime));
    debugLog("indexOfCurrTime: " + String(indexOfCurrTime));
    for (int i = 0; i < dates; i++)
    {
        int stringIndex = indexOfCurrTime + (i * DATE_BYTES);
        if (stringIndex >= buf.length())
        {
            stringIndex = stringIndex - buf.length();
        }
        debugLog("stringIndex: " + String(stringIndex));

        uint64_t unixTime = buf.substring(stringIndex, stringIndex + DATE_BYTES - 1).toInt();
        String dateStr = unixToDate(unixTime) + " " + unixToDayName(unixTime);
        debugLog("dateStr: " + dateStr);

        buttons[i] = {dateStr, &emptyImgPack, initCalendar};
    }
    initMenu(buttons, dates, "Calendar", 1);
}

#endif