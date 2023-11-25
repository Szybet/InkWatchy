#include "defines/defines.h"
#include "defines/macros.h"

void setup()
{
  setupHardware();
  initBattery();
  initDebugDisplay();
}

void loop()
{  
  loopBattery();

  log(BOOL_STR(true));

  loopDebugDisplay();
#if DEBUG
  dumpButtons();
  showSetupResults();
#endif
}
