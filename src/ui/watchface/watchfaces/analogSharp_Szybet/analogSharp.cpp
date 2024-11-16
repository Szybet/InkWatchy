#include "analogSharp.h"

#if WATCHFACE_ANALOG_SHARP_SZYBET

void drawHand(int centerX, int centerY, float angle, int length)
{
    float adjustedAngle = 90 - angle;
    float rad = adjustedAngle * (PI / 180);
    int x = centerX + cos(rad) * length;
    int y = centerY - sin(rad) * length;
    display.drawLine(centerX, centerY, x, y, GxEPD_BLACK);
}

#define LINE_WIDTH 7

wfmTwoRet analogConwayDef(wfmTwoArg arg)
{
    wFTime = timeRTCLocal;
    debugLog("Launched analog conway");
    display.fillRect(0, 0, 200, 200, GxEPD_WHITE);

    float hourAngle = (timeRTCLocal.Hour + timeRTCLocal.Minute / 60.0) * 15;
    float minuteAngle = (timeRTCLocal.Minute / 60.0) * 6;

    debugLog("Hour angle: " + String(hourAngle));
    debugLog("Minute angle: " + String(minuteAngle));

    for(int i = 100 - LINE_WIDTH; i < 100 + LINE_WIDTH; i++) {
        drawHand(i, i, hourAngle, 50);
        drawHand(i, i, minuteAngle, 90);
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