#include "bookMod.h"
#include "rtcMem.h"

#if BOOK_MODULE_ENABLED
#define BOOK_NAME_MODULE_NAME_MAX 25

void wfBookcheckShow(bool *showBool, bool *redrawBool)
{
    *showBool = true;
}

void calcBookTextMod()
{
    calculateBookTextHeight();
    startHeightBook = startHeightBook - 3;
}

void wfBookrequestShow(buttonState button, bool *showBool)
{
    debugLog("Launched");
    bool showPageChecker = false;
    switch (button)
    {
    case Menu:
    {
        if (isModuleEngaged() == true)
        {
            if (rM.disableSomeDrawing == false)
            {
                rM.disableSomeDrawing = true;
                calcBookTextMod();
                showPageChecker = true;
            }
            else
            {
                rM.disableSomeDrawing = false;
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
        calcBookTextMod();
        break;
    }
    case Up:
    {
        debugLog("Book mod up");
        changePageUp(BOOK_MODULE_CHARS_PER_PAGE, false);
        showPageChecker = true;
        calcBookTextMod();
        break;
    }
    default:
    {
        break;
    }
    }

    String curBook = getCurrentBook();
    squareInfo modSq = getWatchModuleSquare();
    squareInfo someSq = getSomeDrawingSquare();
    if (rM.disableSomeDrawing == false)
    {
        if (curBook == "")
        {
            debugLog("Entered no book selected");
            drawTextSimple("No book selected", "UbuntuMono-Regular10", modSq.cord.x, modSq.cord.y);
        }
        else
        {
            debugLog("Entered book selected");
            sizeInfo s = drawTextSimple("Selected book:", "dogicapixel4", modSq.cord.x, modSq.cord.y);
            // Max BOOK_NAME_MODULE_NAME_MAX
            String curBookTmp = curBook;
            if (curBookTmp.length() > BOOK_NAME_MODULE_NAME_MAX)
            {
                curBookTmp = curBookTmp.substring(0, BOOK_NAME_MODULE_NAME_MAX);
            }
            sizeInfo ss = drawTextSimple(curBookTmp, "dogicapixel4", modSq.cord.x, modSq.cord.y + s.h + 2);
            drawTextSimple(bookGetPages(BOOK_MODULE_CHARS_PER_PAGE), "dogicapixel4", modSq.cord.x, modSq.cord.y + s.h + ss.h + 4);
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
            dis->setCursor(1, startHeightBook + someSq.cord.y);
            dis->setTextWrap(true);
            dis->print(text);
        }
        else
        {
            debugLog("Don't show the book");
        }
    }

    dUChange = true;
}

#endif