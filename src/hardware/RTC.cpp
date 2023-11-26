#include "RTC.h"

tmElements_t UTC;


#ifdef DEBUG
void debugRTC () {
    SRTC.read(UTC);
    log("Second: " + String(UTC.Second));
    log("Minute: " + String(UTC.Minute));
    log("Hour: " + String(UTC.Hour));
    log("Day: " + String(UTC.Day));
    log("Month: " + String(UTC.Month));
    log("Year: " + String(UTC.Year));

}
#endif