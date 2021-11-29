

This repo's format:

Bin folder contains the current bin that you can use esptool (.py or .exe) to install the current bin to the Watchy.

src folder contains a universal (Arduino & PlatformIO) setup for compilation, instructions on how to get it to compile are in the src folder.

Needed libraries:  Arduino, ArduinoOTA and Watchy (1.2.8 or greater) base.

**NOTES:**
- Watchy uses version 1.0.6 of the ESP32 libraries, anything higher won't compile, remove them manually and revert back to version 1.0.6 or use 2.0.1 (which seems to have been fixed).
- Until SQFMI includes the WatchyBattery.h, be sure to download this as well, feel free to place it where you keep this code, or in anywhere the compiler can see it.

For instructions on usage of the firmware, see "Usage".
