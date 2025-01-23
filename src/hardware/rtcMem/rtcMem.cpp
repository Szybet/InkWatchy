#include "rtcMem.h"

RTC_DATA_ATTR rtcMem rM = {
    // Battery
    .bat = {
        .curV = 0.0,
        .charV = 0.0,
        .isCharging = false,
        .isFullyCharged = false,
        .percentage = 0,
        .prevVOne = 0.0,
        .oneCheck = false},
    .isBatterySaving = false,
#if ATCHY_VER == YATCHY
    .previousStatInStateBefore = false,
    .previousStatInStateAfter = true, // There is no such configuration, so it will always trigger at least once
    .previousFiveVolt = false,        // false because it will be true after flashing
#endif
// Acc
#if AXC_ENABLED
    .SBMA = StableBMA{}, // Class
    .initedAxc = false,
    .stepsInited = false,
    .stepDay = 0,
#endif
    // Display
    .display = GxEPD2_154_D67(EPD_CS, EPD_DC, EPD_RESET, EPD_BUSY),
    .updateCounter = 0,
// Mcp23018
#if ATCHY_VER == YATCHY
    .gpioExpander = mcp23018{},
#endif
    // RTC
    .SRTC = SmallRTC{}, // Class
    .posixTimeZone = TIMEZONE_POSIX,
// Temp
#if TEMP_CHECKS_ENABLED
    .fixCounts = 0,
    .initialTemp = 0.0,
    .previousTemp = 0.0,
#endif
    // Wifi logic
    .lastSyncUnix = 0,
    .lastTryUnix = 0,
// Ntp
// Time drift correction
#if TIME_DRIFT_CORRECTION
    .driftStartUnix = 0,
#endif
// Watchdog
#if WATCHDOG_TASK
    .everythingIsFine = true,
#endif
    // Power settings
    .disableAllVibration = false,
    .disableWakeUp = false,
    // Manager
    .placeTree = {NoPlace},
    .currentPlace = NoPlace, // For loop manager for launching init or loop of a function
    // wManageOne
    .wFTime = {0},
    .disableSomeDrawing = false,
    .batteryPercantageWF = 0,
    // wManageAll
    .watchfaceSelected = 0,
// Inkfield watchface
#if WATCHFACE_INKFIELD_SZYBET
    .inkfield = {
        .dayBar = 0,
        .percentOfDay = 0,
        .percentSteps = 0,
        .weatherMinutes = 0,
        // inkput
        .watchfacePos = 0,
        .positionEngaged = false,
    },
#endif
#if WATCHFACE_SHADES_SZYBET
    .shades = {
        .stepsSaved = 0,
    },
#endif
// dark watchface
#if WATCHFACE_TAYCHRON
    .taychron = {
        .dayBar = 0,
        .weatherMinutes = 0,
        .Hours = 0,
    },
#endif

    // Watchface modules
    // MAKE SURE HERE ARE ALL MODULES INSERTED
    .wfModulesList = {
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
    },
    .latestModuleUpdate = 0,
    .currentModule = -1,
    .previousModuleCount = -1,
    .previousCurrentModule = -1, // The translated one!
// wFApi
#if API_MODULE
    .wfApi = {
        true,
        wfApicheckShow,
        wfApirequestShow,
    },
#endif
// wFBit
#if BITCOIN_MODULE
    .wfBit = {
#if MODULE_PERSISTENT
        true,
#else
        false,
#endif
        wfBitcheckShow,
        wfBitrequestShow,
    },
    .isBtcDataAvail = false,
    .isBtcDataNew = false,
    .smallBtcData = SMALL_BTC_MODULE,
    .btcLastUpdate = 0,
#endif
// wFBook
#if BOOK_MODULE_ENABLED
    .wfBook = {
        true,
        wfBookcheckShow,
        wfBookrequestShow,
    },
#endif
// wFConway
#if CONWAY_MODULE_ENABLED
    .wfConway = {
        true,
        wfConwaycheckShow,
        wfConwayrequestShow,
    },
    .timeChangeCheck = INIT_CONWAY_MOD_VAL, // if INIT_CONWAY_MOD_VAL, init the grid
    .conwayModuleGrid = {0},
#endif
// wFEvent
#if EVENT_MODULE
    .wfEvent = {
        true,
        wfEventcheckShow,
        wfEventrequestShow,
    },
    .currentEventTime = 0,
    .currentDay = -1,
#endif
// wFImg
#if IMAGE_MODULE
    .wfImage = {
        true,
        wfImagecheckShow,
        wfImagerequestShow,
    },
#if IMG_MODULE_CHANGE_EVERY_HOUR
    .imageCurrentHour = -1,
#endif
    .imageNameCrc32 = 0,
#endif
// wFNet
#if WIFI_MODULE
    .wfNet = {
        false,
        wfNetcheckShow,
        wfNetrequestShow,
    },
#endif
};