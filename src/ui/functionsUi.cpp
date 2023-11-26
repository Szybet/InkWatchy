#include "functionsUi.h"

GFXcanvas1 canvas(200, 200);
const GFXfont *font;
int maxHeight;
int textSize;

void setTextSize(int i) {
    textSize = i;
    display.setTextSize(i);
}

void setFont(const GFXfont *fontTmp) {
    font = fontTmp;
    display.setFont(font);
}

void writeLine(String strToWrite, int cursorX, uint16_t* currentHeight) {
    display.setCursor(cursorX, *currentHeight);
    display.print(strToWrite);
    *currentHeight = *currentHeight + maxHeight;
}

void writeTextReplaceBack(String str, int16_t x, int16_t y, uint16_t frColor, uint16_t bgColor, int tolerance) {
    log("Drawing bitmap with text: " + str);

    uint16_t w, h;
    int16_t xt, yt; // tmp, don't need it
    display.getTextBounds(str, 1, 100, &xt, &yt, &w, &h);
    log("w: " + String(w));
    log("h: " + String(h));
    w = w + tolerance;
    h = h;
    GFXcanvas1 canvasTmp(w, h);
    canvasTmp.fillScreen(false); // Clear canvas
    canvasTmp.setFont(font);
    canvasTmp.setTextSize(textSize);
    canvasTmp.setCursor(0, h - 1);
    canvasTmp.print(str);
    display.drawBitmap(x, y - h + (tolerance / 3), canvasTmp.getBuffer(), w, h, frColor, bgColor);
}
