#include "notesManager.h"

#if NOTES_APP

#define CONNECT_KEYBOARD "Connect keyboard"
#define NEW_NOTE "New note"

bool goingUp = false;
void noteSelected() {
    if(lastMenuSelected == CONNECT_KEYBOARD) {
        hostBleNotifyCallback = hostBleKeyboardNotify;
        goingUp = true;
        switchBluetoothHostScanner();
    } else if(lastMenuSelected == NEW_NOTE) {
        uint64_t unixTime = getUnixTime(timeRTCLocal);
        String filePath = String(NOTES_DIR) + "/" + unixToDate(unixTime) + " " + getHourMinuteUnix(unixTime);
        if(fsFileExists(filePath) == false) {
            LittleFS.open(filePath, "w", true);
            goingUp = true;
            initNotesManager();
        } else {
            debugLog("This note exists with this time");
        }
    } else {
        // TODO: check name if it is a file
        goingUp = true;
        switchNotes();
    }
}

cpuSpeed notesSpeedRestore = cpuSpeed::normalSpeed;
void initNotesManager() {
    if(goingUp == false) {
        notesSpeedRestore = getCpuSpeed();
        setCpuSpeed(cpuSpeed::maxSpeed);
    }
    goingUp = false;
    if(fsFileExists(NOTES_DIR) == false) {
        fsCreateDir(NOTES_DIR);
    }

    uint16_t count = fsItemsInDir(NOTES_DIR) + 2;

    entryMenu *entry = new entryMenu[count];
    if(bleClientConnected == false) {
        entry[0] = {CONNECT_KEYBOARD, &emptyImgPack, noteSelected};
    } else {
        entry[0] = {"Connected: " + hostBleClientName, &emptyImgPack, NULL};
    }
    entry[1] = {NEW_NOTE, &emptyImgPack, noteSelected};

    File root = LittleFS.open(NOTES_DIR);
    File file = root.openNextFile();
    uint16_t c = 2;
    while (file)
    {
        if (!file.isDirectory())
        {
            String fileName = String(file.name());
            entry[c] = {fileName, &emptyImgPack, noteSelected};
            c++;
        }
        file = root.openNextFile();
    }
    root.close();

    initMenu(entry, count, "Notes");
}

void loopNotesManager() {
    loopMenu();
}

void exitNotesManager() {
    // When fully exit only
    if(goingUp == false) {
        hostBleNotifyCallback = NULL;
        hostBleDeInitEverything();
        setCpuSpeed(notesSpeedRestore);
        notesSpeedRestore = cpuSpeed::normalSpeed;
    }
}
#endif
