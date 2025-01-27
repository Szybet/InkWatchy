#include "watchFaceModule.h"
#include "rtcMem.h"

wfModule wfEmpty = {
    false,
    [](bool *showBool, bool *redrawBool) {},
    [](buttonState button, bool *showBool) {},
};

// MAKE SURE HERE ARE ALL MODULES INSERTED
wfModule *wfModulesList[MODULE_COUNT] = {
#if WIFI_MODULE
    &rM.wfNet,
#else
    &wfEmpty,
#endif
#if IMAGE_MODULE
    &rM.wfImage,
#else
    &wfEmpty,
#endif
#if BITCOIN_MODULE
    &rM.wfBit,
#else
    &wfEmpty,
#endif
#if BOOK_MODULE_ENABLED
    &rM.wfBook,
#else
    &wfEmpty,
#endif
#if CONWAY_MODULE_ENABLED
    &rM.wfConway,
#else
    &wfEmpty,
#endif
#if API_MODULE
    &rM.wfApi,
#else
    &wfEmpty,
#endif
#if EVENT_MODULE
    &rM.wfEvent,
#else
    &wfEmpty,
#endif
};

void clearModuleArea()
{
    debugLog("clearModuleArea: extecuted...");
    squareInfo modSq = getWatchModuleSquare();
    dis->fillRect(modSq.cord.x, modSq.cord.y, modSq.size.w, modSq.size.h, GxEPD_WHITE);
    dUChange = true;
}

void nothingModule()
{
    debugLog("Nothing module executed");
    clearModuleArea();
}

void moveModule(direction where)
{
    if (where == Right)
    {
        rM.currentModule = rM.currentModule + 1;
    }
    else if (where == Left)
    {
        rM.currentModule = rM.currentModule - 1;
    }
    checkMaxMin(&rM.currentModule, MODULE_COUNT - 1);
    debugLog("rM.currentModule: " + String(rM.currentModule));
}

// TODO: move this to watchface manager
#define MODULE_NUM_POS_X 185
#define MODULE_NUM_POS_Y 170
#define MODULE_COUNT_POS_X 185
#define MODULE_COUNT_POS_Y 181
#define MC_W 6
#define MC_H 8
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
        if (listShows[i] == rM.currentModule)
        {
            currentModuleTranslated = i;
            break;
        }
    }

    // debugLog("counter: " + String(counter));
    // debugLog("rM.currentModule: " + String(currentModuleTranslated));
    if (rM.previousModuleCount != counter || rM.previousCurrentModule != currentModuleTranslated || force == true)
    {
        rM.previousModuleCount = counter;
        rM.previousCurrentModule = currentModuleTranslated;
        setFont(getFont("dogicapixel4"));
        setTextSize(1);

        debugLog("Printing the current module");
        dis->fillRect(MODULE_NUM_POS_X, MODULE_NUM_POS_Y - MC_H + 1, MC_W, MC_H + 1, GxEPD_WHITE);
        dis->setCursor(MODULE_NUM_POS_X, MODULE_NUM_POS_Y);
        if (counter != 0)
        {
            currentModuleTranslated = currentModuleTranslated + 1;
        }
        dis->print(String(currentModuleTranslated));

        debugLog("Printing the counter");
        dis->fillRect(MODULE_COUNT_POS_X, MODULE_COUNT_POS_Y - MC_H + 1, MC_W, MC_H + 1, GxEPD_WHITE);
        dis->setCursor(MODULE_COUNT_POS_X, MODULE_COUNT_POS_Y);
        dis->print(String(counter));

        dUChange = true;

        if (counter == 0 && currentModuleTranslated == 0)
        {
            debugLog("There are actually no modules, clearing the area.");
            clearModuleArea();
        }
    }
}

void wfModuleSwitch(direction where)
{
#if DEBUG && true == false
    for (int i = 0; i < MODULE_COUNT; i++)
    {
        debugLog("Dump show values: " + String(i) + " " + BOOL_STR(wfModulesList[i]->show));
    }
#endif
    moveModule(where);
    int counter = 0;
    while (wfModulesList[rM.currentModule]->show != true && counter < MODULE_COUNT * 2)
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
        rM.currentModule = -1;
    }
}

// TODO: because of fallback, forcerender is not needed anymore?
void wfModulesManage(buttonState button, bool forceRender)
{
    rM.latestModuleUpdate = getUnixTime(timeRTCLocal);

    debugLog("Running wfModulesManage, current module is: " + String(rM.currentModule));
    if (rM.currentModule != -1 && button != None)
    {
        wfModulesList[rM.currentModule]->requestShow(button, &wfModulesList[rM.currentModule]->show);
        if (wfModulesList[rM.currentModule]->show == false)
        {
            rM.currentModule = -1;
        }
        else
        {
            return; // Don't do anything
        }
    }
    button = None; // Reset the button to not affect other modules, NOT SURE if it will work
    bool doIt = false;
    bool isThereAShow = false;
    for (int i = 0; i < MODULE_COUNT; i++)
    {
        bool render = false;
        // debugLog("Checking if show for index: " + String(i));
        wfModulesList[i]->checkShow(&wfModulesList[i]->show, &render);
        // debugLog("Render is: " + BOOL_STR(render));
        if (wfModulesList[i]->show == true)
        {
            isThereAShow = true;
        }
        // Force management
        if (forceRender == true && i == rM.currentModule)
        {
            doIt = true;
        }
        if ((i == rM.currentModule || rM.currentModule == -1) && render == true)
        {
            if (rM.currentModule == -1)
            {
                rM.currentModule = i;
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
    if (isThereAShow == true && doIt == false && rM.currentModule == -1)
    {
        debugLog("Looking for fallback module to show");
        for (int i = 0; i < MODULE_COUNT; i++)
        {
            bool render = false;
            wfModulesList[i]->checkShow(&wfModulesList[i]->show, &render);
            if (wfModulesList[i]->show == true)
            {
                debugLog("Found fallback module to show");
                rM.currentModule = i;
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
    debugLog("Exiting wfModulesManage");
}