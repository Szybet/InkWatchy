#include "analogSharp.h"
#include "rtcMem.h"

#if WATCHFACE_ANALOG_SHARP_SZYBET

void drawHand(int centerX, int centerY, uint16_t angle, int length)
{
    float adjustedAngle = 90 - angle;
    float rad = adjustedAngle * (PI / 180);
    int x = centerX + cos(rad) * length;
    int y = centerY - sin(rad) * length;
    dis->drawLine(centerX, centerY, x, y, GxEPD_BLACK);
}

#define LINE_WIDTH 6

wfmTwoRet analogConwayDef(wfmTwoArg arg)
{
    rM.wFTime = timeRTCLocal;
    debugLog("Launched analog conway");
    dis->fillRect(0, 0, 200, 200, GxEPD_WHITE);

    uint8_t hour = timeRTCLocal.Hour;
    hour = hour % 12;
    debugLog("Hour: " + String(hour) + " Minute: " + String(timeRTCLocal.Minute));

    uint16_t hourAngle = ((360 * hour) / 12);
    uint16_t minuteAngle = ((360 * timeRTCLocal.Minute) / 60);

    debugLog("Hour angle: " + String(hourAngle));
    debugLog("Minute angle: " + String(minuteAngle));

    for(int i = 100 - LINE_WIDTH; i < 100 + LINE_WIDTH; i++) {
        drawHand(i, i, hourAngle, 50);
        drawHand(i, i + 1, hourAngle, 50);
        drawHand(i, i, minuteAngle, 90);
        drawHand(i, i + 1, minuteAngle, 90);
    }

    disUp(true, false, true);
    setSleepDelay(0);
    if (useButton() == Menu)
    {
        resetSleepDelay();
        generalSwitch(mainMenu);
    }
    return wTNone;
}

#endif