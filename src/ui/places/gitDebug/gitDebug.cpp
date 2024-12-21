#include "gitDebug.h"

#if DEBUG_MENUS

void initGitDebug()
{
    uint16_t h;
    setFont(&FreeSansBold9pt7b);
    setTextSize(1);
    dis->setCursor(0, 1);
    String menuName = "Debug Menu: Git";
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

    centerText("Commit hash:", &currentHeight);
    centerText(String(GIT_COMMIT_HASH), &currentHeight);

    centerText("Branch:", &currentHeight);
    centerText(String(GIT_BRANCH), &currentHeight);

    centerText("Build time:", &currentHeight);
    centerText(String(BUILD_TIME), &currentHeight);

    disUp(true);
}

#endif