#include "menu.h"

menuData data = {0};
#define buttonsOffset 4;

void initMenu(entryMenu entryList[MAX_MENU_ITEMS], int totalMenus, String menuName, int textSize, int linesThick)
{
  memcpy(data.entryList, entryList, sizeof(entryList[0]) * totalMenus);

  data.totalMenus = totalMenus;
  data.textSize = textSize;
  data.menuName = menuName;
  data.linesThick = linesThick;
  data.currentPage = 0;
  data.currentButton = 0;

  if (textSize == 1)
  {
    data.itemsOnPage = 7;
    data.maxHeight = 19;
  }
  else
  {
    data.itemsOnPage = 5;
    data.maxHeight = 32;
  }
  showMenu();
}

void showMenu()
{
  display.fillScreen(GxEPD_WHITE);
  uint16_t textHeight;
  uint16_t currentHeight;
  int startingButton = 0;
  setFont(&FreeSansBold9pt7b);
  setTextSize(1);
  display.setCursor(1, 1);
  getTextBounds(data.menuName, NULL, NULL, NULL, &textHeight);
  currentHeight = textHeight + 1; // +1 to offset between edge of screen and menu name
  display.setCursor(1, currentHeight);

  display.print(data.menuName);
  currentHeight = currentHeight + 4; // +2 to offset between line and menu name

  display.fillRect(0, currentHeight, display.width(), 1, GxEPD_BLACK);
  currentHeight = currentHeight + 4; // +2 to offset line and button

  while (startingButton + data.itemsOnPage <= data.currentButton)
  {
    startingButton = startingButton + data.itemsOnPage;
  }
  log("current button: " + String(data.currentButton));
  log("starting button: " + String(startingButton));
  setTextSize(data.textSize);
  for (int i = startingButton; i < startingButton + data.itemsOnPage && i < data.totalMenus; i++)
  {
    log("iterating " + String(i));
    bool invert = false;
    if (data.currentButton == i)
    {
      invert = true;
    }
    drawButton(1, currentHeight, data.entryList[i].text, data.entryList[i].image, invert, 2, 0);
    if (i != startingButton && MENU_LINES == true)
    {
      display.fillRect(0, currentHeight - 2, display.width(), 2, GxEPD_BLACK);
    }

    currentHeight = currentHeight + data.maxHeight + buttonsOffset;
  }
  display.display(PARTIAL_UPDATE);
}

void menuLoop()
{
  switch (useButton())
  {
  case Up:
  {
    data.currentButton -= 1;
    checkMaxMin(&data.currentButton, data.totalMenus - 1);
    showMenu();
    break;
  }
  case Down:
  {
    data.currentButton += 1;
    checkMaxMin(&data.currentButton, data.totalMenus - 1);
    showMenu();
    break;
  }
  case Menu:
  {
    break;
  }
  }
}