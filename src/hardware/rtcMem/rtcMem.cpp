#include "rtcMem.h"

RTC_DATA_ATTR rtcMem rM = {
    // Battery
    .bat = {
        .curV = 0.0,
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
#if ACC_ENABLED
    .SBMA = StableBMA{}, // Class
    .initedAcc = false,
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
    // Power settings
    .disableAllVibration = false,
    .disableWakeUp = false,
    .userDisableAllVibration = false,
    .userDisableWakeUp = false,
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
#if ACC_ENABLED
        .percentSteps = 0,
#else
        .showedTemp = 0,
#endif
        .weatherMinutes = 0,
        // inkput
        .watchfacePos = 0,
        .positionEngaged = false,
    },
#endif
// Shades watchface
#if WATCHFACE_SHADES_SZYBET
    .shades = {
        .stepsSaved = 0,
    },
#endif
// slate watchface
#if WATCHFACE_SLATE
    .slate = {
        .weatherAvailable = false, 
        .lastHourWeatherCheck = 255, 
        .lastBatteryLevel = 255, 
        .lastDay = 255, 
        .lastMonth = 255,
        .lastTemp = "",      // Initialize empty
        .lastCondition = "", // Initialize empty
    },
#endif
// taychron watchface
#if WATCHFACE_TAYCHRON
    .taychron = {
        .dayBar = 0,
        .weatherMinutes = 0,
        .Hours = 0,
    },
#endif
    // Watchface modules
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
// wFAlarm
#if ALARM_MODULE && INK_ALARMS
    .wfAlarm = {
        true,
        wfAlarmcheckShow,
        wfAlarmrequestShow,
    },
    .wfAlarmMinutes = -1,
    .wfAlarmHours = -1,
#endif
// Alarms
#if INK_ALARMS
    .alarms = {0},
    .nextAlarm = 0,
    .nextAlarmIndex = 0,
#if POMODORO_ALARM
    .pomodoroIter = 0,
    .pomodoroWorkNow = false,
#endif
#endif
    .ble_connection_attempts = 0,
};

#if RTC_MEMORY_BACKUP

RTC_DATA_ATTR unsigned char rtcMd5[16];

bool didRtcChange(rtcMem *source, rtcMem *destination)
{
    debugLog("Comparing rtc data");

    // Alarms
    if (memcmp(source->alarms, destination->alarms, sizeof(source->alarms)) != 0)
    {
        debugLog("Alarms data differs");
        return true;
    }

    if (source->nextAlarm != destination->nextAlarm ||
        source->nextAlarmIndex != destination->nextAlarmIndex)
    {
        debugLog("Alarm metadata differs");
        return true;
    }

    // posixTimeZone
    if (strlen(TIMEZONE_POSIX) == 0 && strlen(TIMEZONE_OLSON) == 0)
    {
        if (memcmp(source->posixTimeZone, destination->posixTimeZone, sizeof(source->posixTimeZone)) != 0)
        {
            debugLog("Timezone data differs");
            return true;
        }
    }

    debugLog("No changes detected");
    return false;
}

void rtcMemRetrieve(rtcMem *source, rtcMem *destination)
{
    debugLog("Retrieving data for rtc memory");
    // Alarms
    /*
    for (int i = 0; i < MAX_ALARMS; i++) {
        destination->alarms[i] = source->alarms[i];
    }
    */
    memcpy(destination->alarms, source->alarms, sizeof(destination->alarms));

    // Don't retrieve those, as it will trigger an old alarm, instead just calculate
    // destination->nextAlarm = source->nextAlarm;
    // destination->nextAlarmIndex = source->nextAlarmIndex;
    calculateNextAlarm();

    // posixTimeZone
    /*
    for (int i = 0; i < 50; i++) {
        destination->posixTimeZone[i] = source->posixTimeZone[i];
    }
    */
    if (strlen(TIMEZONE_POSIX) == 0 && strlen(TIMEZONE_OLSON) == 0)
    {
        memcpy(
            destination->posixTimeZone,
            source->posixTimeZone,
            sizeof(destination->posixTimeZone));
    }
}

void rtcMemBackupManage()
{
    debugLog("Entering rtcMemBackupManage");
    // bootStatus.fromWakeup == false is checked in initHardware
    if (bootStatus.resetReason != ESP_RST_PANIC && fsSetup() == true)
    {
        String filePath = String("/conf/") + String(CONF_RTC_BACKUP);
        if (fsFileExists(filePath) == true)
        {
            if (fsGetFileSize(filePath) == sizeof(rtcMem))
            {
                debugLog("Rtc backup exists and is correct size, recovering it");
                bufSize buff = fsGetBlob(CONF_RTC_BACKUP);
                rtcMem *rtcMemTmp = (rtcMem *)buff.buf;
                rtcMemRetrieve(rtcMemTmp, &rM);
                free(buff.buf);

                // Yatchy doesn't go to sleep so often, so we don't remove it here
                // fsRemoveFile(filePath);
                // This confuses yatchy for some reason also not needed anymore because this part is early in the boot now
                // esp_sleep_enable_timer_wakeup(1000);
                // esp_deep_sleep_start();
            }
            else
            {
                debugLog("Rtc backup size is wrong, removing that file...");
                fsRemoveFile(filePath);
            }
        }
        else
        {
            debugLog("Rtc backup doesn't exist, nevermind");
        }
    }
}
#endif
