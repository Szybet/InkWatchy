#pragma once

#include "defines.h"

#if INK_ALARMS

// Function to format alarm duration based on minutes
String formatAlarmDuration(int minutes) {
    String result = "";
    
    int days = minutes / 1440;
    int hours = (minutes % 1440) / 60;
    int mins = minutes % 60;
    
    bool hasMultipleParts = false;
    
    // Handle days
    if (days > 0) {
        if (days == 1) {
            result += String(days) + " " + String(TIME_UNIT_DAY);
        } else {
            result += String(days) + " " + String(TIME_UNIT_DAYS);
        }
        hasMultipleParts = (hours > 0 || mins > 0);
    }
    
    // Handle hours
    if (hours > 0) {
        if (hasMultipleParts && mins == 0) {
            result += " " + String(TIME_UNIT_AND) + " ";
        } else if (result.length() > 0) {
            result += " ";
        }
        
        if (hours == 1) {
            result += String(hours) + " " + String(TIME_UNIT_HOUR);
        } else {
            result += String(hours) + " " + String(TIME_UNIT_HOURS);
        }
        hasMultipleParts = mins > 0;
    }
    
    // Handle minutes
    if (mins > 0) {
        if (hasMultipleParts) {
            result += " " + String(TIME_UNIT_AND) + " ";
        } else if (result.length() > 0) {
            result += " ";
        }
        
        if (mins == 1) {
            result += String(mins) + " " + String(TIME_UNIT_MINUTE);
        } else {
            result += String(mins) + " " + String(TIME_UNIT_MINUTES);
        }
    }
    
    // Handle case where input is 0
    if (minutes == 0) {
        result = "0 " + String(TIME_UNIT_MINUTES);
    }
    
    return result;
}

// Polish-specific formatting
#if INKWATCHY_LANG == PL
String formatAlarmDurationPL(int minutes) {
    String result = "";
    
    int days = minutes / 1440;
    int hours = (minutes % 1440) / 60;
    int mins = minutes % 60;
    
    bool hasMultipleParts = false;
    
    // Handle days
    if (days > 0) {
        if (days == 1) {
            result += String(days) + " " + String(TIME_UNIT_DAY);
        } else if (days % 10 >= 2 && days % 10 <= 4 && (days % 100 < 10 || days % 100 > 20)) {
            result += String(days) + " " + String(TIME_UNIT_DAYS_2_4);
        } else {
            result += String(days) + " " + String(TIME_UNIT_DAYS);
        }
        hasMultipleParts = (hours > 0 || mins > 0);
    }
    
    // Handle hours
    if (hours > 0) {
        if (hasMultipleParts && mins == 0) {
            result += " " + String(TIME_UNIT_AND) + " ";
        } else if (result.length() > 0) {
            result += " ";
        }
        
        if (hours == 1) {
            result += String(hours) + " " + String(TIME_UNIT_HOUR);
        } else if (hours % 10 >= 2 && hours % 10 <= 4 && (hours % 100 < 10 || hours % 100 > 20)) {
            result += String(hours) + " " + String(TIME_UNIT_HOURS_2_4);
        } else {
            result += String(hours) + " " + String(TIME_UNIT_HOURS);
        }
        hasMultipleParts = mins > 0;
    }
    
    // Handle minutes
    if (mins > 0) {
        if (hasMultipleParts) {
            result += " " + String(TIME_UNIT_AND) + " ";
        } else if (result.length() > 0) {
            result += " ";
        }
        
        if (mins == 1) {
            result += String(mins) + " " + String(TIME_UNIT_MINUTE);
        } else if (mins % 10 >= 2 && mins % 10 <= 4 && (mins % 100 < 10 || mins % 100 > 20)) {
            result += String(mins) + " " + String(TIME_UNIT_MINUTES_2_4);
        } else {
            result += String(mins) + " " + String(TIME_UNIT_MINUTES);
        }
    }
    
    // Handle case where input is 0
    if (minutes == 0) {
        result = "0 " + String(TIME_UNIT_MINUTES);
    }
    
    return result;
}
#endif

// Wrapper function that selects appropriate language-specific formatter
inline String getFormattedAlarmDuration(int minutes) {
    #if INKWATCHY_LANG == PL
        return formatAlarmDurationPL(minutes);
    #else
        return formatAlarmDuration(minutes);
    #endif
}

#endif // INK_ALARMS