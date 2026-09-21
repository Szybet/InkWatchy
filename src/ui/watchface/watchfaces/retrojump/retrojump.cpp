#include "retrojump.h"
#include "rtcMem.h"
#if WATCHFACE_RETROJUMP


#define DEG2RAD 0.01745329252f
#define OUTER_RADIUS 80
#define INNER_RADIUS 56

static void rjDrawTicks()
{
    const unsigned int center_x = 100;
    const unsigned int center_y = 100;

    const int tickWidth = OUTER_RADIUS - INNER_RADIUS;
    //ticks
    for (int i = -120; i <= 120; i += 4)
    {
        int inRad = INNER_RADIUS;
        
        if(i % 20 != 0) {
            inRad += tickWidth/4;
        }
        float cx1 = center_x + (inRad) * cosf((i - 90) * DEG2RAD);
        float cy1 = center_y + (inRad) * sinf((i - 90) * DEG2RAD);
        float cx2 = center_x + (OUTER_RADIUS+12) * cosf((i - 90) * DEG2RAD);
        float cy2 = center_y + (OUTER_RADIUS+12) * sinf((i - 90) * DEG2RAD);
        dis->drawLine(cx1, cy1, cx2, cy2, SCBlack);
    }

    //clear top arc
    for(int i = -90; i <= 89; i+= 1) {
        float cx1 = center_x + (OUTER_RADIUS) * cosf((i - 90) * DEG2RAD);
        float cy1 = center_y + (OUTER_RADIUS) * sinf((i - 90) * DEG2RAD);
        float cx2 = center_x + (OUTER_RADIUS+13) * cosf((i - 90) * DEG2RAD);
        float cy2 = center_y + (OUTER_RADIUS+13) * sinf((i - 90) * DEG2RAD);
        float cx3 = center_x + (OUTER_RADIUS) * cosf((i - 90 + 1) * DEG2RAD);
        float cy3 = center_y + (OUTER_RADIUS) * sinf((i - 90 + 1) * DEG2RAD);
        float cx4 = center_x + (OUTER_RADIUS+13) * cosf((i - 90 + 1) * DEG2RAD);
        float cy4 = center_y + (OUTER_RADIUS+13) * sinf((i - 90 + 1) * DEG2RAD);

        dis->fillTriangle(cx1, cy1, cx2, cy2, cx3, cy3, SCWhite);
        dis->fillTriangle(cx2, cy2, cx3, cy3, cx4, cy4, SCWhite);
    }
    //clear bottom arc
    int padWidth = 100-OUTER_RADIUS;
    dis->fillRect(0,100,padWidth,101, SCWhite);
    dis->fillRect(200-padWidth,100,padWidth, 101, SCWhite);
    //inner
    // dis->fillRect(100-INNER_RADIUS,100,100-(INNER_RADIUS*2),101, SCWhite);
    dis->fillRect(100-INNER_RADIUS,100,(INNER_RADIUS*2),101, SCWhite);

    //decorations
    dis->drawRoundRect(100-INNER_RADIUS+8, 100-(INNER_RADIUS-8), (INNER_RADIUS-8)*2, 200, (INNER_RADIUS-8)*2, SCBlack);
}


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
    dis->fillScreen(SCWhite);
    writeImageN(0, 0, getImg("retrojump/watchface"));
    //rjDrawTicks();
    
    rjDrawHand(100,100, ((240 * timeRTCLocal.Minute) / 60), OUTER_RADIUS-8);
    //((360 * timeRTCLocal.Minute) / 60)
    
    
    
}
void drawTimeBeforeApply() {return;}
static void drawTimeAfterApply(bool forceDraw)
{
    (void)forceDraw;
}
void drawDay() {return;}
void drawMonth() {return;}
void showTimeFull() {rjDrawWatchface();}
void initWatchface() {rjDrawWatchface();}
void drawBattery() {return;}

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