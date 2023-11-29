#ifndef FUNCTIONSUI_H
#define FUNCTIONSUI_H

#include "defines/defines.h"

void setTextSize(int i);
void setFont(const GFXfont *fontTmp);
void writeLine(String strToWrite, int cursorX, uint16_t* currentHeight);
void writeTextReplaceBack(String str, int16_t x, int16_t y, uint16_t frColor = GxEPD_BLACK , uint16_t bgColor = GxEPD_WHITE, int tolerance = 5);
void centerText(String text, uint16_t* currentHeight, int x = -1);

extern GFXcanvas1 canvas;
extern const GFXfont *f;
extern int maxHeight;
extern int textSize;

#endif
