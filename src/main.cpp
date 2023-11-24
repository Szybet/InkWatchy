#include "defines/defines.h"
#include "defines/macros.h"

void setup()
{
  setupHardware();
}

void loop()
{
#if DEBUG
  dumpButtons();
  showSetupResults();
#endif
  display.display(FULL_UPDATE);
  display.setFullWindow();
  delay(10000);
}
