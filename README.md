
This repo's format:

Bin folder contains the current bin that you can use esptool (.py or .exe) to install the current bin to the Watchy.

src folder contains a universal (Arduino & PlatformIO) setup for compilation, instructions on how to get it to compile are in the src folder.

Needed libraries:  Arduino Libraries, ArduinoOTA, SmallRTC (GuruSR), SmallNTP (GuruSR), Olson2POSIX (GuruSR) AND Watchy (1.2.9 or greater) base.

**NOTES:**
- Watchy uses version 1.0.6 of the ESP32 libraries, anything higher won't compile, remove them manually and revert back to version 1.0.6 or use 2.0.1 (which seems to have been fixed).

For instructions on usage of the firmware, see "Usage".
