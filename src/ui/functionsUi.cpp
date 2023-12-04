#include "functionsUi.h"

// Default values... are set in screen
GFXcanvas1 canvas(200, 200);
const GFXfont *font;
uint16_t maxHeight;
int textSize;

void setTextSize(int i)
{
  textSize = i;
  display.setTextSize(i);
}

void setFont(const GFXfont *fontTmp)
{
  font = fontTmp;
  display.setFont(font);
}

void writeLine(String strToWrite, int cursorX, uint16_t *currentHeight)
{
  display.setCursor(cursorX, *currentHeight);
  display.print(strToWrite);
  *currentHeight = *currentHeight + maxHeight;
}

void centerText(String str, uint16_t *currentHeight)
{
  uint16_t w;
  getTextBounds(str, NULL, NULL, &w, NULL);
  int x = (display.width() - w) / 2;
  display.setCursor(x, *currentHeight);
  display.print(str);
  *currentHeight = *currentHeight + maxHeight;
}

void writeTextReplaceBack(String str, int16_t x, int16_t y, uint16_t frColor, uint16_t bgColor)
{
  log("Drawing bitmap with text: " + str + " at: " + String(x) + "x" + String(y));
  uint16_t w, h;
  getTextBounds(str, NULL, NULL, &w, &h);
  log("w: " + String(w));
  log("h: " + String(h));
  w = w + 5;
  if (containsBelowChar(str) == true)
  {
    GFXcanvas1 canvasTmp(w, h + 3);
    canvasTmp.setFont(font);
    canvasTmp.setTextSize(textSize);
    canvasTmp.setCursor(0, h - 3);
    canvasTmp.print(str);
    display.drawBitmap(x, y - h + 3, canvasTmp.getBuffer(), w, h + 3, frColor, bgColor); // this is relative to the cursor.
#if DRAW_DEBUG_RECT
    display.drawRect(x, y - h + 3, w, h + 3, frColor);
#endif
  }
  else
  {
    GFXcanvas1 canvasTmp(w, h + 3);
    canvasTmp.setFont(font);
    canvasTmp.setTextSize(textSize);
    canvasTmp.setCursor(0, h);
    canvasTmp.print(str);
    display.drawBitmap(x, y - h, canvasTmp.getBuffer(), w, h + 3, frColor, bgColor); // this is relative to the cursor.
#if DRAW_DEBUG_RECT
    display.drawRect(x, y - h, w, h + 3, frColor);
#endif
  }
}

void writeTextCenterReplaceBack(String str, uint16_t y, uint16_t frColor, uint16_t bgColor)
{
  uint16_t w, h;
  getTextBounds(str, NULL, NULL, &w, &h);
  log("w: " + String(w));
  log("h: " + String(h));
  w = w + 5;
  int16_t x = (display.width() - w) / 2;
  if (containsBelowChar(str) == true)
  {
    GFXcanvas1 canvasTmp(w, h + 3);
    canvasTmp.setFont(font);
    canvasTmp.setTextSize(textSize);
    canvasTmp.setCursor(0, h - 3);
    canvasTmp.print(str);
    display.drawBitmap(x, y - h + 3, canvasTmp.getBuffer(), w, h + 3, frColor, bgColor); // this is relative to the cursor.
#if DRAW_DEBUG_RECT
    display.drawRect(x, y - h + 3, w, h + 3, frColor);
#endif
  }
  else
  {
    GFXcanvas1 canvasTmp(w, h + 3);
    canvasTmp.setFont(font);
    canvasTmp.setTextSize(textSize);
    canvasTmp.setCursor(0, h);
    canvasTmp.print(str);
    display.drawBitmap(x, y - h, canvasTmp.getBuffer(), w, h + 3, frColor, bgColor); // this is relative to the cursor.
#if DRAW_DEBUG_RECT
    display.drawRect(x, y - h, w, h + 3, frColor);
#endif
  }
}

int16_t xS;
int16_t yS;
uint16_t wS;
uint16_t hS;
void getTextBounds(String &str, int16_t *xa, int16_t *ya, uint16_t *wa, uint16_t *ha, int16_t cxa, int16_t cya)
{
  int16_t cx = display.getCursorX();
  int16_t cy = display.getCursorY();
  display.setCursor(10, 100);
  xS = 0;
  yS = 0;
  wS = 0;
  hS = 0;
  display.getTextBounds(str, cxa, cya, &xS, &yS, &wS, &hS);
  if (xa != NULL)
  {
    *xa = xS;
  }
  if (ya != NULL)
  {
    *ya = yS;
  }
  if (wa != NULL)
  {
    *wa = wS;
  }
  if (ha != NULL)
  {
    *ha = hS;
  }
  display.setCursor(cx, cy);
}

// Use the image pack here to make it easier
// Like that:
// writeImageN(100, 100, testSomethiongImgPack);
void writeImageN(int16_t x, int16_t y, const uint8_t *bitmap, int16_t w, int16_t h, uint16_t frColor, uint16_t bgColor)
{
  int16_t cx = display.getCursorX();
  int16_t cy = display.getCursorY();
  display.setCursor(0, 0);
  display.drawBitmap(x, y, bitmap, w, h, frColor, bgColor);
  display.refresh(PARTIAL_UPDATE);
  display.setCursor(cx, cy);
}

void drawButton()
{
}
