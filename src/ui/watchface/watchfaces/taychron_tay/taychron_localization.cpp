#include "taychron_localization.h"
#include "rtcMem.h"

// Formats time string according to 12H/24H setting from config.h
String getTaychronLocalizedTimeString(tmElements_t timeEl) {
#if WATCHFACE_12H
    String time24 = getHourMinute(timeEl);
    return convertTo12HourFormat(time24);
#else
    return getHourMinute(timeEl);
#endif
}

// Returns AM/PM indicator for 12-hour format using global language system
String getTaychronLocalizedAMPM(tmElements_t timeEl) {
#if WATCHFACE_12H
    // Determine if it's afternoon/evening (PM) or morning (AM)
    bool isPM = (timeEl.Hour >= 12);
    
    // Use global language macros
    return isPM ? WF_TIME_PM : WF_TIME_AM;
#else
    // Return empty string for 24-hour format
    return "";
#endif
}

// Updated getAMPM to use global language system
String getAMPM(String time24) {
    // Extract the hour from the 24-hour time string
    int colonIndex = time24.indexOf(':');
    int hour = time24.substring(0, colonIndex).toInt();

    // Determine AM or PM using global language macros
    if (hour < 12) {
        return WF_TIME_AM;
    } else {
        return WF_TIME_PM;
    }
}

// Keep the existing convertTo12HourFormat function unchanged
String convertTo12HourFormat(String time24) {
    // Split the input time (hh:mm) into hours and minutes
    int colonIndex = time24.indexOf(':');
    int hour = time24.substring(0, colonIndex).toInt();
    String minute = time24.substring(colonIndex + 1);

    // Convert the hour to 12-hour format
    if (hour == 0) {
        hour = 12; // Midnight case
    } else if (hour > 12) {
        hour -= 12; // Afternoon case
    }

    // Add leading zero to the hour if necessary
    String hourStr = String(hour);
    if (hourStr.length() == 1) {
        hourStr = "0" + hourStr;
    }

    // Combine hour and minute for the final result
    return hourStr + ":" + minute;
}