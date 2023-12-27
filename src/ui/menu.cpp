#include "menu.h"

menuData data = {0};
#define buttonsOffset 2;
int currentMenuItem = 0;

void initMenu(entryMenu *entryList, int totalMenus, String menuName, int textSize, int linesThick)
{
  // A lot of funny problems right here!
  // memcpy(data.entryList, entryList, sizeof(entryList[0]) * totalMenus);
  for (int i = 0; i < totalMenus; i++)
  {
    data.entryList[i].text = entryList[i].text;
    data.entryList[i].image = entryList[i].image;
    data.entryList[i].function = entryList[i].function;
  }

  data.totalMenus = totalMenus;
  data.textSize = textSize;
  data.menuName = menuName;
  data.linesThick = linesThick;
  data.currentButton = currentMenuItem;

  if (textSize == 1)
  {
    data.itemsOnPage = 7;
  }
  else
  {
    data.itemsOnPage = 5;
  }
  showMenu();
}

int pageNumber = 0;
int currentPage = 0;
void showMenu()
{
  display.fillScreen(GxEPD_WHITE);

  uint16_t pageStringWidth;
  uint16_t textHeight;
  uint16_t currentHeight;
  int startingButton = 0;
  setFont(&FreeSansBold9pt7b);
  setTextSize(1);
  display.setCursor(1, 1);
  getTextBounds(data.menuName, NULL, NULL, NULL, &textHeight);

  currentPage = data.currentButton / data.itemsOnPage;
  pageNumber = ceil(float(data.totalMenus) / float(data.itemsOnPage));

  String pageString = String(currentPage + 1) + "/" + String(pageNumber);
  debugLog(pageString);
  getTextBounds(pageString, NULL, NULL, &pageStringWidth, NULL);
  debugLog("Page string width: " + String(pageStringWidth));
  currentHeight = textHeight + 1; // +1 to offset between edge of screen and menu name
  display.setCursor(display.width() - pageStringWidth - 10, currentHeight);

  display.print(pageString);

  display.setCursor(1, currentHeight);

  display.print(data.menuName);
  currentHeight = currentHeight + 4; // +2 to offset between line and menu name

  display.fillRect(0, currentHeight, display.width(), 1, GxEPD_BLACK);
  currentHeight = currentHeight + 4; // +2 to offset line and button

  while (startingButton + data.itemsOnPage <= data.currentButton)
  {
    startingButton = startingButton + data.itemsOnPage;
  }
  debugLog("current button: " + String(data.currentButton));
  debugLog("starting button: " + String(startingButton));
  setTextSize(data.textSize);
  for (int i = startingButton; i < startingButton + data.itemsOnPage && i < data.totalMenus; i++)
  {
    // debugLog("iterating " + String(i));
    bool invert = false;
    if (data.currentButton == i)
    {
      invert = true;
    }
    debugLog("Menu entry text is: " + data.entryList[i].text);
    sizeInfo buttonSize = drawButton(1, currentHeight, data.entryList[i].text, *data.entryList[i].image, invert, 2, 0);
    debugLog("Button h in menu: " + String(buttonSize.h));
    if (invert == true)
    {
      display.fillRect(1 + buttonSize.w, currentHeight, display.width() - buttonSize.w - 1, buttonSize.h, GxEPD_BLACK);
    }
    if (i != startingButton && MENU_LINES == true)
    {
      display.fillRect(0, currentHeight - 2, display.width(), 2, GxEPD_BLACK);
    }

    // currentHeight = currentHeight + data.maxHeight + buttonsOffset;
    currentHeight = currentHeight + buttonSize.h + buttonsOffset;
  }
  disUp(true);
}

String lastMenuSelected;
void loopMenu()
{
  switch (useButton())
  {
  case Up:
  {
    data.currentButton -= 1;
    checkMaxMin(&data.currentButton, data.totalMenus - 1);
    currentMenuItem = data.currentButton;
    showMenu();
    break;
  }
  case Down:
  {
    data.currentButton += 1;
    checkMaxMin(&data.currentButton, data.totalMenus - 1);
    currentMenuItem = data.currentButton;
    showMenu();
    break;
  }
  case Menu:
  {
    if (data.entryList[data.currentButton].function != nullptr)
    {
      lastMenuSelected = data.entryList[data.currentButton].text;
      data.entryList[data.currentButton].function();
    }
#if DEBUG
    else
    {
      debugLog("Menu entry item has a invalid function: " + data.entryList[data.currentButton].text);
    }
#endif
    break;
  }
  case LongUp:
  {
    currentPage -= 1;
    checkMaxMin(&currentPage, pageNumber - 1);
    data.currentButton = currentPage * data.itemsOnPage;
    currentMenuItem = data.currentButton;
    debugLog("data.currentButton: " + String(data.currentButton));
    showMenu();
    break;
    break;
  }
  case LongDown:
  {
    currentPage += 1;
    checkMaxMin(&currentPage, pageNumber - 1);
    data.currentButton = currentPage * data.itemsOnPage;
    currentMenuItem = data.currentButton;
    debugLog("data.currentButton: " + String(data.currentButton));
    showMenu();
    break;
  }
  }
}

/*
// Menu testing code
entryMenu button0 = {"text0", crossImgPack, NULL};
entryMenu button1 = {"text1", acceptImgPack, NULL};
entryMenu button2 = {"text2", emptyImgPack, NULL};
entryMenu button3 = {"text3", emptyImgPack, NULL};
entryMenu button4 = {"text4", emptyImgPack, NULL};
entryMenu button5 = {"text5", emptyImgPack, NULL};
entryMenu button6 = {"text6", emptyImgPack, NULL};
entryMenu button7 = {"text7", emptyImgPack, NULL};
entryMenu button8 = {"text8", emptyImgPack, NULL};
entryMenu button9 = {"text9", emptyImgPack, NULL};
entryMenu button10 = {"text10", emptyImgPack, NULL};
entryMenu button11 = {"text11", emptyImgPack, NULL};
entryMenu button12 = {"text12", emptyImgPack, NULL};
entryMenu button13 = {"text13", emptyImgPack, NULL};
entryMenu button14 = {"text14", emptyImgPack, NULL};

entryMenu buttons[15] = {button0, button1, button2, button3, button4, button5, button6, button7, button8, button9, button10, button11, button12, button13, button14};
initMenu(buttons, 15, "MDKNCMIUEjqi", 2);
*/
