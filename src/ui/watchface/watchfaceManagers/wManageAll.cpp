#include "wManageAll.h"
#include "rtcMem.h"

#if WATCHFACE_ORBITAL
#include "../watchfaces/orbital_Defaltastra/orbital.h"
#endif

#if GSR_WATCHFACES
#include "importGSR.h"
#endif

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

#if WATCHFACE_SLATE
const watchfaceDef slateWatchface = {
    .manager = wfmOne,
    .name = "Slate",
    .data = (genPointer)&slateDef,
};
#endif

#if WATCHFACE_DOMAIN_DOTP
const watchfaceDef dotpDomain = {
    .manager = wfmOne,
    .name = "DotP's Domain",
    .data = (genPointer)&domainDef,
};
#endif

#if WATCHFACE_TERRAIN
const watchfaceDef terrainDef = {
    .manager = wfmOne,
    .name = "Szybet's Terrain",
    .data = (genPointer)&terrainDefOne,
};
#endif

#if WATCHFACE_ORBITAL
const watchfaceDef orbitalWatchface = {
    .manager = wfmOne,
    .name = "Orbital Defaltastra",
    .data = (genPointer)&orbitalDef,
};
#endif

#if WATCHFACE_STATION
const watchfaceDef stationWatchface = {
    .manager = wfmOne,
    .name = "Szybet's station",
    .data = (genPointer)&stationDef,
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
const watchfaceDef szybetAnalogSharp = {
    .manager = wfmTwo,
    .name = "Szybet's Analog sharp",
    .data = (genPointer)&analogConwayDef,
};
#endif

#if GSR_WATCHFACES && GSR_STARFIELD
const watchfaceDef gsrStarfield = {
    .manager = wfmGSR,
    .name = "GSR Starfield",
    .data = (genPointer)&MyGSRWatchFaceStarfield,
};
#endif

#if GSR_WATCHFACES && GSR_CLASSICS_BASICS
const watchfaceDef gsrBasic = {
    .manager = wfmGSR,
    .name = "GSR Basic",
    .data = (genPointer)&GSRBasicLoader,
};
#endif

#if GSR_WATCHFACES && GSR_CLASSICS_7SEG
const watchfaceDef gsr7Seg = {
    .manager = wfmGSR,
    .name = "GSR 7-SEG",
    .data = (genPointer)&GSR7SEGLoader,
};
#endif

#if GSR_WATCHFACES && GSR_CLASSICS_DOS
const watchfaceDef gsrDos = {
    .manager = wfmGSR,
    .name = "GSR DOS",
    .data = (genPointer)&GSRDOSLoader,
};
#endif

#if GSR_WATCHFACES && GSR_CLASSICS_POKE
const watchfaceDef gsrPokemon = {
    .manager = wfmGSR,
    .name = "GSR Pokemon",
    .data = (genPointer)&GSRPokemonLoader,
};
#endif

#if GSR_WATCHFACES && GSR_CLASSICS_STARRY
const watchfaceDef gsrStarry = {
    .manager = wfmGSR,
    .name = "GSR Starry Horizon",
    .data = (genPointer)&GSRStarryHorizonLoader,
};
#endif

#if GSR_WATCHFACES && GSR_CLASSICS_TETRIS
const watchfaceDef gsrTetris = {
    .manager = wfmGSR,
    .name = "GSR Tetris",
    .data = (genPointer)&GSRTetrisLoader,
};
#endif

#if GSR_WATCHFACES && GSR_CLASSICS_MAC
const watchfaceDef gsrMac = {
    .manager = wfmGSR,
    .name = "GSR Mac Paint",
    .data = (genPointer)&GSRMacPaintLoader,
};
#endif

#if GSR_WATCHFACES && GSR_CLASSICS_MARIO
const watchfaceDef gsrMario = {
    .manager = wfmGSR,
    .name = "GSR Mario",
    .data = (genPointer)&GSRMarioLoader,
};
#endif

#if GSR_WATCHFACES && GSR_STATIONARY
const watchfaceDef gsrStationary = {
    .manager = wfmGSR,
    .name = "GSR Stationary",
    .data = (genPointer)&TextualTimeGSRLoader,
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
#if WATCHFACE_SLATE
    &slateWatchface,
#else
    &noWatchFace,
#endif
#if WATCHFACE_DOMAIN_DOTP
    &dotpDomain,
#else
    &noWatchFace,
#endif
#if WATCHFACE_TERRAIN
    &terrainDef,
#else
    &noWatchFace,
#endif
#if WATCHFACE_ORBITAL
    &orbitalWatchface,
#else
    &noWatchFace,
#endif
#if WATCHFACE_STATION
    &stationWatchface,
#else
    &noWatchFace,
#endif
#if WATCHFACE_SHADES_SZYBET
    &szybetShades,
#else
    &noWatchFace,
#endif
#if WATCHFACE_ANALOG_SHARP_SZYBET
    &szybetAnalogSharp,
#else
    &noWatchFace,
#endif
#if GSR_WATCHFACES && GSR_STARFIELD
    &gsrStarfield,
#else
    &noWatchFace,
#endif
#if GSR_WATCHFACES && GSR_CLASSICS_BASICS
    &gsrBasic,
#else
    &noWatchFace,
#endif
#if GSR_WATCHFACES && GSR_CLASSICS_7SEG
    &gsr7Seg,
#else
    &noWatchFace,
#endif
#if GSR_WATCHFACES && GSR_CLASSICS_DOS
    &gsrDos,
#else
    &noWatchFace,
#endif
#if GSR_WATCHFACES && GSR_CLASSICS_POKE
    &gsrPokemon,
#else
    &noWatchFace,
#endif
#if GSR_WATCHFACES && GSR_CLASSICS_STARRY
    &gsrStarry,
#else
    &noWatchFace,
#endif
#if GSR_WATCHFACES && GSR_CLASSICS_TETRIS
    &gsrTetris,
#else
    &noWatchFace,
#endif
#if GSR_WATCHFACES && GSR_CLASSICS_MAC
    &gsrMac,
#else
    &noWatchFace,
#endif
#if GSR_WATCHFACES && GSR_CLASSICS_MARIO
    &gsrMario,
#else
    &noWatchFace,
#endif
#if GSR_WATCHFACES && GSR_STATIONARY
    &gsrStationary,
#else
    &noWatchFace,
#endif
};

// const watchfaceDef *watchfaceSel = getCurrentWatchface();
const watchfaceDef *getCurrentWatchface()
{
    const watchfaceDef *watchfaceSel = watchfacesList[rM.watchfaceSelected];
    // debugLog("Watchface selected: " + String(watchfaceSel->name));
    if (watchfaceSel->manager == wfmNone)
    {
        for (uint8_t i = rM.watchfaceSelected; i < WATCHFACE_COUNT; i++)
        {
            if (watchfacesList[i]->manager != wfmNone)
            {
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
        wfmTwoRet (*wfTwoFunc)(wfmTwoArg) = (wfmTwoRet (*)(wfmTwoArg))watchfaceSel->data;
        wfmTwoArg arg = wfmTwoArg::wTloop;
        if (init == true)
        {
            arg = wfmTwoArg::wTinit;
        }
        wfTwoFunc(arg);
        break;
    }
    case wfmGSR:
    {
#if GSR_WATCHFACES
        WatchyGSR *gsr = reinterpret_cast<WatchyGSR *>(watchfaceSel->data);
        wManageGsrLaunch(gsr, init);
#else
        debugLog("How did it happen? GSR IS DISABLED ISIN'T IT");
#endif
        break;
    }
    case wfmNone:
    {
        debugLog("So here we are, you, trying to debug wtf is going on with inkwatchy. Probably no watchfaces enabled");
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
#if WATCHFACE_INVERT_COLORS
    invertScreenColors();
#endif
    watchfaceManageAll(false);
#if WATCHFACE_INVERT_COLORS
    resetScreenColors();
#endif
}

void initWatchfaceManage()
{
#if WATCHFACE_INVERT_COLORS
    invertScreenColors();
#endif
    watchfaceManageAll(true);
#if WATCHFACE_INVERT_COLORS
    resetScreenColors();
#endif
}

// Only called when in watchface!
void nextWatchface()
{
    rM.watchfaceSelected += 1;
    if (rM.watchfaceSelected >= WATCHFACE_COUNT)
    {
        rM.watchfaceSelected = 0;
    }
    const watchfaceDef *watchfaceSel = watchfacesList[rM.watchfaceSelected];
    if (watchfaceSel->manager == wfmNone)
    {
        nextWatchface();
        return;
    }
    rM.currentPlace = NoPlace; // force initWatchfaceManage on next loop iteration, hacky, works
}

// Only called when in watchface!
void previousWatchFace()
{
    if (rM.watchfaceSelected == 0)
    {
        rM.watchfaceSelected = WATCHFACE_COUNT - 1;
    }
    else
    {
        rM.watchfaceSelected -= 1;
    }
    const watchfaceDef *watchfaceSel = watchfacesList[rM.watchfaceSelected];
    if (watchfaceSel->manager == wfmNone)
    {
        previousWatchFace();
        return;
    }
    rM.currentPlace = NoPlace; // force initWatchfaceManage on next loop iteration, hacky, works
}
