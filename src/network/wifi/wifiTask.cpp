#include "wifiTask.h"

std::mutex wifiTaskMutex;
bool isWifiTaskRunning;

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
        debugLog("The task is already running? The user is spamming the button...");
    }
}

void tryToConnectWifi()
{
    debugLog("sizeof(wifiCredStatic): " + String(SIZE_WIFI_CRED_STAT));
    for (int i = 0; i < SIZE_WIFI_CRED_STAT; i++)
    {
        if (wifiCredStatic[i] == NULL || wifiCredStatic[i]->ssid == NULL || wifiCredStatic[i]->password == NULL)
        {
            debugLog("Skipping wifi id: " + String(i) + " because of null");
            continue;
        }
        else if (strlen(wifiCredStatic[i]->ssid) == 0 || strlen(wifiCredStatic[i]->password) < 8)
        {
            debugLog("Skipping wifi id: " + String(i) + " because bad length");
            continue;
        }
        debugLog("Trying to connect to wifi number: " + String(i) + " so: " + String(wifiCredStatic[i]->ssid) + " " + String(wifiCredStatic[i]->password));
        softStartDelay();

        WiFi.begin(wifiCredStatic[i]->ssid, wifiCredStatic[i]->password);

        for (int i = 0; i < WIFI_SYNC_TIME / 1000; i++)
        {
            delayTask(1000);
            if (WiFi.status() == WL_CONNECTED)
            {
                return;
            }
#if DEBUG
            else
            {
                debugLog("Failed to connect to wifi...");
            }
#endif
        }
        if (WiFi.status() == WL_CONNECTED)
        {
            return;
        }
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
#if HARDWARE_POWER_SAVINGS
        WiFi.setSleep(WIFI_PS_MAX_MODEM);
        debugLog("Setting sleep mode for wifi");
#endif
        // We don't have NVS anymore
        // esp_wifi_set_storage(WIFI_STORAGE_RAM);
        // WiFi.persistent(false);
        // Won't work, fuck IDF for forcing that. We need a NVS partition
        // https://docs.espressif.com/projects/esp-idf/en/stable/esp32/api-guides/partition-tables.html
        // 0x3000 bytes we need
        softStartDelay();
        WiFi.mode(WIFI_STA);
        softStartDelay();

        debugLog("Wifi sleep mode: " + String(WiFi.getSleep()));

        WiFi.setAutoReconnect(true);

        tryToConnectWifi();

        if (WiFi.status() == WL_CONNECTED)
        {
            debugLog("Finally connected: " + WiFi.localIP().toString());
            break;
        }
        else
        {
            debugLog("Wifi failed to connect, retrying...");
            turnOffWifiMinimal();
            delayTask(WIFI_ERROR_TIME);
        }
    }
    softStartDelay();
    if (WiFi.status() == WL_CONNECTED)
    {
        debugLog("Wifi sleep mode: " + String(WiFi.getSleep()));
        delayTask(300);
        wifiFunction();
    }
    else
    {
        debugLog("Failed to connect to wifi properly");
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
    wl_status_t wifiStatus = WiFi.status();
    switch (wifiStatus)
    {
    case WL_NO_SHIELD:
        return "NO SHIELD";
    case WL_STOPPED:
        return "WL_STOPPED";
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
        return "UNKNOWN: " + String(wifiStatus);
    }
}
#endif

#if DEBUG
void loopwifiDebug()
{
}
#endif
