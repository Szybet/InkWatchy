#include "menu.h"

menuData data = {0};

void initMenu(entryMenu entryList[MAX_MENU_ITEMS], int totalMenus, String menuName, int textSize, int linesThick)
{
  memcpy(data.entryList, entryList, sizeof(entryList[0]) * totalMenus);

  data.totalMenus = totalMenus;
  data.textSize = textSize;
  data.menuName = menuName;
  data.linesThick = linesThick;
  data.currentPage = 0;


}


void showMenu()
{


}