#include "bookUi.h"

int getPageNumber()
{
    return NVS.getInt(NVS_CURRENT_PAGE, 1);
}

void setPageNumber(int page)
{
    NVS.setInt(NVS_CURRENT_PAGE, page, true); // I think we can write many times without problem?
}

void showPage(int page)
{
    setFont(&FreeSansBold9pt7b);
    setTextSize(1);
    display.fillScreen(GxEPD_WHITE);
    display.print(bookList[page]);
}

void initBook()
{
    initNvsManage();
    display.setTextWrap(true);
    showPage(getPageNumber());
    disUp(true);
}

void loopBook()
{
    switch (useButton())
    {
    case Up:
    {
        int page = getPageNumber();
        page = page + 1;
        if (page > BOOK_PAGES)
        {
            break;
        }
        setPageNumber(page);
        showPage(page);
        break;
    }
    case Down:
    {
        int page = getPageNumber();
        page = page + 1;
        if (page == 0)
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
    return String(getPageNumber()) + "/" + String(BOOK_PAGES);
}
