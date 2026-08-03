#include "cyber.h"

#define DEG_RAD 0.01745329252f

int cyberCenterTextXOffset(String str)
{
  uint16_t w, h;
  getTextBounds(str, NULL, NULL, &w, &h);
  uint16_t wHalf = w / 2;
  debugLog(String(wHalf));
  return wHalf;
}

int cyberRightTextXOffset(String str)
{
  uint16_t w, h;
  getTextBounds(str, NULL, NULL, &w, &h);
  return w;
}

// delightfully borrowed from Orbital watchface
static std::pair<float, float> calcArcPoint(float deg, int x, int y, int radius)
{
  float cx = x + (radius)*cosf(deg * DEG_RAD);
  float cy = y + (radius)*sinf(deg * DEG_RAD);
  return std::make_pair(cx, cy);
}
void cyberDrawArc(float startDeg, float end, int x, int y, int radius)
{
  const float d2r = 0.01745329252f;

  // Redraw the arcs
  for (float i = 0; i <= end; i++)
  {
    // float cx = x + (radius - 1/2.0f) * cosf((i-startDeg) * d2r);
    // float cy = y + (radius - 1/2.0f) * sinf((i-startDeg) * d2r);
    std::pair<float, float> xy = calcArcPoint((i - startDeg), x, y, radius);
    dis->drawPixel(xy.first, xy.second, SCBlack);
  }
}

void cyberDrawPctGraph(float pct, int x, int y)
{
  const float d2r = 0.01745329252f;
  const int radius = 13;
  const float arcStart = 225.0f;
  if (pct < 0)
  {
    pct = 0.0f;
  }
  if (pct > 1)
  {
    pct = 1.0f;
  }
  pct = 270 * pct;

  // Redraw the arc
  cyberDrawArc(arcStart, 270.0f, x, y, 13);

  // Draw the percentage dot
  // float cx = x + (radius)*cosf((pct - arcStart) * d2r);
  // float cy = y + (radius)*sinf((pct - arcStart) * d2r);
  std::pair<float, float> xy = calcArcPoint((pct - arcStart), x, y, radius);
  dis->fillCircle(xy.first, xy.second, 4, SCWhite);
  dis->fillCircle(xy.first, xy.second, 1, SCBlack);
}
