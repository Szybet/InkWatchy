#include "../../defines/config.h"
#if BOOK
#include "bookUi.h"

#define BOOK_FONT &monofonto_rg10pt7b

void resetSleepDelayBook() {
    sleepDelayMs = millis() + ADD_SLEEP_DELAY_BOOK_MS;
    debugLog("sleepDelayMs is:" + String(sleepDelayMs));
    debugLog("millis is:" + String(millis()));
}

int getPageNumber()
{
    return NVS.getInt(NVS_CURRENT_PAGE, 0);
}

void setPageNumber(int page)
{
    NVS.setInt(NVS_CURRENT_PAGE, page, true); // I think we can write many times without problem?
}

String test = "jqyQRTY";
uint16_t startHeight;
void showPage(int page)
{
    resetSleepDelayBook();
    setFont(BOOK_FONT);
    setTextSize(1);

    display.setCursor(1, startHeight);
    display.fillScreen(GxEPD_WHITE);
    display.print(bookList[page]);
    disUp(true);
}

void initBook()
{
    initNvsManage();
    setFont(BOOK_FONT);
    setTextSize(1);
    getTextBounds(test, NULL, NULL, NULL, &startHeight);
    startHeight = startHeight - 3;
    display.setTextWrap(true);

    int currPage = getPageNumber();
    if(currPage > BOOK_PAGES - 1 || currPage <= -1) {
        setPageNumber(0);
        currPage = 0;
    }

    showPage(currPage);
    disUp(true);
}

void loopBook()
{
    switch (useButton())
    {
    case Up:
    {
        resetSleepDelayBook();
        int page = getPageNumber();
        page = page + 1;
        if (page > BOOK_PAGES - 1)
        {
            break;
        }
        setPageNumber(page);
        showPage(page);
        break;
    }
    case Down:
    {
        resetSleepDelayBook();
        int page = getPageNumber();
        page = page - 1;
        if (page <= -1)
        {
            break;
        }
        setPageNumber(page);
        showPage(page);
        break;
    }
    case Menu:
    {
        break;
    }
    case LongUp:
    {
        break;
    }
    case LongDown:
    {
        break;
    }
    }
}

String bookGetPages()
{
    initNvsManage();
    return String(getPageNumber()) + "/" + String(BOOK_PAGES - 1);
}
#endif
