#include "functionsUi.h"

// Default values... are set in screen
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
  // debugLog("Drawing bitmap with text: " + str + " at: " + String(x) + "x" + String(y));
  uint16_t w, h;
  getTextBounds(str, NULL, NULL, &w, &h);
  // debugLog("w: " + String(w));
  // debugLog("h: " + String(h));
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
  // debugLog("w: " + String(w));
  // debugLog("h: " + String(h));
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
void writeImageN(int16_t x, int16_t y, const ImageDef image, uint16_t frColor, uint16_t bgColor)
{
  int16_t cx = display.getCursorX();
  int16_t cy = display.getCursorY();
  display.setCursor(0, 0);
  display.drawBitmap(x, y, image.bitmap, image.bw, image.bh, frColor, bgColor);
  display.refresh(PARTIAL_UPDATE);
  display.setCursor(cx, cy);
}

sizeInfo drawButton(int16_t x, int16_t y, String str, const ImageDef image, bool invert, int tolerance, int borderWidth, uint16_t frColor, uint16_t bgColor)
{
  sizeInfo size = {0};
  int toleranceSize = tolerance * 2 + borderWidth * 2;
  size.w = toleranceSize;
  size.h = toleranceSize;

  uint16_t tw = 0;
  uint16_t th = 0;
  if (str != "")
  {
    getTextBounds(str, NULL, NULL, &tw, &th);
    size.w += tw;
    size.w += 1;
  }

  if (image.bitmap != NULL)
  {
    size.w += image.bw;
  }

  if (image.bitmap != NULL && str != "")
  {
    size.w += tolerance;
  }

  uint16_t mainH;
  if (image.bitmap != NULL && image.bh > th)
  {
    size.h += image.bh;
    mainH = image.bh;
  }
  else
  {
    size.h += th;
    mainH = th;
  }

  // debugLog("Final size.h is: " + String(size.h));
  // debugLog("Final size.w is: " + String(size.w));

  GFXcanvas1 canvasTmp(size.w, size.h);
  canvasTmp.setFont(font);
  canvasTmp.setTextSize(textSize);
  canvasTmp.setCursor(tolerance + borderWidth, tolerance + borderWidth);
  if (image.bitmap != NULL)
  {
    canvasTmp.drawBitmap(tolerance + borderWidth, tolerance + borderWidth, image.bitmap, image.bw, image.bh, frColor, bgColor);
    if (str != "")
    {
      canvasTmp.setCursor(canvasTmp.getCursorX() + tolerance + image.bw, canvasTmp.getCursorY());
    }
  }
  if (str != "")
  {
    int yCursorTmp = size.h - ((size.h - th) / 2) - 1;
    if (containsBelowChar(str) == true)
    {
      yCursorTmp = yCursorTmp - 3;
    }
    canvasTmp.setCursor(canvasTmp.getCursorX(), yCursorTmp); // -1 for reasons? -3 for cutted j g etc.
    // debugLog("Printing text to canvas: " + str + " at: " + String(canvasTmp.getCursorX()));
    canvasTmp.print(str);
  }

  int16_t canvasCursorXTmp = canvasTmp.getCursorX();
  int16_t canvasCursorYTmp = canvasTmp.getCursorY();
  int16_t rectOffset = 0;
  canvasTmp.setCursor(0, 0);
  uint16_t borderColor = bgColor;
  if (invert == true)
  {
    borderColor = frColor;
  }
  for (int16_t i = 0; i < borderWidth; i++)
  {
    // debugLog("Drawing border in button");
    canvasTmp.drawRect(i + rectOffset, i + rectOffset, size.w - rectOffset, size.h - rectOffset, borderColor);
  }
  canvasTmp.setCursor(canvasCursorXTmp, canvasCursorYTmp);

  int16_t cx = display.getCursorX();
  int16_t cy = display.getCursorY();
  display.setCursor(0, 0);
  if (invert == false)
  {
    // debugLog("Drawing non inverted button");
    display.drawBitmap(x, y, canvasTmp.getBuffer(), size.w, size.h, frColor, bgColor); // why inverted?
  }
  else
  {
    display.drawBitmap(x, y, canvasTmp.getBuffer(), size.w, size.h, bgColor, frColor);
    // display.drawInvertedBitmap(x, y, canvasTmp.getBuffer(), size.w, size.h, frColor);
  }
  display.setCursor(cx, cy);
  return size;
}
