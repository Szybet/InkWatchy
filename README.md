Watchy GSR Compile instructions (Arduino):

Go to the Arduino folder:

Inside Libraries Folder:  Open GxEPD2 folder, then src Folder.

Delete GxEPD2_BW.h  (or move it into Examples folder one level up).

Go into epd Folder:

Delete GxEPD2_154_D67.h & GxEPD2_154_D67.cpp (or move them both to Examples, 2 folder levels up).

Go back to the top of the Libraries folder:  Look for "arduino_######", check each folder's src folder for Watchy.h & Watchy.cpp, when you find them move them into Examples (one folder level up).

Download the GSR-211004a.zip and extract the GSR into the Streams folder inside the Arduino folder.

For platformio, you would need to make sure your Stream and your Libraries are at the same folder level, otherwise the re-linked GxEPD_BW and GxEPD_154_D67 files will not be able to find the other files included in the regular library.

This repo's format:

Stream folder contains the stream.
Archive folder contains the ZIP of the stream to be placed inside Arduino's Stream folder.
Bin folder contains the current bin that you can use esptool (.py or .exe) to install the current bin to the Watchy.
