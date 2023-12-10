#include "wifi.h"

WiFiMulti wifiMulti;
TaskHandle_t wifiTask;
bool isWifiTaskRunning = false;

void initWifi()
{
    if (WIFI_SSID1 != "" && WIFI_PASS1 != "")
    {
        log("Attempting to connect to WiFi " + String(WIFI_SSID1));
        wifiMulti.addAP(WIFI_SSID1, WIFI_PASS1);
    }
    if (WIFI_SSID2 != "" && WIFI_PASS2 != "")
    {
        log("Attempting to connect to WiFi " + String(WIFI_SSID2));
        wifiMulti.addAP(WIFI_SSID2, WIFI_PASS2);
    }
    if (WIFI_SSID3 != "" && WIFI_PASS3 != "")
    {
        log("Attempting to connect to WiFi " + String(WIFI_SSID3));
        wifiMulti.addAP(WIFI_SSID3, WIFI_PASS3);
    }
    if (WIFI_SSID4 != "" && WIFI_PASS4 != "")
    {
        log("Attempting to connect to WiFi " + String(WIFI_SSID4));
        wifiMulti.addAP(WIFI_SSID4, WIFI_PASS4);
    }
    if (WIFI_SSID5 != "" && WIFI_PASS5 != "")
    {
        log("Attempting to connect to WiFi " + String(WIFI_SSID5));
        wifiMulti.addAP(WIFI_SSID5, WIFI_PASS5);
    }
    if (WIFI_SSID6 != "" && WIFI_PASS6 != "")
    {
        log("Attempting to connect to WiFi " + String(WIFI_SSID6));
        wifiMulti.addAP(WIFI_SSID7, WIFI_PASS7);
    }
    if (WIFI_SSID7 != "" && WIFI_PASS7 != "")
    {
        log("Attempting to connect to WiFi " + String(WIFI_SSID7));
        wifiMulti.addAP(WIFI_SSID7, WIFI_PASS7);
    }
    if (WIFI_SSID8 != "" && WIFI_PASS8 != "")
    {
        log("Attempting to connect to WiFi " + String(WIFI_SSID8));
        wifiMulti.addAP(WIFI_SSID8, WIFI_PASS8);
    }
    if (WIFI_SSID9 != "" && WIFI_PASS9 != "")
    {
        log("Attempting to connect to WiFi " + String(WIFI_SSID9));
        wifiMulti.addAP(WIFI_SSID9, WIFI_PASS9);
    }
    if (WIFI_SSID10 != "" && WIFI_PASS10 != "")
    {
        log("Attempting to connect to WiFi " + String(WIFI_SSID10));
        wifiMulti.addAP(WIFI_SSID10, WIFI_PASS10);
    }
}

void turnOnWifiTask(void *parameter)
{
    log("Turning wifi on");
    isWifiTaskRunning = true;
    for (int i = 0; i < 15; i++)
    {
        WiFi.mode(WIFI_STA);
        WiFi.setSleep(WIFI_PS_NONE);
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
            log("Wifi failed to connect, retrying...");
            turnOffWifi();
        }
    }
    syncNtp();
    syncWeather();
    isWifiTaskRunning = false;
    vTaskDelete(NULL);
}

void turnOnWifi()
{
    if (isWifiTaskRunning == false)
    {
        xTaskCreate(
            turnOnWifiTask,
            "wifiTask",
            10000,
            NULL,
            0,
            &wifiTask);
    }
}

void turnOffWifi()
{
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
    //log("Pure RSSI:" + String(rssi));
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
    // log("Milis before asking time status: " + String(millis()));
    log("Wifi status: " + wifiStatus());
    wl_status_t status = WiFi.status();
    // log("Milis after asking time status: " + String(millis()));

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
