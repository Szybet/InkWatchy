#include <WatchyRTC.h>
#ifndef WatchyBattery_H
#define WatchyBattery_H

class WatchyBatt {
    public:
        float Read(WatchyRTC RTC){
            if (RTC.rtcType == DS3231)
                return analogRead(33);
             else if (RTC.rtcType == PCF8563)
                return analogRead(35);
             return 0.0;
        };
};
#endif
