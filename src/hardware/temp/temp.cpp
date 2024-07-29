#include "temp.h"

#if TEMP_CHECKS_ENABLED

#ifdef __cplusplus
extern "C"
{
#endif
    uint8_t temprature_sens_read();
#ifdef __cplusplus
}
#endif
uint8_t temprature_sens_read();

float getTemp()
{
    return float(temprature_sens_read() - 32) / 1.8 + float(TEMP_REAL_OFFSET);
}

RTC_DATA_ATTR uint8_t fixCounts = 0;
void screenTempFix()
{
    if (fixCounts <= TEMP_MAX_SCREEN_FIXES && updateCounter < FULL_DISPLAY_UPDATE_QUEUE)
    {
        updateCounter = FULL_DISPLAY_UPDATE_QUEUE;
        fixCounts = fixCounts + 1;
    }
}

RTC_DATA_ATTR float initialTemp = 0.0;
RTC_DATA_ATTR float previousTemp = 0.0;
void tempChecker()
{
    float newTemp = getTemp();
    if (newTemp > previousTemp + 1.5 || newTemp < previousTemp - 1.5)
    {
        if (initialTemp == 0.0)
        {
            debugLog("Initial temp is: " + String(newTemp));
            initialTemp = newTemp;
        }
        debugLog("Temperature changed to: " + String(newTemp));
        previousTemp = newTemp;
        if (newTemp > initialTemp + float(TEMP_REBOOT_LIMIT_RELATIVE))
        {
            debugLog("Temperature too high, exiting");
            assert(true == false);
        }
        if (newTemp > initialTemp + float(TEMP_HIGHER_LIMIT_RELATIVE))
        {
            screenTempFix();
        }
        else if (newTemp < initialTemp + float(TEMP_LOWER_LIMIT_RELATIVE))
        {
            screenTempFix();
        }
        else
        {
            fixCounts = 0;
        }
    }
}

#endif