#ifndef WMANAGE_ONE_H
#define WMANAGE_ONE_H

#include "../../../defines/defines.h"

/*
#define SOME_RECT_X 0
#define SOME_RECT_Y 62
#define SOME_RECT_W 200
#define SOME_RECT_H 138

#define MODULE_RECT_X 7
#define MODULE_RECT_Y 160

#define MODULE_W 177
#define MODULE_H 37
*/

struct watchfaceDefOne {
    void (*drawTimeBeforeApply)(); // Draw the time. Here you can compare the times to draw only whats needed, or just draw it all. If you don't want to implement this mess, in this function call showTimeFull
    void (*drawTimeAfterApply)(); // Do things after time is applied. Like update the step counter, some bars
    void (*drawDay)(); //  Do things after the day changed, like update the date
    void (*drawMonth)(); // Do things after the month changed, like updating the month
    void (*showTimeFull)(); // Fully draw the current time
    void (*initWatchface)(); // Called on init once. Draw here static images and all other things
    void (*drawBattery)(); // Draw the battery, it's to avoid logic replication as every watchface will show it. This function is only called when battery percentage changed
    void (*manageInput)(buttonState bt);

    bool watchfaceModules; // To enable modules. All things below are only used if this is enabled
    squareInfo watchfaceModSquare; // Position of watchface modules
    squareInfo someDrawingSquare; // Should probably not be different then SOME_RECT_X
    bool (*isModuleEngaged)(); // Replaces watchfacePos == MODULE_ENG_POS && positionEngaged == true
};

void wManageOneLaunch(const watchfaceDefOne* wdo, bool init);
void wManageOneDrawAll(const watchfaceDefOne* wdo);

extern tmElements_t wFTime;
extern bool disableSomeDrawing;
extern uint8_t batteryPercantageWF;
extern bool disableWatchfaceFastOperating;

void initWatchfaceDisplay();
void loopWatchfaceLoop();

#endif
