#ifndef FUNCTIONSUI_H
#define FUNCTIONSUI_H

#include "defines/defines.h"

#define UItolerance 3

void setTextSize(int i);
void setFont(const GFXfont *fontTmp);
void writeLine(String strToWrite, int cursorX, uint16_t* currentHeight);
void writeTextReplaceBack(String str, int16_t x, int16_t y, uint16_t frColor = GxEPD_BLACK , uint16_t bgColor = GxEPD_WHITE, int tolerance = UItolerance);
void centerText(String str, uint16_t* currentHeight);
void writeTextCenterReplaceBack(String str, uint16_t y,uint16_t frColor = GxEPD_BLACK , uint16_t bgColor = GxEPD_WHITE, int tolerance = UItolerance);
void getTextBounds(String &str, int16_t *xa, int16_t *ya, uint16_t *wa, uint16_t *ha, int16_t cxa = 1, int16_t cya = 100);
void writeImageN(int16_t x, int16_t y, const uint8_t *bitmap, int16_t w, int16_t h, uint16_t frColor = GxEPD_WHITE , uint16_t bgColor = GxEPD_BLACK);

extern GFXcanvas1 canvas;
extern const GFXfont *f;
extern uint16_t maxHeight;
extern int textSize;

#endif
