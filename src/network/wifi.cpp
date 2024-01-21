#include "wifi.h"

WiFiMulti wifiMulti;
TaskHandle_t wifiTask;
bool isWifiTaskRunning = false;

void initWifi()
{
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
}

RTC_DATA_ATTR long lastSyncUnix = 0;
void turnOnWifiTask(void *parameter)
{
    debugLog("Turning wifi on");
    isWifiTaskRunning = true;
    for (int i = 0; i < 15; i++)
    {
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
            turnOffWifi();
        }
    }
    if (WiFi.status() == WL_CONNECTED)
    {
        syncNtp();
        syncWeather();
        lastSyncUnix = getUnixTime();
    }
    sleepDelayMs = millis(); // reset sleep delay
    isWifiTaskRunning = false;
    turnOffWifi();
    vTaskDelete(NULL);
}

void turnOnWifi()
{
    if (isWifiTaskRunning == false)
    {
        xTaskCreate(
            turnOnWifiTask,
            "wifiTask",
            50000,
            NULL,
            0,
            &wifiTask);
    }
}

void turnOffWifi()
{
    debugLog("Turning wifi off");
    if (isWifiTaskRunning == true)
    {
        vTaskDelete(wifiTask);
        isWifiTaskRunning = false;
    }
    WiFi.disconnect();
    WiFi.mode(WIFI_OFF);
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
    if (SYNC_WIFI == 1 && (getUnixTime() - lastSyncUnix > SYNC_WIFI_SINCE_LAST_DELAY_S) && bat.isCharging == true)
    {
        if (isWifiTaskRunning == false)
        {
            debugLog("Regular sync going on");
            turnOnWifi();
        }
    }
    else
    {
        //debugLog("Not doing regular sync: " + String(getUnixTime() - lastSyncUnix) + " " + BOOL_STR(bat.isCharging));
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

bool isWifiConnected()
{
    // TODO: Is asking 2 times time consuming? The answer is no
    // debugLog("Milis before asking time status: " + String(millis()));
    debugLog("Wifi status: " + wifiStatus());
    wl_status_t status = WiFi.status();
    // debugLog("Milis after asking time status: " + String(millis()));

    if (status == WL_CONNECTED)
    {

        return true;
    }
    else
    {
        return false;
    }
}

#if DEBUG
void loopwifiDebug()
{
}
#endif
