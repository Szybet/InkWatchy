#include "bookMod.h"

#if BOOK_MODULE_ENABLED

void wfBookcheckShow(bool *showBool, bool *redrawBool)
{
    *showBool = true;
}

void wfBookrequestShow(buttonState button, bool *showBool)
{
    debugLog("Launched");
    String curBook = getCurrentBook();
    if (curBook == "")
    {
        debugLog("Entered no book selected");
        String no = "No book selected";
        uint16_t h = 0;
        setFont(getFont("smileandwave20"));
        setTextSize(1);
        getTextBounds(no, NULL, NULL, NULL, &h);
        display.setCursor(MODULE_RECT_X, MODULE_RECT_Y + h);
        display.print(no);
        dUChange = true;
    }

    if (disableSomeDrawing == false)
    {
    }
    disUp();
}

RTC_DATA_ATTR wfModule wfBook = {
    true,
    wfBookcheckShow,
    wfBookrequestShow,
};

#endif