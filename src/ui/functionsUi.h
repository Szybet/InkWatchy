#pragma once

#include "defines.h"

extern const GFXfont *font;
extern uint16_t maxHeight;
extern int textSize;

void setTextSize(int i);
void setFont(const GFXfont *fontTmp);
void writeLine(String strToWrite, int cursorX, uint16_t *currentHeight);
void writeTextReplaceBack(String str, int16_t x, int16_t y, uint16_t frColor = GxEPD_BLACK, uint16_t bgColor = GxEPD_WHITE, bool manualWidth = false, uint8_t manualWidthAdd = 1, uint8_t manualHeighAdd = 3);
void centerText(String str, uint16_t *currentHeight);
void writeTextCenterReplaceBack(String str, uint16_t y, uint16_t frColor = GxEPD_BLACK, uint16_t bgColor = GxEPD_WHITE);
void getTextBounds(String &str, int16_t *xa, int16_t *ya, uint16_t *wa, uint16_t *ha, int16_t cxa = 1, int16_t cya = 100);
void writeImageN(int16_t x, int16_t y, ImageDef* image, uint16_t frColor = GxEPD_WHITE, uint16_t bgColor = GxEPD_BLACK);
sizeInfo drawButton(int16_t x, int16_t y, String str, ImageDef* image = &emptyImgPack, bool invert = false, int tolerance = 2, int borderWidth = 1, uint16_t frColor = GxEPD_BLACK, uint16_t bgColor = GxEPD_WHITE, bool draw = true, const GFXfont *buttonFont = font);
void simpleCenterText(String text);
void textPage(String title, String *strList, int listCount, const GFXfont * customFont);
void drawProgressBar(int x, int y, int width, int height, int progress);
sizeInfo drawTextSimple(String text, String font, int16_t x, int16_t y);
void wLineStr(String *dest, String *text);