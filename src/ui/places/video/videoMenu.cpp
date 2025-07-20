#include "video.h"

#if VIDEO_PLAYER

// External declarations (these need to be declared somewhere accessible)
extern String lastMenuSelected;
extern void switchVideoPlayer();
extern ImageDef emptyImgPack;

void choosedVideo() {
    videoName = lastMenuSelected;
    switchVideoPlayer();
}

void initVideoMenu() {
    // First pass: count directories
    File root = LittleFS.open("/videos/");
    File file = root.openNextFile();
    int itemsInDir = 0;
    while (file) {
        if (file.isDirectory() == true) {
            itemsInDir++;
        }
        file = root.openNextFile();
    }
    file.close();
    root.close();

    // Create array with exact size
    entryMenu buttons[itemsInDir];

    // Second pass: fill the array
    root = LittleFS.open("/videos/");
    file = root.openNextFile();
    int counter = 0;
    while (file)
    {
        if (file.isDirectory() == true)
        {
            buttons[counter] = {String(file.name()), &emptyImgPack, choosedVideo};
            counter = counter + 1;
        }
        file = root.openNextFile();
    }
    file.close();
    root.close();

    initMenu(buttons, counter, VIDEO_PLAYER_CHOOSE);
}

#endif