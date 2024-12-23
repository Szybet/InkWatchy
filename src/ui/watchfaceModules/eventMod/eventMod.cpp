#include "eventMod.h"
#include "rtcMem.h"

#if EVENT_MODULE

#define DATE_BYTES 11 // unix is 10 bytes + \n
#define LINE_LIMIT 28

// Makes redrawing turned off for the current day
void wfEventresetValues() {
    rM.currentEventTime = 0;
    rM.currentDay = day(getUnixTime(timeRTCLocal));
}

void wfEventcheckShow(bool *showBool, bool *redrawBool)
{
    *showBool = true;
    if(rM.currentDay == day(getUnixTime(timeRTCLocal))) {
        return;
    } else {
        if(rM.currentEventTime < getUnixTime(timeRTCLocal)) {
            *redrawBool = true;
        }
    }
}

void wfEventrequestShow(buttonState button, bool *showBool)
{
    dUChange = true;
    setFont(getFont("dogicapixel4"));
    setTextSize(1);
    squareInfo modSq = getWatchModuleSquare();
    dis->setCursor(modSq.cord.x, modSq.cord.y + 10);
    if (fsFileExists("/calendar/index.txt") == false)
    {
        dis->print("No calendar data");
        wfEventresetValues();
        return;
    }
    String buf = fsGetString("index.txt", "", "/calendar/");
    if (buf.length() <= 0)
    {
        dis->print("Failed to read index");
        wfEventresetValues();
        return;
    }
    int dates = buf.length() / DATE_BYTES;
    debugLog("dates: " + String(dates));
    debugLog("buf.size: " + String(buf.length()));
    entryMenu buttons[dates];
    int64_t currentTime = getUnixTime(timeRTCLocal);
    debugLog("Current time: " + String(currentTime));
    int64_t smallestDifference = 9223372036854775807;
    int64_t theUnix = 0;
    for (int i = 0; i < dates; i++)
    {
        int stringIndex = (i * DATE_BYTES);
        String retrUnix = buf.substring(stringIndex, stringIndex + DATE_BYTES - 1);
        debugLog("Got unix: " + retrUnix);
        int64_t newUnix = retrUnix.toInt();
        int diff = abs(currentTime - newUnix);
        if (diff < smallestDifference)
        {
            smallestDifference = diff;
            theUnix = newUnix;
        }
    }
    debugLog("Current time: " + String(currentTime));
    if (day(theUnix) != day(currentTime) || month(theUnix) != month(currentTime))
    {
        dis->print("No events today");
        wfEventresetValues();
        return;
    }
    JsonDocument doc = getCalendarJson(unixToDate(theUnix));
    JsonArray array = doc.as<JsonArray>();
    int arraySize = array.size();
    if(array == 0) {
        dis->print("Invalid json?");
        wfEventresetValues();
        return;
    }
    int finalI = -1;
    int smallDiff = 999999;
    uint64_t eventUnix = 0;
    for (int i = 0; i < arraySize; i++)
    {
        uint64_t startUnix = array[i]["start_time"].as<String>().toInt();
        if(startUnix > currentTime) {
            int diff = llabs(startUnix - currentTime);
            if(diff < smallDiff) {
                finalI = i;
                smallDiff = diff;
                eventUnix = startUnix;
            }
        }
    }
    if(finalI == -1) {
        dis->print("No more events");
        wfEventresetValues();
        return;
    }
    String des = array[finalI]["name"].as<String>();
    uint64_t end = array[finalI]["end_time"].as<String>().toInt();

    String btnStr = getHourMinuteUnix(eventUnix) + "-" + getHourMinuteUnix(end) + " " + des;
    debugLog("btnStr: " + btnStr);

    if(btnStr.length() <= LINE_LIMIT) {
        dis->print(btnStr);
    } else {
        String btnStr1 = btnStr.substring(0, LINE_LIMIT);
        //String btnStr2 = btnStr.substring(LINE_LIMIT, btnStr.length());
        //String btnStr2 = btnStr2.substring(0, LINE_LIMIT);
        // Not tested yet
        String btnStr2 = btnStr.substring(LINE_LIMIT, LINE_LIMIT + LINE_LIMIT);
        dis->print(btnStr1);
        dis->setCursor(modSq.cord.x, modSq.cord.y + 10 + 10);
        dis->print(btnStr2);
    }

    String details = array[finalI]["description"].as<String>();
    details.replace(CALENDAR_SPLIT_DESCRIPTION_STRING, ", ");
    details = details.substring(0, LINE_LIMIT);
    dis->setCursor(modSq.cord.x, modSq.cord.y + modSq.size.h - 1);
    dis->print(details);

    // Set the next redraw, kind of
    rM.currentEventTime = eventUnix;
    rM.currentDay = -1;

    // No.
    //drawButton(MODULE_RECT_X + img->bw + 2, MODULE_RECT_Y + 10, btnStr, &emptyImgPack, false, 2, 0, GxEPD_BLACK, GxEPD_WHITE, true, getFont("dogicapixel4"));
}

#endif