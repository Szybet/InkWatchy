

This repo's format:

Bin folder contains the current bin that you can use esptool (.py or .exe) to install the current bin to the Watchy.

src folder contains a universal (Arduino & PlatformIO) setup for compilation, instructions on how to get it to compile are in the src folder.

Needed libraries:  Arduino, ArduinoOTA and Watchy base.

**NOTES:**
- Watchy uses version 1.0.6 of the ESP32 libraries, anything higher won't compile, remove them manually and revert back to version 1.0.6.
- PCF8563 RTCs are supported with the accompanying Library folder, be sure to set #define PCF8563RTC 1 in one of your files to activate that and be sure to follow the Compilation Instructions on this to ensure things work, any issues, submit Issues or pull requests to that file.

For instructions on usage of the firmware, see "Usage".
