#include "defines/defines.h"
#include "defines/macros.h"

void setup() {
  setupHardware();

}

void loop() {
  isDebug(dumpButtons());
}

