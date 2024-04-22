#include "wifiLogic.h"

RTC_DATA_ATTR long lastSyncUnix = 0;
RTC_DATA_ATTR long lastTryUnix = 0;

void wifiSyncModules()
{
    syncWeather();
#if BITCOIN_MODULE
    bitcoinSync();
#endif
}

void wifiRegular()
{
    debugLog("Launching");
    syncNtp();
    wifiSyncModules();
    lastSyncUnix = getUnixTime();
}

void wifiPersistent()
{
    debugLog("Launching");
    while (WiFi.status() == WL_CONNECTED && bat.isCharging == true)
    {
        syncNtp(false);
        int counter = 0;
        while (counter < SYNC_NTP_ON_CHARGING_DELAY && WiFi.status() == WL_CONNECTED && bat.isCharging == true)
        {
            delayTask(1000);
            counter = counter + 1000;
        }
        lastTryUnix = getUnixTime();
    }
    if (WiFi.status() == WL_CONNECTED)
    {
        syncNtp(false);
        lastTryUnix = getUnixTime();
    }
}

void wifiKindOfPersistent() {
    debugLog("Launching");
    syncNtp(false); // Because we are connected to usb for some time now so the drift drifted too much in a bad way
    wifiSyncModules();
    lastSyncUnix = getUnixTime();
}

void turnOnWifiRegular()
{
    // Regular turn on
    createWifiTask(WIFI_CONNECTION_TRIES, wifiRegular, WIFI_PRIORITY_REGULAR);
}

void turnOnWifiPersistent()
{
    // Turn on but keep being connected and sync NTP
    createWifiTask(WIFI_CONNECTION_TRIES_PERSISTENT, wifiPersistent, WIFI_PRIORITY_PERSISTENT);
}

void turnOnWifiKindOfPersistent()
{
    createWifiTask(WIFI_CONNECTION_TRIES_PERSISTENT, wifiKindOfPersistent, WIFI_PRIORITY_PERSISTENT);
}

void regularSync()
{
    // debugLog("Entering regular sync");
    // debugLog("bat.isCharging:" + BOOL_STR(bat.isCharging));
    if (SYNC_WIFI == 1 && bat.isCharging == true && isWifiTaskCheck() == false)
    {
        if (getUnixTime() - lastSyncUnix > SYNC_WIFI_SINCE_SUCC)
        {
            debugLog("Regular sync going on");
            turnOnWifiRegular();
            lastSyncUnix = getUnixTime();
        }
        else if (getUnixTime() - lastTryUnix > SYNC_WIFI_SINCE_FAIL)
        {
            // We dont want persistent without regular first, so we launch a "in between" mode
            if (getUnixTime() - lastSyncUnix > SYNC_WIFI_SINCE_SUCC)
            {
                debugLog("Kind of persistent sync going on");
                turnOnWifiKindOfPersistent();
            }
            else
            {
                debugLog("Persistent sync going on");
                turnOnWifiPersistent();
            }
            lastTryUnix = getUnixTime();
        }
    }
    else
    {
        // debugLog("Not doing regular sync: " + String(getUnixTime() - lastSyncUnix) + " " + BOOL_STR(bat.isCharging));
    }
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