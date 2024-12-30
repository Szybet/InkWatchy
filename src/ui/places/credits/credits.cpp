#include "credits.h"

#if CREDITS

#define ICON_PAGE 0
#define QR_INKWATCHY_PAGE 1
#define QR_YATCHY_PAGE 2
#define CREDITS_PAGE 3

#define CREDITS_PAGE_MAX 3

int credPage = 0;

void changePageCredits()
{
    dis->fillScreen(GxEPD_WHITE);
    switch (credPage)
    {
    case ICON_PAGE:
    {
        writeImageN(0, 0, getImg("credits/inkwatchy_icon"));
        break;
    }
    case QR_INKWATCHY_PAGE:
    {
        uint16_t h;
        setFont(&FreeSansBold9pt7b);
        setTextSize(1);
        dis->setCursor(0, 0);
        String menuName = "InkWatchy";
        debugLog(menuName);
        getTextBounds(menuName, NULL, NULL, NULL, &h);
        uint16_t hTmp = h;
        centerText(menuName, &hTmp);
        writeImageN(10, 25, getImg("credits/inkwatchy_qr"));
        break;
    }
    case QR_YATCHY_PAGE:
    {
        uint16_t h;
        setFont(&FreeSansBold9pt7b);
        setTextSize(1);
        dis->setCursor(0, 0);
        String menuName = "Yatchy";
        debugLog(menuName);
        getTextBounds(menuName, NULL, NULL, NULL, &h);
        uint16_t hTmp = h;
        centerText(menuName, &hTmp);
        writeImageN(10, 25, getImg("credits/yatchy_qr"));
        break;
    }
    case CREDITS_PAGE:
    {
        uint16_t h;
        setFont(&FreeSansBold9pt7b);
        setTextSize(1);
        dis->setCursor(0, 0);
        String str1 = "Created by:";
        debugLog(str1);
        getTextBounds(str1, NULL, NULL, NULL, &h);
        h = h + 1;
        maxHeight = h;
        centerText(str1, &h);
        centerText("Szybet", &h);
        centerText("github.com/Szybet", &h);
        centerText("Helped:", &h);
        centerText("Cat in a hoodie", &h);
        centerText("GuruSR", &h);
        centerText("Prokuon", &h);
        centerText("Jason2866", &h);
        centerText("C4nn0n", &h);
        centerText("invpt", &h);
        break;
    }
    };
    dUChange = true;
}

void initCredits()
{
    credPage = 0;
    changePageCredits();
}

void loopCredits()
{
    buttonState btn = useButton();
    bool did = false;
    if (btn == Up)
    {
        credPage = credPage + 1;
        did = true;
    }
    else if (btn == Down)
    {
        credPage = credPage - 1;
        did = true;
    }
    if (did == true)
    {
        checkMaxMin(&credPage, CREDITS_PAGE_MAX);
        changePageCredits();
    }
    resetSleepDelay();
    disUp();
}

#endif