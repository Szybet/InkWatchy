#include "wifiDebug.h"

int statusLine;
int ipLine;
int ssidLine;
int signalStrengthLine;
int taskStatusLine;
int macAddressLine;

String previousMac;
String wifiStatusLastStr;
String taskStatusLastStr;
int wifiSignalLast;

void wifiOnBtn()
{
    turnOnWifiRegular();
    vibrateMotor(VIBRATION_ACTION_TIME);
}

void wifiOffBtn()
{
    turnOffWifi();
    vibrateMotor(VIBRATION_ACTION_TIME);
}

void wifiNtpBtn()
{
    turnOnWifiNtpOnly();
    vibrateMotor(VIBRATION_ACTION_TIME);
}

void initWifiDebugDisplay()
{
    init_general_page(6);
    general_page_set_title(DEBUG_MENU_WIFI);
    genpage_set_center();

    GeneralPageButton button[] = {GeneralPageButton{DEBUG_WIFI_ON, wifiOnBtn}, GeneralPageButton{DEBUG_WIFI_OFF, wifiOffBtn}, GeneralPageButton("NTP", wifiNtpBtn)};
    general_page_set_buttons(button, 3);

    genpage_add(DEBUG_WIFI_MAC_ADDRESS);
    if (wifiStatusWrap() != wifiStatusSimple::WifiOff)
    {
        previousMac = WiFi.macAddress();
    }
    else
    {
        previousMac = "";
    }
    macAddressLine = genpage_add(previousMac.c_str());

    genpage_add(DEBUG_WIFI_STATUS);
    wifiStatusLastStr = wifiStatus();
    statusLine = genpage_add(wifiStatusLastStr.c_str());

    if (wifiStatusWrap() != wifiStatusSimple::WifiOff)
    {
        ipLine = genpage_add(String(DEBUG_WIFI_IP + WiFi.localIP().toString()).c_str());
        ssidLine = genpage_add(String(DEBUG_WIFI_SSID + WiFi.SSID()).c_str());
        signalStrengthLine = genpage_add(String(DEBUG_WIFI_SIGNAL + String(getSignalStrength()) + DEBUG_COMMON_PERCENT).c_str());
    }
    else
    {
        ipLine = genpage_add(String(DEBUG_WIFI_IP).c_str());
        ssidLine = genpage_add(String(DEBUG_WIFI_SSID).c_str());
        signalStrengthLine = genpage_add((String(DEBUG_WIFI_SIGNAL) + String("0") + String(DEBUG_COMMON_PERCENT)).c_str());
    }

    taskStatusLastStr = BOOL_STR(isWifiTaskCheck());
    taskStatusLine = genpage_add(String(DEBUG_WIFI_CONNECTING + taskStatusLastStr).c_str());

    general_page_set_main();
}

void loopWifiDebugDisplay()
{
    if (genpage_is_menu() == false)
    {
        if (wifiStatusWrap() != wifiStatusSimple::WifiOff)
        {
            String macNow = WiFi.macAddress();
            if (previousMac != macNow)
            {
                previousMac = macNow;
                genpage_change(macNow.c_str(), macAddressLine);
            }

            String wifiStatusStr = wifiStatus();
            if (wifiStatusStr != wifiStatusLastStr)
            {
                wifiStatusLastStr = wifiStatusStr;
                genpage_change(wifiStatusLastStr.c_str(), statusLine);
                genpage_change(String(DEBUG_WIFI_IP + WiFi.localIP().toString()).c_str(), ipLine);
                genpage_change(String(DEBUG_WIFI_SSID + WiFi.SSID()).c_str(), ssidLine);
            }

            int wifiSignal = getSignalStrength();
            if (wifiSignal != wifiSignalLast)
            {
                wifiSignalLast = wifiSignal;
                genpage_change(String(DEBUG_WIFI_SIGNAL + String(wifiSignal) + DEBUG_COMMON_PERCENT).c_str(), signalStrengthLine);
            }
        }

        String wifiTaskStatus = BOOL_STR(isWifiTaskCheck());
        if (wifiTaskStatus != taskStatusLastStr)
        {
            taskStatusLastStr = wifiTaskStatus;
            genpage_change(wifiTaskStatus.c_str(), taskStatusLine);
        }
    }

    // Never exit this wifi is on or connected
    if (isWifiTaskCheck() == true)
    {
        resetSleepDelay();
    }

    general_page_set_main();
    slint_loop();
}
