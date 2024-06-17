#include "bookMod.h"

#if BOOK_MODULE_ENABLED
#define BOOK_NAME_MODULE_NAME_MAX 25

void wfBookcheckShow(bool *showBool, bool *redrawBool)
{
    *showBool = true;
}

void wfBookrequestShow(buttonState button, bool *showBool)
{
    debugLog("Launched");
    bool showPageChecker = false;
    switch (button)
    {
    case Menu:
    {
        if (watchfacePos == MODULE_ENG_POS && positionEngaged == true)
        {
            if (disableSomeDrawing == false)
            {
                disableSomeDrawing = true;
                calculateBookTextHeight();
                startHeightBook = startHeightBook - 3;
                showPageChecker = true;
            }
            else
            {
                disableSomeDrawing = false;
                showFullWatchface();
            }
        }
        break;
    }
    case Down:
    {
        debugLog("Book mod down");
        changePageDown(BOOK_MODULE_CHARS_PER_PAGE, false);
        showPageChecker = true;
        break;
    }
    case Up:
    {
        debugLog("Book mod up");
        changePageUp(BOOK_MODULE_CHARS_PER_PAGE, false);
        showPageChecker = true;
        break;
    }
    }

    String curBook = getCurrentBook();
    if (disableSomeDrawing == false)
    {
        if (curBook == "")
        {
            debugLog("Entered no book selected");
            drawTextSimple("No book selected", "UbuntuMono-Regular10", MODULE_RECT_X, MODULE_RECT_Y);
        }
        else
        {
            debugLog("Entered book selected");
            sizeInfo s = drawTextSimple("Selected book:", "dogicapixel4", MODULE_RECT_X, MODULE_RECT_Y);
            // Max BOOK_NAME_MODULE_NAME_MAX
            String curBookTmp = curBook;
            if(curBookTmp.length() > BOOK_NAME_MODULE_NAME_MAX) {
                curBookTmp = curBookTmp.substring(0, BOOK_NAME_MODULE_NAME_MAX);
            }
            sizeInfo ss = drawTextSimple(curBookTmp, "dogicapixel4", MODULE_RECT_X, MODULE_RECT_Y + s.h + 2);
            drawTextSimple(bookGetPages(BOOK_MODULE_CHARS_PER_PAGE), "dogicapixel4", MODULE_RECT_X, MODULE_RECT_Y + s.h + ss.h + 4);
        }
    }
    else
    {
        if (showPageChecker == true)
        {
            debugLog("Show the book");
            String text = showPage(getPageNumber(), false, BOOK_MODULE_CHARS_PER_PAGE); // this sets duChange
            debugLog("book mod text: \n" + text + "\n");
            cleanSomeDrawing();
            setFont(BOOK_FONT);
            setTextSize(1);
            display.setCursor(1, startHeightBook + SOME_RECT_Y);
            display.setTextWrap(true);
            display.print(text);
        }
        else
        {
            debugLog("Don't show the book");
        }
    }

    disUp();
}

RTC_DATA_ATTR wfModule wfBook = {
    true,
    wfBookcheckShow,
    wfBookrequestShow,
};

#endif