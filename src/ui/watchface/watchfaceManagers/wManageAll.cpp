#include "wManageAll.h"
#include "rtcMem.h"

const watchfaceDef noWatchFace{
    .manager = wfmNone,
    .name = "No watchface",
    .data = 0,
};

#if WATCHFACE_INKFIELD_SZYBET
const watchfaceDef szybetStarfield = {
    .manager = wfmOne,
    .name = "Szybet's InkField",
    .data = (genPointer)&inkFieldDef,
};
#endif
#if WATCHFACE_TAYCHRON
const watchfaceDef tayDef = {
    .manager = wfmOne,
    .name = "Taychron",
    .data = (genPointer)&taychronDef,
};
#endif
#if WATCHFACE_SHADES_SZYBET
const watchfaceDef szybetShades = {
    .manager = wfmOne,
    .name = "Szybet's Shades",
    .data = (genPointer)&shadesDef,
};
#endif
#if WATCHFACE_ANALOG_SHARP_SZYBET
const watchfaceDef szybetAnalogConway = {
    .manager = wfmTwo,
    .name = "Szybet's Analog sharp",
    .data = (genPointer)&analogConwayDef,
};
#endif

const watchfaceDef *watchfacesList[WATCHFACE_COUNT] = {
#if WATCHFACE_INKFIELD_SZYBET
    &szybetStarfield,
#else
    &noWatchFace,
#endif
#if WATCHFACE_TAYCHRON
    &tayDef,
#else
    &noWatchFace,
#endif
#if WATCHFACE_SHADES_SZYBET
    &szybetShades,
#else
    &noWatchFace,
#endif
#if WATCHFACE_SHADES_SZYBET
    &szybetAnalogConway,
#else
    &noWatchFace,
#endif
};

// const watchfaceDef *watchfaceSel = getCurrentWatchface();
const watchfaceDef *getCurrentWatchface()
{
    const watchfaceDef *watchfaceSel = watchfacesList[rM.watchfaceSelected];
    // debugLog("Watchface selected: " + String(watchfaceSel->name));
    if(watchfaceSel->manager == wfmNone) {
        for(uint8_t i = 0; i < WATCHFACE_COUNT; i++) {
            if(watchfacesList[i]->manager != wfmNone) {
                rM.watchfaceSelected = i;
                return getCurrentWatchface();
            }
        }
        debugLog("I'm pretty sure no watchfaces are enabled, well, crap");
    }
    return watchfaceSel;
}

// const watchfaceDefOne * wFO = getwatchfaceDefOne();
const watchfaceDefOne *getwatchfaceDefOne()
{
    const watchfaceDef *watchfaceSel = getCurrentWatchface();
    if (watchfaceSel->manager == wfmOne)
    {
        watchfaceDefOne *wOne = (watchfaceDefOne *)watchfaceSel->data;
        return wOne;
    }
    // debugLog("Get watchface one failed, returning NULL. This should not happen");
    return NULL;
}

void watchfaceManageAll(bool init)
{

    const watchfaceDef *watchfaceSel = getCurrentWatchface();
    switch (watchfaceSel->manager)
    {
    case wfmOne:
    {
        // debugLog("wfmOne selected");
        watchfaceDefOne *wOne = (watchfaceDefOne *)watchfaceSel->data;
        wManageOneLaunch(wOne, init);
        break;
    }
    case wfmTwo:
    {
        // debugLog("wfmTwo selected");
        wfmTwoRet (*wfTwoFunc)(wfmTwoArg) = (wfmTwoRet(*)(wfmTwoArg))watchfaceSel->data;
        wfmTwoArg arg = wfmTwoArg::wTloop;
        if (init == true)
        {
            arg = wfmTwoArg::wTinit;
        }
        wfTwoFunc(arg);
        break;
    }
    case wfmNone:
    {
        debugLog("So here we are, you, trying to debug wtf is going on with inkwatchy and me, trying to provide good documentation on how to update it. Here, look, the link: https://github.com/Szybet/InkWatchy/wiki/Updating-the-firmware I really can't help you further... :(");
        break;
    }
    default:
    {
        debugLog("Watchface manager is unknown, how, why, oof");
        break;
    }
    }
}

void loopWatchfaceManage()
{
    watchfaceManageAll(false);
}

void initWatchfaceManage()
{
    watchfaceManageAll(true);
}