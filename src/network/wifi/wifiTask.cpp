#include "wifiTask.h"

std::mutex wifiTaskMutex;
bool isWifiTaskRunning;

WiFiMulti wifiMulti;
TaskHandle_t wifiTask;

uint8_t wifiConnectionTries = 0;
void (*wifiFunction)() = NULL;

void createWifiTask(uint8_t tries, void (*functionToRunAfterConnection)(), uint8_t wifiPriority)
{
    debugLog("Creating wifi task");
    wifiConnectionTries = tries;
    wifiFunction = functionToRunAfterConnection;
    if (isWifiTaskCheck() == false)
    {
        debugLog("xTaskCreate wifi");
        setBoolMutex(&wifiTaskMutex, &isWifiTaskRunning, true); // To make sure it's fast enough
        xTaskCreate(
            turnOnWifiTask,
            "wifiTask",
            TASK_STACK_WIFI,
            NULL,
            wifiPriority,
            &wifiTask);
    }
    else
    {
        debugLog("The task is already running? why?");
    }
}

void turnOnWifiTask(void *parameter)
{
    debugLog("Turning wifi on");
    setBoolMutex(&wifiTaskMutex, &isWifiTaskRunning, true);
    for (int i = 0; i < wifiConnectionTries; i++)
    {
        debugLog("Running wifi loop: " + String(i));
        // debugLog("isWifiTaskRunning: " + BOOL_STR(isWifiTaskCheck()));
        if (HARDWARE_POWER_SAVINGS == 1)
        {
            WiFi.setSleep(WIFI_PS_MAX_MODEM);
            debugLog("Setting sleep mode for wifi");
        }
        // We don't have NVS anymore
        // esp_wifi_set_storage(WIFI_STORAGE_RAM);
        // WiFi.persistent(false);
        // Won't work, fuck IDF for forcing that. We need a NVS partition
        // https://docs.espressif.com/projects/esp-idf/en/stable/esp32/api-guides/partition-tables.html
        // 0x3000 bytes we need
        WiFi.mode(WIFI_STA);

        debugLog("Wifi sleep mode: " + String(WiFi.getSleep()));

        WiFi.setAutoConnect(true);
        WiFi.setAutoReconnect(true);
        initWifi();
        wifiMulti.run(WIFI_MULTI_SYNC_TIME);

        if (WiFi.status() == WL_CONNECTED)
        {
            break;
        }
        else
        {
            debugLog("Wifi failed to connect, retrying...");
            // turnOffWifi();
            turnOffWifiMinimal();
            delayTask(WIFI_MULTI_ERROR_TIME);
        }
    }
    if (WiFi.status() == WL_CONNECTED)
    {
        debugLog("Wifi sleep mode: " + String(WiFi.getSleep()));
        delayTask(300);
        wifiFunction();
    }
    turnOffWifiMinimal();
    debugLog("Setting isWifiTaskRunning to false NOW");
    setBoolMutex(&wifiTaskMutex, &isWifiTaskRunning, false);
    vTaskDelete(NULL);
}

void turnOffWifiMinimal()
{
    if (WiFi.getMode() != WIFI_OFF)
    {
        if (WiFi.disconnect(true) == false)
        {
            debugLog("Failed to disconnect from wifi? turning it off anyway");
            if (WiFi.mode(WIFI_OFF) == false)
            {
                debugLog("Failed to force set mode of wifi, doing manual esp idf way");
                debugLog("esp_wifi_deinit: " + String(esp_err_to_name(esp_wifi_deinit())));
                debugLog("esp_wifi_stop: " + String(esp_err_to_name(esp_wifi_stop())));
            }
        }
    }
}

void turnOffWifi()
{
    debugLog("Turning wifi off");
    if (isWifiTaskCheck() == true)
    {
#if DEBUG
        uint8_t debugCounter = 0;
#endif
        while (WiFi.scanComplete() == WIFI_SCAN_RUNNING)
        {
            delayTask(30);
#if DEBUG
            if (debugCounter > 15)
            {
                debugLog("Wifi scan going, waiting...");
                debugCounter = 0;
            }
            debugCounter = debugCounter + 1;
#endif
        }
        vTaskDelete(wifiTask);
        // This is because task `arduino_events` has a queue and communicated with wifi task. Idk about this fix
        setBoolMutex(&wifiTaskMutex, &isWifiTaskRunning, false);
        // vTaskSuspend(wifiTask);
        // delayTask(1500);
    }
    turnOffWifiMinimal();
}

bool initWifiMultiDone = false;
void initWifi()
{
    if (initWifiMultiDone == true)
    {
        return void();
    }
    if (strlen(WIFI_SSID1) != 0 && strlen(WIFI_PASS1) != 0)
    {
        debugLog("Adding wifi " + String(WIFI_SSID1));
        wifiMulti.addAP(WIFI_SSID1, WIFI_PASS1);
    }

    if (strlen(WIFI_SSID2) != 0 && strlen(WIFI_PASS2) != 0)
    {
        debugLog("Adding wifi " + String(WIFI_SSID2));
        wifiMulti.addAP(WIFI_SSID2, WIFI_PASS2);
    }

    if (strlen(WIFI_SSID3) != 0 && strlen(WIFI_PASS3) != 0)
    {
        debugLog("Adding wifi " + String(WIFI_SSID3));
        wifiMulti.addAP(WIFI_SSID3, WIFI_PASS3);
    }

    if (strlen(WIFI_SSID4) != 0 && strlen(WIFI_PASS4) != 0)
    {
        debugLog("Adding wifi " + String(WIFI_SSID4));
        wifiMulti.addAP(WIFI_SSID4, WIFI_PASS4);
    }

    if (strlen(WIFI_SSID5) != 0 && strlen(WIFI_PASS5) != 0)
    {
        debugLog("Adding wifi " + String(WIFI_SSID5));
        wifiMulti.addAP(WIFI_SSID5, WIFI_PASS5);
    }

    if (strlen(WIFI_SSID6) != 0 && strlen(WIFI_PASS6) != 0)
    {
        debugLog("Adding wifi " + String(WIFI_SSID6));
        wifiMulti.addAP(WIFI_SSID6, WIFI_PASS6);
    }

    if (strlen(WIFI_SSID7) != 0 && strlen(WIFI_PASS7) != 0)
    {
        debugLog("Adding wifi " + String(WIFI_SSID7));
        wifiMulti.addAP(WIFI_SSID7, WIFI_PASS7);
    }

    if (strlen(WIFI_SSID8) != 0 && strlen(WIFI_PASS8) != 0)
    {
        debugLog("Adding wifi " + String(WIFI_SSID8));
        wifiMulti.addAP(WIFI_SSID8, WIFI_PASS8);
    }

    if (strlen(WIFI_SSID9) != 0 && strlen(WIFI_PASS9) != 0)
    {
        debugLog("Adding wifi " + String(WIFI_SSID9));
        wifiMulti.addAP(WIFI_SSID9, WIFI_PASS9);
    }

    if (strlen(WIFI_SSID10) != 0 && strlen(WIFI_PASS10) != 0)
    {
        debugLog("Adding wifi " + String(WIFI_SSID10));
        wifiMulti.addAP(WIFI_SSID10, WIFI_PASS10);
    }

    initWifiMultiDone = true;
}

bool isWifiTaskCheck()
{
    wifiTaskMutex.lock();
    bool tmp = isWifiTaskRunning;
    wifiTaskMutex.unlock();
    return tmp;
}

#if DEBUG == 1 || DEBUG_MENUS == 1
String wifiStatus()
{
    switch (WiFi.status())
    {
    case WL_NO_SHIELD:
        return "NO SHIELD";
    case WL_IDLE_STATUS:
        return "IDLE STATUS";
    case WL_NO_SSID_AVAIL:
        return "NO SSID AVAIL";
    case WL_SCAN_COMPLETED:
        return "SCAN COMPLETED";
    case WL_CONNECTED:
        return "CONNECTED";
    case WL_CONNECT_FAILED:
        return "CONNECT FAILED";
    case WL_CONNECTION_LOST:
        return "CONNECTION LOST";
    case WL_DISCONNECTED:
        return "DISCONNECTED";
    default:
        return "UNKNOWN STATUS";
    }
}
#endif

#if DEBUG
void loopwifiDebug()
{
}
#endif