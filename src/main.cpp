#include "defines/defines.h"

void setup()
{
  setupHardware();
  initBattery();
  initDebugDisplay();
}

void loop()
{  
  loopBattery();

  loopDebugDisplay();
#if DEBUG
  dumpButtons();
  showSetupResults();
  debugRTC();
#endif
  delay(LOOP_DELAY);
}
