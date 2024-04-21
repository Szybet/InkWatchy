#include "fontPreview.h"

#if FONT_MENU_ENABLED
void exitFontPreview() {

}

void initFontPreview() {
    display.fillScreen(GxEPD_WHITE);
    for(int i = 0; i < FONT_COUNT; i++) {
        if(lastMenuSelected.indexOf(fontListStr[i]) != -1) {
            setFont(&fontListRef[i]);
            break;
        }
    }
    setTextSize(1);
    display.setTextWrap(true);
    String heighTest = "123";
    String preview = "1234567890abcdefghijklmnopqrstuvwxyz!\"#$%&'()*+,-./:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`qrstuvwxyz{|}~";
    uint16_t h;
    getTextBounds(heighTest, NULL, NULL, NULL, &h);
    display.setCursor(0, h);
    display.print(preview);
    disUp(true);
}

void loopFontPreview() {

}
#endif