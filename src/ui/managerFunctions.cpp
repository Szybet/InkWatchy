#include "manager.h"

void initMainMenu()
{
    entryMenu buttons[1] = {{"Debug", debugImgPack, switchDebugMenu}};
    initMenu(buttons, 1, "Main menu", 1);
}

void initDebugMenu() {
    entryMenu buttons[3] = {{"General", emptyImgPack, NULL},{"Battery", emptyImgPack, NULL},{"Wifi", emptyImgPack, NULL}};
    initMenu(buttons, 3, "Debug menu", 1);
}
