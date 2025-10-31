#include "credits.h"

#if CREDITS

#define CREDITS_INKWATCHY_QR "InkWatchy QR"
#define CREDITS_YATCHY_QR "Yatchy QR"
#define CREDITS_ICON "Logo"

bool isQrOn = false;
int qrToShow = 0;

#define NO_PAGE -1
#define ICON_PAGE 0
#define QR_INKWATCHY_PAGE 1
#define QR_YATCHY_PAGE 2

void turnInkWatchyQr()
{
    isQrOn = true;
    qrToShow = QR_INKWATCHY_PAGE;
}

void turnYatchyQr()
{
    isQrOn = true;
    qrToShow = QR_YATCHY_PAGE;
}

void turnIcon()
{
    isQrOn = true;
    qrToShow = ICON_PAGE;
}

void initCredits()
{
    isQrOn = false;

    init_general_page(10);
    general_page_set_title(MENU_CREDITS);

    genpage_add("Created by:");
    genpage_add("Szybet");
    genpage_add("github.com/Szybet");
    genpage_add("Helped:");
    genpage_add(get_credits_nicks());

    GeneralPageButton buttons[] = {
        GeneralPageButton{CREDITS_INKWATCHY_QR, turnInkWatchyQr},
        GeneralPageButton{CREDITS_YATCHY_QR, turnYatchyQr},
        GeneralPageButton{CREDITS_ICON, turnIcon}};
    general_page_set_buttons(buttons, 3);

    general_page_set_main();
}

void loopCredits()
{
    if (isQrOn == false)
    {
        slint_loop();
    }
    else
    {
        if (useButton() != None)
        {
            isQrOn = false;
            genpage_force_update();
            return;
        }
        if (qrToShow != NO_PAGE)
        {
            dis->fillScreen(SCWhite);
        }
        if (qrToShow == QR_INKWATCHY_PAGE)
        {
            writeImageN(10, 10, getImg("credits/inkwatchy_qr"));
        }
        else if (qrToShow == QR_YATCHY_PAGE)
        {
            writeImageN(10, 10, getImg("credits/yatchy_qr"));
        }
        else if (qrToShow == ICON_PAGE)
        {
            writeImageN(0, 0, getImg("credits/inkwatchy_icon"));
        }
        if (qrToShow != NO_PAGE)
        {
            qrToShow = NO_PAGE;
            dUChange = true;
        }
        disUp();
    }
    resetSleepDelay();
}

#endif
