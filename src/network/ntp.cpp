#include "ntp.h"

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);

void syncNtp() {
    timeClient.begin();
    timeClient.update();
    timeClient.setTimeOffset(TIME_OFFSET_S);
    debugLog("NTP time: " + timeClient.getFormattedTime());
    breakTime(timeClient.getEpochTime(), timeRTC);
    saveRTC();
    timeClient.end();
}
