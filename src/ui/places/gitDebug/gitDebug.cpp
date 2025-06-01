#include "gitDebug.h"
#include "localization.h"

#if DEBUG_MENUS

void initGitDebug()
{
    uint16_t h;
    setFont(&FreeSansBold9pt7b);
    setTextSize(1);
    dis->setCursor(0, 1);
    String menuName = DEBUG_MENU_GIT;
    debugLog(menuName);
    getTextBounds(menuName, NULL, NULL, NULL, &h);
    if (containsBelowChar(menuName) == true)
    {
        h = h + 2;
    }
    maxHeight = h;
    uint16_t currentHeight = maxHeight;
    dis->setCursor(0, currentHeight - 3);
    dis->print(menuName);

    dis->fillRect(0, currentHeight, dis->width(), 3, GxEPD_BLACK);
    currentHeight = currentHeight + maxHeight;

    centerText(DEBUG_GIT_COMMIT_HASH, &currentHeight);
    centerText(String(GIT_COMMIT_HASH), &currentHeight);

    centerText(DEBUG_GIT_BRANCH, &currentHeight);
    centerText(String(GIT_BRANCH), &currentHeight);

    centerText(DEBUG_GIT_BUILD_TIME, &currentHeight);
    centerText(String(BUILD_TIME), &currentHeight);

    disUp(true);
}

#endif