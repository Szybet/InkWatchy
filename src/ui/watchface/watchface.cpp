#include "watchface.h"

void initWatchfaceDisplay() {
    display.setCursor(100, 100);
    display.print("wololo");
    disUp(true); // uzywaj tej funkcji zamiast display.display, zobacz sobie inne debug menu jak je zmodyfikowalem by dzialaly z tym
}

void loopWatchfaceLoop() {
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
}
