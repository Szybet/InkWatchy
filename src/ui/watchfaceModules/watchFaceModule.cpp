#include "watchFaceModule.h"

wfModule wfEmpty = {
    false,
    [](bool *showBool, bool *redrawBool) {},
    [](buttonState button) {},
};

// Ugly but works... menu could be done in the same way, at least the main one
// MAKE SURE HERE ARE ALL MODULES INSERTED
RTC_DATA_ATTR wfModule wfModulesList[MODULE_COUNT] = {
#if WIFI_MODULE
    wfNet,
#else
    wfEmpty,
#endif
#if BITCOIN_MODULE
    wfBit,
#else
    wfEmpty,
#endif
    wfEmpty,
    wfEmpty,
    wfEmpty,
    wfEmpty,
    wfEmpty,
    wfEmpty,
    wfEmpty,
    wfEmpty,
};

void clearModuleArea()
{
    display.fillRect(MODULE_RECT_X, MODULE_RECT_Y, MODULE_W, MODULE_H, GxEPD_WHITE);
    disUp(true);
}

void nothingModule()
{
    debugLog("Nothing module executed");
    clearModuleArea();
    wfPlace.requestShow(None);
}

RTC_DATA_ATTR int currentModule = -1;
void moveModule(direction where)
{
    if (where == Right)
    {
        currentModule = currentModule + 1;
    }
    else if (where == Left)
    {
        currentModule = currentModule - 1;
    }
    checkMaxMin(&currentModule, MODULE_COUNT - 1);
    debugLog("currentModule: " + String(currentModule));
}

#define MODULE_NUM_POS_X 185
#define MODULE_NUM_POS_Y 170
#define MODULE_COUNT_POS_X 185
#define MODULE_COUNT_POS_Y 181
#define MC_W 6
#define MC_H 8
RTC_DATA_ATTR int previousModuleCount = -1;
RTC_DATA_ATTR int previousCurrentModule = -1; // The translated one!
void drawModuleCount(bool force)
{
    // debugLog("drawModuleCount executed");
    int counter = 0;
    int listShows[MODULE_COUNT] = {0};
    int listIndexer = 0;
    for (int i = 0; i < MODULE_COUNT; i++)
    {
        if (wfModulesList[i].show == true)
        {
            listShows[listIndexer] = i;
            listIndexer = listIndexer + 1;
            counter = counter + 1;
        }
    }
    int currentModuleTranslated = 0;
    for (int i = 0; i < listIndexer; i++)
    {
        if (listShows[i] == currentModule)
        {
            currentModuleTranslated = i;
            break;
        }
    }

    // debugLog("counter: " + String(counter));
    // debugLog("currentModule: " + String(currentModuleTranslated));
    if (previousModuleCount != counter || previousCurrentModule != currentModuleTranslated || force == true)
    {
        previousModuleCount = counter;
        previousCurrentModule = currentModuleTranslated;
        setFont(&dogicapixel4pt7b);
        setTextSize(1);

        debugLog("Printing the current module");
        display.fillRect(MODULE_NUM_POS_X, MODULE_NUM_POS_Y - MC_H + 1, MC_W, MC_H + 1, GxEPD_WHITE);
        display.setCursor(MODULE_NUM_POS_X, MODULE_NUM_POS_Y);
        if (counter != 0)
        {
            currentModuleTranslated = currentModuleTranslated + 1;
        }
        display.print(String(currentModuleTranslated));

        debugLog("Printing the counter");
        display.fillRect(MODULE_COUNT_POS_X, MODULE_COUNT_POS_Y - MC_H + 1, MC_W, MC_H + 1, GxEPD_WHITE);
        display.setCursor(MODULE_COUNT_POS_X, MODULE_COUNT_POS_Y);
        display.print(String(counter));

        disUp(true);
    }
}

void wfModuleSwitch(direction where)
{
    moveModule(where);
    int counter = 0;
    while (wfModulesList[currentModule].show != true && counter < MODULE_COUNT)
    {
        moveModule(where);
        counter = counter + 1;
    }

    if (counter < MODULE_COUNT)
    {
        wfModulesManage(None, true);
    }
    else
    {
        nothingModule();
        currentModule = -1;
    }
}

void wfModulesManage(buttonState button, bool forceRender)
{
    //debugLog("Running wfModulesManage, current module is: " + String(currentModule));
    if (button == Menu)
    {
        wfModulesList[currentModule].show = false;
        wfModuleSwitch(Right);
        return;
    }
    bool doIt = false;
    for (int i = 0; i < MODULE_COUNT; i++)
    {
        bool render = false;
        // debugLog("Checking if show for index: " + String(i));
        wfModulesList[i].checkShow(&wfModulesList[i].show, &render);
        // Force management
        if (forceRender == true && i == currentModule)
        {
            doIt = true;
        }
        if ((i == currentModule || currentModule == -1) && render == true)
        {
            if (currentModule == -1)
            {
                currentModule = i;
            }
            doIt = true;
        }
        if (doIt == true)
        {
            clearModuleArea();
            debugLog("Launching module show request: " + String(i));
            wfModulesList[i].requestShow(button);
            drawModuleCount();
            break;
        }
    }
    // debugLog("Exited for loop in wfModulesManage");
    if (doIt == false && forceRender == true)
    {
        nothingModule();
    }
}