#pragma once

// The priority (order) here matters a bit
STATIC_WIFI_CRED wifi_credential1 = {.ssid = "", .password = ""};
STATIC_WIFI_CRED wifi_credential2 = {.ssid = "", .password = ""};
STATIC_WIFI_CRED wifi_credential3 = {.ssid = "", .password = ""};
STATIC_WIFI_CRED wifi_credential4 = {.ssid = "", .password = ""};
STATIC_WIFI_CRED wifi_credential5 = {.ssid = "", .password = ""};
STATIC_WIFI_CRED wifi_credential6 = {.ssid = "", .password = ""};
STATIC_WIFI_CRED wifi_credential7 = {.ssid = "", .password = ""};
STATIC_WIFI_CRED wifi_credential8 = {.ssid = "", .password = ""};
STATIC_WIFI_CRED wifi_credential9 = {.ssid = "", .password = ""};
STATIC_WIFI_CRED wifi_credential10 = {.ssid = "hotspot", .password = "12345678"};

const wifiQuickCred generalWifiQuick = {
    .ssid = "",                         // Replace with actual SSID
    .pass = "",                         // Replace with actual password
    .bssid = "",                        // Replace with actual BSSID, format like this: 00:00:00:00:00:00
    .ip = IPAddress(0, 0, 0, 0),        // Static IP
    .gateway = IPAddress(0, 0, 0, 0),   // Gateway IP
    .subnet = IPAddress(0, 0, 0, 0),    // Subnet IP
    .dns1 = IPAddress(1, 1, 1, 1),      // DNS1 IP
    .dns2 = IPAddress(8, 8, 8, 8),      // DNS2 IP
    .connectTo = IPAddress(0, 0, 0, 0), // IP address to connect to
    .port = 54321                       // Port to connect to
};

// Connect to and port here doesn't matter for alarms
const wifiQuickCred alarmWifiQuick = {
    .ssid = "",                         // Replace with actual SSID
    .pass = "",                         // Replace with actual password
    .bssid = "",                        // Replace with actual BSSID, format like this: 00:00:00:00:00:00
    .ip = IPAddress(0, 0, 0, 0),        // Static IP
    .gateway = IPAddress(0, 0, 0, 0),   // Gateway IP
    .subnet = IPAddress(0, 0, 0, 0),    // Subnet IP
    .dns1 = IPAddress(1, 1, 1, 1),      // DNS1 IP
    .dns2 = IPAddress(8, 8, 8, 8),      // DNS2 IP
    .connectTo = IPAddress(0, 0, 0, 0), // IP address to connect to
    .port = 54321                       // Port to connect to
};

// Get those values here: https://open-meteo.com/en/docs/geocoding-api
// For hamburg, germany example values:
#define WEATHER_LATIT ""   // 53.55073
#define WEATHER_LONGTIT "" // 9.99302

#define VAULT_PASSWORD "" // This must be a numerical PIN, don't start it with 0

// If you want just a path, implement it yourself, look up the script and the program arguments
// Link to your ICS file to download it
// you can also link multiple ICS for example
//#define CALENDAR_URL "web.com/one.ics"  
//#define CALENDAR_URL "web.com/second.ics"  
//if your website doesnt have proper ssl certificate or self signed certificate dont forget to allow ALLOW_INSECURE_CAL 1 in config.h
#define CALENDAR_URL "" // https://MyIcsFileSomewhere.toDownload

