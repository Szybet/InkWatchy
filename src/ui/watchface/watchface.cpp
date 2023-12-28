#include "watchface.h"

#define TIME_CORD 10, 56
#define DAY_NAME_CORD 16, 79
#define DATE_CORD 8, 95
#define MONTH_NAME_CORD 54, 97
#define MONTH_NUMBER_1_CORD 89, 93
#define MONTH_NUMBER_2_CORD 89, 102

void initWatchfaceDisplay() {
    writeImageN(0,0, watchfaceImgPack);
    readRTC(true);

    setFont(&ticketing_regular40pt7b);
    writeTextReplaceBack(getHourMinute(), TIME_CORD);

    disUp(true);
}

void loopWatchfaceLoop() {
  readRTC();



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
