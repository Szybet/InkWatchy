#include "wifi.h"

WiFiMulti wifiMulti;
TaskHandle_t wifiTask;

std::mutex wifiTaskMutex;
bool isWifiTaskRunning;

bool initWifiMultiDone = false;

void initWifi()
{
    if (initWifiMultiDone == true)
    {
        return void();
    }
    if (WIFI_SSID1 != "" && WIFI_PASS1 != "")
    {
        debugLog("Attempting to connect to WiFi " + String(WIFI_SSID1));
        wifiMulti.addAP(WIFI_SSID1, WIFI_PASS1);
    }
    if (WIFI_SSID2 != "" && WIFI_PASS2 != "")
    {
        debugLog("Attempting to connect to WiFi " + String(WIFI_SSID2));
        wifiMulti.addAP(WIFI_SSID2, WIFI_PASS2);
    }
    if (WIFI_SSID3 != "" && WIFI_PASS3 != "")
    {
        debugLog("Attempting to connect to WiFi " + String(WIFI_SSID3));
        wifiMulti.addAP(WIFI_SSID3, WIFI_PASS3);
    }
    if (WIFI_SSID4 != "" && WIFI_PASS4 != "")
    {
        debugLog("Attempting to connect to WiFi " + String(WIFI_SSID4));
        wifiMulti.addAP(WIFI_SSID4, WIFI_PASS4);
    }
    if (WIFI_SSID5 != "" && WIFI_PASS5 != "")
    {
        debugLog("Attempting to connect to WiFi " + String(WIFI_SSID5));
        wifiMulti.addAP(WIFI_SSID5, WIFI_PASS5);
    }
    if (WIFI_SSID6 != "" && WIFI_PASS6 != "")
    {
        debugLog("Attempting to connect to WiFi " + String(WIFI_SSID6));
        wifiMulti.addAP(WIFI_SSID7, WIFI_PASS7);
    }
    if (WIFI_SSID7 != "" && WIFI_PASS7 != "")
    {
        debugLog("Attempting to connect to WiFi " + String(WIFI_SSID7));
        wifiMulti.addAP(WIFI_SSID7, WIFI_PASS7);
    }
    if (WIFI_SSID8 != "" && WIFI_PASS8 != "")
    {
        debugLog("Attempting to connect to WiFi " + String(WIFI_SSID8));
        wifiMulti.addAP(WIFI_SSID8, WIFI_PASS8);
    }
    if (WIFI_SSID9 != "" && WIFI_PASS9 != "")
    {
        debugLog("Attempting to connect to WiFi " + String(WIFI_SSID9));
        wifiMulti.addAP(WIFI_SSID9, WIFI_PASS9);
    }
    if (WIFI_SSID10 != "" && WIFI_PASS10 != "")
    {
        debugLog("Attempting to connect to WiFi " + String(WIFI_SSID10));
        wifiMulti.addAP(WIFI_SSID10, WIFI_PASS10);
    }
    initWifiMultiDone = true;
}

RTC_DATA_ATTR long lastSyncUnix = 0;
RTC_DATA_ATTR long lastTryUnix = 0;
int wifiConnectionTries = 0;
bool stillBeConnected = false;
void turnOnWifiTask(void *parameter)
{
    debugLog("Turning wifi on");
    setBoolMutex(&wifiTaskMutex, &isWifiTaskRunning, true);
    for (int i = 0; i < wifiConnectionTries; i++)
    {
        debugLog("Running wifi loop: " + String(i));
        //debugLog("isWifiTaskRunning: " + BOOL_STR(isWifiTaskCheck()));
        WiFi.mode(WIFI_STA);
        // WiFi.setSleep(WIFI_PS_NONE);
        WiFi.setAutoConnect(true);
        WiFi.setAutoReconnect(true);
        initWifi();
        wifiMulti.run(17000);

        if (WiFi.status() == WL_CONNECTED)
        {
            break;
        }
        else
        {
            debugLog("Wifi failed to connect, retrying...");
            // turnOffWifi();
            turnOffWifiMinimal();
            delayTask(1500);
        }
    }
    delayTask(300);
    if (WiFi.status() == WL_CONNECTED)
    {
        syncNtp();
        syncWeather();
        lastSyncUnix = getUnixTime();
    }
    if (stillBeConnected == true)
    {
        while (WiFi.status() == WL_CONNECTED)
        {
            syncNtp(false);
            int counter = 0;
            while (counter < SYNC_NTP_ON_CHARGING_DELAY)
            {
                delayTask(1000);
                counter = counter + 1000;
                if (bat.isCharging == false)
                {
                    syncNtp(false);
                    break;
                }
                if(WiFi.status() != WL_CONNECTED) {
                    break;
                }
            }
            lastTryUnix = getUnixTime();
        }
    }
    turnOffWifiMinimal();
    if(stillBeConnected == false) {
        resetSleepDelay(20000);
    } else {
        resetSleepDelay();
    }
    
    debugLog("Setting isWifiTaskRunning to false NOW");
    setBoolMutex(&wifiTaskMutex, &isWifiTaskRunning, false);
    vTaskDelete(NULL);
}

void turnOnWifi()
{
    // Regular turn on
    wifiConnectionTries = 9;
    stillBeConnected = false;
    if (isWifiTaskCheck() == false)
    {
        xTaskCreate(
            turnOnWifiTask,
            "wifiTask",
            40000,
            NULL,
            0,
            &wifiTask);
    }
}

void turnOnWifiPersistent()
{
    // Turn on but keep being connected and sync NTP
    wifiConnectionTries = 3;
    stillBeConnected = true;
    if (isWifiTaskCheck() == false)
    {
        xTaskCreate(
            turnOnWifiTask,
            "wifiTask",
            40000,
            NULL,
            0,
            &wifiTask);
    }
}

void turnOffWifiMinimal()
{
    WiFi.disconnect();
    WiFi.mode(WIFI_OFF);
}

void turnOffWifi()
{
    debugLog("Turning wifi off");
    if (isWifiTaskCheck() == true)
    {
        vTaskDelete(wifiTask);
        setBoolMutex(&wifiTaskMutex, &isWifiTaskRunning, false);
    }
    turnOffWifiMinimal();
}

#define MIN_RSSI -100
#define MAX_RSSI -40
int getSignalStrength()
{
    int rssi = WiFi.RSSI();
    // debugLog("Pure RSSI:" + String(rssi));
    if (rssi == 0)
    {
        return 0;
    }

    if (rssi < MIN_RSSI)
    {
        rssi = MIN_RSSI;
    }
    else if (rssi > MAX_RSSI)
    {
        rssi = MAX_RSSI;
    }

    int percentage = map(rssi, MIN_RSSI, MAX_RSSI, 0, 100);
    return percentage;
}

void regularSync()
{
    if (SYNC_WIFI == 1 && bat.isCharging == true && isWifiTaskCheck() == false)
    {
        if (getUnixTime() - lastSyncUnix > SYNC_WIFI_SINCE_SUCC)
        {
            debugLog("Regular sync going on");
            turnOnWifi();
            lastSyncUnix = getUnixTime();
        }
        else if (getUnixTime() - lastTryUnix > SYNC_WIFI_SINCE_FAIL)
        {
            debugLog("Persistent sync going on");
            turnOnWifiPersistent();
            lastTryUnix = getUnixTime();
        }
    }
    else
    {
        // debugLog("Not doing regular sync: " + String(getUnixTime() - lastSyncUnix) + " " + BOOL_STR(bat.isCharging));
    }
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

wifiStatusSimple wifiStatusWrap()
{
    // TODO: Is asking 2 times time consuming? The answer is no
    // debugLog("Milis before asking time status: " + String(millis()));
    // debugLog("Wifi status: " + wifiStatus());
    wl_status_t status = WiFi.status();
    // debugLog("Milis after asking time status: " + String(millis()));

    if (status == WL_CONNECTED)
    {
        return WifiConnected;
    }
    else
    {
        if (WiFi.getMode() == WIFI_OFF)
        {
            return WifiOff;
        }
        else
        {
            return WifiOn;
        }
    }
}

bool isWifiTaskCheck()
{
    wifiTaskMutex.lock();
    bool tmp = isWifiTaskRunning;
    wifiTaskMutex.unlock();
    return tmp;
}

#if DEBUG
void loopwifiDebug()
{
}
#endif
