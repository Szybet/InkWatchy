#include "menu.h"

menuData currentMenuData = {0, 0, "", 0, nullptr, 0, 0};
#define buttonsOffset 2;
int currentMenuItem = 0;

bool showedMenuName = false;
String previousPageNumber = "";
struct buttonCheck
{
  bool inverted;
  String text;
};
#define MAX_BUTTONS 8
buttonCheck previousButtons[MAX_BUTTONS];
sizeInfo buttonSize;

void resetPreviousItems()
{
  // Clear variables
  showedMenuName = false;
  previousPageNumber = "";
  for (int i = 0; i < MAX_BUTTONS; ++i)
  {
    previousButtons[i].inverted = false;
    previousButtons[i].text = "";
  }
  buttonSize.h = 0;
  buttonSize.w = 0;
}

void initMenu(entryMenu* entryList, int totalMenus, String menuName, int textSize, int linesThick)
{
  if (currentMenuData.entryList != nullptr) {
    delete[] currentMenuData.entryList;
  }
  currentMenuData.entryList = new entryMenu[totalMenus];
  for (int i = 0; i < totalMenus; i++) {
    currentMenuData.entryList[i] = entryList[i];
  }

  currentMenuData.totalMenus = totalMenus;
  currentMenuData.textSize = textSize;
  currentMenuData.menuName = menuName;
  currentMenuData.linesThick = linesThick;
  currentMenuData.currentButton = currentMenuItem;

  if (textSize == 1)
  {
    currentMenuData.itemsOnPage = 7;
  }
  else
  {
    currentMenuData.itemsOnPage = 5;
  }

  resetPreviousItems();

  dis->fillScreen(SCWhite);
  showMenu();
}

int pageNumber = 0;
int currentPage = 0;

void showMenu()
{
  uint16_t pageStringWidth;
  uint16_t textHeight;
  uint16_t currentHeight;
  int startingButton = 0;
  setFont(&FreeSansBold9pt7b);
  setTextSize(1);
  dis->setCursor(1, 1);
  getTextBounds(currentMenuData.menuName, NULL, NULL, NULL, &textHeight);

  currentPage = currentMenuData.currentButton / currentMenuData.itemsOnPage;
  pageNumber = ceil(float(currentMenuData.totalMenus) / float(currentMenuData.itemsOnPage));

  String pageString = String(currentPage + 1) + "/" + String(pageNumber);
  debugLog(pageString);
  getTextBounds(pageString, NULL, NULL, &pageStringWidth, NULL);
  // debugLog("Page string width: " + String(pageStringWidth));
  currentHeight = textHeight + 1; // +1 to offset between edge of screen and menu name
  dis->setCursor(dis->width() - pageStringWidth - 10, currentHeight);

  if (previousPageNumber != pageString)
  {
    // uint16_t hTmp;
    // uint16_t wTmp;
    // getTextBounds(previousPageNumber, NULL, NULL, &wTmp, &hTmp);
    // dis->fillRect(dis->width() - pageStringWidth - 10, currentHeight - hTmp, wTmp, hTmp, SCWhite);

    dis->fillScreen(SCWhite);
    resetPreviousItems();

    dis->print(pageString);
    previousPageNumber = pageString;
    debugLog("Printing page numbers");
  }

  dis->setCursor(1, currentHeight);
  if (showedMenuName == false)
  {
    dis->print(currentMenuData.menuName);
    showedMenuName = true;
    debugLog("Printing menu name");
  }

  currentHeight = currentHeight + 4; // +2 to offset between line and menu name

  dis->fillRect(0, currentHeight, dis->width(), 1, SCBlack);
  currentHeight = currentHeight + 4; // +2 to offset line and button

  while (startingButton + currentMenuData.itemsOnPage <= currentMenuData.currentButton)
  {
    startingButton = startingButton + currentMenuData.itemsOnPage;
  }
  // debugLog("current button: " + String(currentMenuData.currentButton));
  // debugLog("starting button: " + String(startingButton));
  setTextSize(currentMenuData.textSize);
  for (int i = startingButton; i < startingButton + currentMenuData.itemsOnPage && i < currentMenuData.totalMenus; i++)
  {
    // debugLog("iterating " + String(i));
    // debugLog("iterating list" + String(i % currentMenuData.itemsOnPage));

    bool invert = false;
    if (currentMenuData.currentButton == i)
    {
      invert = true;
    }
    bool draw = false;

    if (currentMenuData.entryList[i].text != previousButtons[i % currentMenuData.itemsOnPage].text || previousButtons[i % currentMenuData.itemsOnPage].inverted != invert)
    {
      draw = true;
      previousButtons[i % currentMenuData.itemsOnPage].text = currentMenuData.entryList[i].text;
      previousButtons[i % currentMenuData.itemsOnPage].inverted = invert;
      // debugLog("Printing button: " + String(i % currentMenuData.itemsOnPage));
    }

    // debugLog("Menu entry text is: " + currentMenuData.entryList[i].text);
    String textToShow = currentMenuData.entryList[i].text;
    // To show the cut off text fully when it's selected :D the value of 18 should be calculated somehow
    ImageDef *img = currentMenuData.entryList[i].image;

    if (textToShow.length() > 18 && currentMenuData.currentButton != i)
    {
      textToShow = textToShow.substring(0, 18);
    }
    else if (textToShow.length() > 18 && currentMenuData.currentButton == i)
    {
      previousPageNumber = ""; // To reset the next iteration
      img = &emptyImgPack;
    }
    buttonSize = drawButton(1, currentHeight, textToShow, img, invert, 2, 0, SCBlack, SCWhite, draw);
    // debugLog("Button h in menu: " + String(buttonSize.h));
    if (draw == true)
    {
      if (invert == true)
      {
        dis->fillRect(1 + buttonSize.w, currentHeight, dis->width() - buttonSize.w - 1, buttonSize.h, SCBlack);
      }
      else
      {
        dis->fillRect(1 + buttonSize.w, currentHeight, dis->width() - buttonSize.w - 1, buttonSize.h, SCWhite);
      }
      if (i != startingButton && MENU_LINES == true)
      {
        dis->fillRect(0, currentHeight - 2, dis->width(), 2, SCBlack);
      }
    }
    currentHeight = currentHeight + buttonSize.h + buttonsOffset;
  }
  dUChange = true;
}

String lastMenuSelected;
void loopMenu()
{
  switch (useButton())
  {
  case Up:
  {
    currentMenuData.currentButton -= 1;
    checkMaxMin(&currentMenuData.currentButton, currentMenuData.totalMenus - 1);
    currentMenuItem = currentMenuData.currentButton;
    // debugLog("Updating currentMenuItem: " + String(currentMenuItem));
    showMenu();
    break;
  }
  case Down:
  {
    currentMenuData.currentButton += 1;
    checkMaxMin(&currentMenuData.currentButton, currentMenuData.totalMenus - 1);
    currentMenuItem = currentMenuData.currentButton;
    // debugLog("Updating currentMenuItem: " + String(currentMenuItem));
    showMenu();
    break;
  }
  case Menu:
  {
    if (currentMenuData.entryList[currentMenuData.currentButton].function != nullptr)
    {
      lastMenuSelected = currentMenuData.entryList[currentMenuData.currentButton].text;
      currentMenuData.entryList[currentMenuData.currentButton].function();
    }
#if DEBUG
    else
    {
      debugLog("Menu entry item has a invalid function: " + currentMenuData.entryList[currentMenuData.currentButton].text);
    }
#endif
    break;
  }
  case LongUp:
  {
    currentPage -= 1;
    checkMaxMin(&currentPage, pageNumber - 1);
    currentMenuData.currentButton = currentPage * currentMenuData.itemsOnPage;
    currentMenuItem = currentMenuData.currentButton;
    debugLog("currentMenuData.currentButton: " + String(currentMenuData.currentButton));
    showMenu();
    break;
  }
  case LongDown:
  {
    currentPage += 1;
    checkMaxMin(&currentPage, pageNumber - 1);
    currentMenuData.currentButton = currentPage * currentMenuData.itemsOnPage;
    currentMenuItem = currentMenuData.currentButton;
    debugLog("currentMenuData.currentButton: " + String(currentMenuData.currentButton));
    showMenu();
    break;
  }
  default:
  {
    break;
  }
  }
  disUp();
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
