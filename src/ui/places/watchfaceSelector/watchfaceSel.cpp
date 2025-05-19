#include "watchfaceSel.h"
#include "rtcMem.h"

void watchfaceClicked() {
    for(int i = 0; i < WATCHFACE_COUNT; i++) {
        if(String(watchfacesList[i]->name) == lastMenuSelected) {
            rM.watchfaceSelected = i;
            watchfaceSelInit();
            return;
        }
    }
}

void watchfaceSelInit() {
    int c = 0;
    entryMenu buttons[WATCHFACE_COUNT];
    for(int i = 0; i < WATCHFACE_COUNT; i++) {
        if(watchfacesList[i]->manager != wfmNone) {
            ImageDef* choosedImage = &emptyImgPack;
            if(rM.watchfaceSelected == i) {
                choosedImage = getImg("cross");
            }
            buttons[c] = {.text = String(watchfacesList[i]->name), .image = choosedImage, .function = watchfaceClicked};
            c = c + 1;
        }
    }
    initMenu(buttons, c, WATCHFACE_MENU_SELECT, 1);
}