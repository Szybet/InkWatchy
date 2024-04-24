#include "watchFaceModule.h"

wfModule wfEmpty = {
    false,
    [](bool *showBool, bool *redrawBool) {},
    [](buttonState button, bool *showBool) {},
};

RTC_DATA_ATTR int currentModule = -1;
// MAKE SURE HERE ARE ALL MODULES INSERTED
RTC_DATA_ATTR wfModule *wfModulesList[MODULE_COUNT] = {
#if WIFI_MODULE
    &wfNet,
#else
    &wfEmpty,
#endif
#if BITCOIN_MODULE
    &wfBit,
#else
    &wfEmpty,
#endif
#if CONWAY_MODULE_ENABLED
    &wfConway,
#else
    &wfEmpty,
#endif
    &wfEmpty,
    &wfEmpty,
    &wfEmpty,
    &wfEmpty,
    &wfEmpty,
    &wfEmpty,
    &wfEmpty,
};

void clearModuleArea()
{
    debugLog("clearModuleArea: extecuted...");
    display.fillRect(MODULE_RECT_X, MODULE_RECT_Y, MODULE_W, MODULE_H, GxEPD_WHITE);
}

void nothingModule()
{
    debugLog("Nothing module executed");
    clearModuleArea();
    wfPlace.requestShow(None, &wfPlace.show);
}

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
        if (wfModulesList[i]->show == true)
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
#if DEBUG && true == true
    for (int i = 0; i < MODULE_COUNT; i++)
    {
        debugLog("Dump show values: " + String(i) + " " + BOOL_STR(wfModulesList[i]->show));
    }
#endif
    moveModule(where);
    int counter = 0;
    while (wfModulesList[currentModule]->show != true && counter < MODULE_COUNT * 2)
    {
        moveModule(where);
        counter = counter + 1;
    }

    if (counter < MODULE_COUNT * 2)
    {
        wfModulesManage(None, true);
    }
    else
    {
        nothingModule();
        currentModule = -1;
    }
}

// TODO: because of fallback, forcerender is not needed anymore?
void wfModulesManage(buttonState button, bool forceRender)
{
    debugLog("Running wfModulesManage, current module is: " + String(currentModule));
    if (currentModule != -1 && button != None)
    {
        wfModulesList[currentModule]->requestShow(button, &wfModulesList[currentModule]->show);
        if (wfModulesList[currentModule]->show == false)
        {
            currentModule = -1;
        }
        else
        {
            return; // Don't do anything
        }
    }
    bool doIt = false;
    bool isThereAShow = false;
    for (int i = 0; i < MODULE_COUNT; i++)
    {
        bool render = false;
        //debugLog("Checking if show for index: " + String(i));
        wfModulesList[i]->checkShow(&wfModulesList[i]->show, &render);
        //debugLog("Render is: " + BOOL_STR(render));
        if (wfModulesList[i]->show == true)
        {
            isThereAShow = true;
        }
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
            wfModulesList[i]->requestShow(button, &wfModulesList[i]->show);
            break;
        }
    }
    // debugLog("Exited for loop in wfModulesManage");
    //  Now a loop for if nothing was found because nothing wants to render but we need to show something and maybe there is something to show
    if (isThereAShow == true && doIt == false && currentModule == -1)
    {
        debugLog("Looking for fallback module to show");
        for (int i = 0; i < MODULE_COUNT; i++)
        {
            bool render = false;
            wfModulesList[i]->checkShow(&wfModulesList[i]->show, &render);
            if (wfModulesList[i]->show == true)
            {
                debugLog("Found fallback module to show");
                currentModule = i;
                doIt = true;
                clearModuleArea();
                wfModulesList[i]->requestShow(button, &wfModulesList[i]->show);
                break;
            }
        }
    }
    if (doIt == false && button != None)
    {
        debugLog("Found nothing...");
        nothingModule();
    }
    drawModuleCount();
}