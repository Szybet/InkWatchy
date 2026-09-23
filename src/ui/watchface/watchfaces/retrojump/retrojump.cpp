#include "retrojump.h"
#include "rtcMem.h"
#if WATCHFACE_RETROJUMP



void rjDrawHand(int centerX, int centerY, uint16_t angle, int length)
{
    float adjustedAngle = 90-(-120 + angle);
    float rad = adjustedAngle * (PI / 180);

    int center_xA = centerX-1;
    int center_yA = centerY-1;
    int center_xB = centerX;
    int center_yB = centerY;
    int center_xC = centerX+1;
    int center_yC = centerY+1; 
    int end_xA = center_xA + cos(rad) * length;
    int end_yA = center_yA - sin(rad) * length;
    int end_xB = center_xB + cos(rad) * length;
    int end_yB = center_yB - sin(rad) * length;
    int end_xC = center_xC + cos(rad) * length;
    int end_yC = center_yC - sin(rad) * length;
    
    dis->drawLine(center_xA, center_yA, end_xA, end_yA, SCBlack);
    dis->drawLine(center_xB, center_yA, end_xB, end_yA, SCBlack);
    dis->drawLine(center_xC, center_yA, end_xC, end_yA, SCBlack);

    dis->drawLine(center_xA, center_yB, end_xA, end_yB, SCBlack);
    dis->drawLine(center_xB, center_yB, end_xB, end_yB, SCBlack);
    dis->drawLine(center_xC, center_yB, end_xC, end_yB, SCBlack);

    dis->drawLine(center_xA, center_yC, end_xA, end_yC, SCBlack);
    dis->drawLine(center_xB, center_yC, end_xB, end_yC, SCBlack);
    dis->drawLine(center_xC, center_yC, end_xC, end_yC, SCBlack);


    //add a little tail
    end_xA = center_xA + cos(rad+PI) * 6;
    end_yA = center_yA - sin(rad+PI) * 6;
    end_xB = center_xB + cos(rad+PI) * 6;
    end_yB = center_yB - sin(rad+PI) * 6;
    end_xC = center_xC + cos(rad+PI) * 6;
    end_yC = center_yC - sin(rad+PI) * 6;

    dis->drawLine(center_xA, center_yA, end_xA, end_yA, SCBlack);
    dis->drawLine(center_xB, center_yA, end_xB, end_yA, SCBlack);
    dis->drawLine(center_xC, center_yA, end_xC, end_yA, SCBlack);

    dis->drawLine(center_xA, center_yB, end_xA, end_yB, SCBlack);
    dis->drawLine(center_xB, center_yB, end_xB, end_yB, SCBlack);
    dis->drawLine(center_xC, center_yB, end_xC, end_yB, SCBlack);

    dis->drawLine(center_xA, center_yC, end_xA, end_yC, SCBlack);
    dis->drawLine(center_xB, center_yC, end_xB, end_yC, SCBlack);
    dis->drawLine(center_xC, center_yC, end_xC, end_yC, SCBlack);
    
    dis->fillCircle(100,100,3, SCBlack);
}

void rjDrawWatchface()
{
    writeImageN(0, 0, getImg("retrojump/watchface"));    
    rjDrawHand(100,100, ((240 * timeRTCLocal.Minute) / 60), 62);

    String time24 = getHourMinute(timeRTCLocal);
#if WATCHFACE_12H
    time24 = convertTo12HourFormat(time24);
#endif
    writeImageN(87, 137, getImg("retrojump/" + String(time24[0])));
    writeImageN(102, 137, getImg("retrojump/" + String(time24[1])));
    
}
void drawTimeBeforeApply() {
    rjDrawWatchface();
}
static void drawTimeAfterApply(bool forceDraw)
{
    (void)forceDraw;
}
void drawDay() {
    dis->fillRect(0,185,50,15,SCWhite);
    setTextSize(1);
    setFont(getFont("dogicapixel4"));

    String day = getLocalizedDayByIndex(timeRTCLocal.Wday, 0);
    String month = getLocalizedMonthName(rM.wFTime.Month);
    String date = String(rM.wFTime.Day);
    
    dis->setCursor(0,197);
    dis->print(day+", "+month+" "+date);
}
void drawMonth() {return;}
void showTimeFull() {rjDrawWatchface();}
void initWatchface() {
    dis->fillScreen(SCWhite);
    writeImageN(0, 0, getImg("retrojump/watchface"));
}
void drawBattery() {
    dis->fillRect(150,185,50,15,SCWhite);
    setTextSize(1);
    setFont(getFont("dogicapixel4"));
    String battPct = String(rM.bat.percentage);

    uint16_t w, h;
    getTextBounds(battPct, NULL, NULL, &w, &h);
    // -1 from edge b/c dogica miscalculates or something
    dis->setCursor(199 - w, 197);
    dis->print(battPct);
}

static void manageInput(buttonState bt)
{
    if (bt == Menu)
    {
        generalSwitch(mainMenu);
        return;
    }
}
static bool retrojump_isModuleEngaged()
{
    return false;
}
const watchfaceDefOne retrojumpDefOne = {
    .drawTimeBeforeApply = drawTimeBeforeApply,
    .drawTimeAfterApply = drawTimeAfterApply,
    .drawDay = drawDay,
    .drawMonth = drawMonth,
    .showTimeFull = showTimeFull,
    .initWatchface = initWatchface,
    .drawBattery = drawBattery,
    .manageInput = manageInput,

    .watchfaceModules = false,
    .watchfaceModSquare = {.size{.w = 0, .h = 0}, .cord{.x = 0, .y = 0}},
    .someDrawingSquare = {.size{.w = 200, .h = 200}, .cord{.x = 0, .y = 0}},
    .isModuleEngaged = retrojump_isModuleEngaged,
    .lpCoreScreenPrepareCustom = NULL,
};
#endif