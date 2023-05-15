![Watchy GSR Face](https://github.com/GuruSR/Watchy_GSR/blob/main/Images/Watchy_GSR.gif)

This repo's format:

Bin folder contains the current OTA Bin that you can use a tool to upload with or an OTA Upgrade capable firmware to install the current OTA Bin to the Watchy.

src folder contains a universal (Arduino & PlatformIO) setup for compilation, instructions on how to get it to compile are in the src folder.

WatchFace Addons contains an EmptyAddOn.h (so you can use it as a template for making your own) as well as 2 stock Watchy_GSR WatchFaces (Ballsy & LCD).

Needed libraries:  Arduino Libraries, ArduinoOTA (included in ESP32 2.0.2), SmallRTC (1.6 or greater, GuruSR), SmallNTP (GuruSR), StableBMA (GuruSR), Olson2POSIX (GuruSR) AND Watchy (1.3.3 or greater) base.

**NOTES:**
- Watchy uses version 1.0.6 or *2.0.5* (or higher) of the ESP32 libraries, 2.0.0 won't compile, 2.0.3 and 2.0.4 causes a boot crash loop, remove them manually and revert back to version 1.0.6 or use 2.0.5 (or higher).

**WARNING:**
As of Version 1.4.3H, the Compilation Instructions have changed, the GxEPD2 requires a define to be added to the .h file in the Compilation Instructions or the Dark Border will be disabled.

For instructions on usage of the firmware, see "Usage".

Below is a layout of the Watchy Connect & the OTA Website (used while online your network) that gives you options to various things.


![OTA Website](https://github.com/GuruSR/Watchy_GSR/blob/main/Images/Server-Help.png)

OTA update is **ONLY** possible via an OTA compatible firmware which are:

`Watchy GSR` and those overriding it.

**If you have an OTA upload function in your firmware, ask for it to be added here.**

For those wanting to override the looks of this Watchy face, look at the Override Information.

**First Timers**

To get this setup for compiling, you need to make a folder in your (Arduino) Stream folder or (PlatformIO) project folder, call it GSR, in there, download the "Starting Point.zip" and latest Release (Source Code) .zip from the Releases.
Extract both "Starting Point.zip" and the latest release (Source Code .zip) into the GSR folder, this will ensure all the necessary files are there.  If you plan to download any AddOns, be sure to deposit those files into the same GSR folder.  Follow the instructions for including AddOns in the src folder's Compilation Instructions.
