#include "fontPreview.h"

#if FONT_MENU_ENABLED

void initFontMenu()
{
    int itemsInDir = fsItemsInDir("/font/");
    entryMenu buttons[itemsInDir];

    File root = LittleFS.open("/font/");
    File file = root.openNextFile();
    int counter = 0;
    while (file)
    {
        if (file.isDirectory() == false)
        {
            buttons[counter] = {String(file.name()), &emptyImgPack, switchFontsPreview};
            counter = counter + 1;
        }
        file = root.openNextFile();
    }
    file.close();
    root.close();

    initMenu(buttons, counter, "Choose font");
}

void initFontPreview()
{
    dis->fillScreen(SCWhite);
    setFont(getFont(lastMenuSelected));
    setTextSize(1);
    dis->setTextWrap(true);
    String heighTest = "123";
    String preview = "1234567890abcdefghijklmnopqrstuvwxyz!\"#$%&'()*+,-./:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`qrstuvwxyz{|}~";
    uint16_t h;
    getTextBounds(heighTest, NULL, NULL, NULL, &h);
    dis->setCursor(0, h);
    dis->print(preview);
    disUp(true);
}

void loopFontPreview()
{
    useButtonBlank();
    resetSleepDelay();
    disUp();
}
#endif