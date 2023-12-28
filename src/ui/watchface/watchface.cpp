#include "watchface.h"

#define TIME_CORD 10, 53
#define DAY_NAME_CORD 16, 79
#define DATE_CORD 8, 95
#define MONTH_NAME_CORD 54, 97
#define MONTH_NUMBER_1_CORD 89, 93
#define MONTH_NUMBER_2_CORD 89, 102

#define TIME_FONT &JackInput40pt7b
RTC_DATA_ATTR tmElements_t watchfaceTime;

void initWatchfaceDisplay() {
    debugLog("Executing init watch face");
    writeImageN(0,0, watchfaceImgPack);

    setTextSize(1);
    setFont(TIME_FONT);
    writeTextReplaceBack(getHourMinute(), TIME_CORD);

    disUp(true);
    watchfaceTime = timeRTC;
}

void loopWatchfaceLoop() {
  debugLog("Executing loop watch face");
  if(watchfaceTime.Minute != timeRTC.Minute || watchfaceTime.Hour != timeRTC.Hour) {

    setTextSize(1);
    setFont(TIME_FONT);
    writeTextReplaceBack(getHourMinute(), TIME_CORD);

    watchfaceTime.Hour = timeRTC.Hour;
    watchfaceTime.Minute = timeRTC.Minute;
    dUChange = true;
  }

  switch (useButton())
  {
  case Up:
  {
    break;
  }
  case Down:
  {
    break;
  }
  case Menu:
  {
    generalSwitch(mainMenu);
    break;
  }
  case LongUp:
  {

    break;
  }
  case LongDown:
  {

    break;
  }
  }

  disUp();
}
