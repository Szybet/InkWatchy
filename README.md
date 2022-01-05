This repo's format:

Bin folder contains the current bin that you can use esptool (.py or .exe) to install the current bin to the Watchy.

src folder contains a universal (Arduino & PlatformIO) setup for compilation, instructions on how to get it to compile are in the src folder.

Needed libraries:  Arduino Libraries, ArduinoOTA, SmallRTC (GuruSR), SmallNTP (GuruSR), Olson2POSIX (GuruSR) AND Watchy (1.2.9 or greater) base.

**NOTES:**
- Watchy uses version 1.0.6 of the ESP32 libraries, anything higher won't compile, remove them manually and revert back to version 1.0.6 or use 2.0.1/2 (which seems to have been fixed).

For instructions on usage of the firmware, see "Usage".

For those **WANTING** to use this Watchy code without programming and having to install Arduino or PlatformIO, you *CAN* install it with the base Watchy face you received when you got your Watchy.

1. Download the Bin folder's file `GSR.ino.esp32.bin` onto the machine that you can connect your Watchy to by WiFi (laptop, cellphone, tablet, etc).
2. Go into the Watchy's menu, push up (so it goes up to the bottom) until you stop on Setup WiFi.  Press Menu to continue.
3. Find the WiFi `"Watchy AP"` on your device and connect to it.
4. Once connected, go to a web browser and type in the IP address in the address bar and press Enter.
5. You'll see a button called `"Update"`, click that.
6. Press the Browse button and locate and select the `GSR.ino.esp32.bin` you downloaded in Step 1.
7. Click the Update/Upload button on the right and wait.
8. If all went well, you'll see the success message and your Watchy will reboot with this Watchy face.
9. If you didn't get this far, then you either didn't connect to Watchy AP fast enough or something else happened, so try again at Step 2.
