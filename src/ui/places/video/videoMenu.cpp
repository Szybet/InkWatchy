#include "video.h"

#if VIDEO_PLAYER

void choosedVideo() {
    videoName = lastMenuSelected;
    switchVideoPlayer();
}

void initVideoMenu() {
    int itemsInDir = fsItemsInDir("/videos/");
    entryMenu buttons[itemsInDir];

    File root = LittleFS.open("/videos/");
    File file = root.openNextFile();
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

    initMenu(buttons, counter, "Choose video");
}

#endif