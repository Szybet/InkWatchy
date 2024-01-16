#include "../../defines/config.h"
#if BOOK
#include "bookUi.h"
#include "../../defines/book.h"

#define BOOK_FONT &monofonto_rg10pt7b

void resetSleepDelayBook()
{
    sleepDelayMs = millis() + BOOK_ADD_SLEEP_DELAY_MS;
    debugLog("sleepDelayMs is:" + String(sleepDelayMs));
    debugLog("millis is:" + String(millis()));
}

int getPageNumber()
{
    return NVS.getInt(NVS_BOOK_CURRENT_PAGE, 0);
}

void setPageNumber(int page)
{
    NVS.setInt(NVS_BOOK_CURRENT_PAGE, page, false); // We commit in exit
}

String test = "jqyQRTY";
uint16_t startHeight;
int16_t staAx_X;
int16_t staAx_Y;
int16_t staAx_Z;
bool waitForReturn = false;

void resetStartAxc()
{
    bma4_accel accel;
    SBMA.getAccel(accel);
    staAx_X = accel.x;
    staAx_Z = accel.z;
    staAx_Y = accel.y;
}

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
    initAxc();
    // SBMA.setAccelConfig();
    SBMA.enableAccel();
    resetStartAxc();

    setFont(BOOK_FONT);
    setTextSize(1);
    getTextBounds(test, NULL, NULL, NULL, &startHeight);
    //startHeight = startHeight - 3;
    startHeight = startHeight + 2;
    display.setTextWrap(true);

    int currPage = getPageNumber();
    if (currPage > BOOK_PAGES - 1 || currPage <= -1)
    {
        setPageNumber(0);
        currPage = 0;
    }

    showPage(currPage);
    disUp(true);
}

void exitBook()
{
    if(BOOK_ON_EXIT_GO_PAGE_BACK == true) {
        debugLog("Going page back because of BOOK_ON_EXIT_GO_PAGE_BACK");
        changePageDown();
    }
    bool comm = NVS.commit();
    debugLog("Commit status: " + BOOL_STR(comm));
    deInitAxc();
}

void loopBook()
{
    switch (useButton())
    {
    case Up:
    {
        resetStartAxc();
        changePageUp();
        break;
    }
    case Down:
    {
        resetStartAxc();
        changePageDown();
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

    bma4_accel accel;
    if (SBMA.getAccel(accel))
    {
        // debugLog("x: " + String(accel.x));
        // debugLog("y: " + String(accel.y));
        // debugLog("z: " + String(accel.z));
        if (staAx_Y + BOOK_AX_Y_BACK_VALUE < accel.y || staAx_Y - BOOK_AX_Y_BACK_VALUE > accel.y)
        {
            if (staAx_Z + BOOK_AX_Z_BACK_VALUE < accel.z || staAx_Z - BOOK_AX_Z_BACK_VALUE > accel.z)
            {
                // Imit a long back button press
                setButton(LongBack);
                return void();
            }
        }

        if (waitForReturn == false)
        {
            if (staAx_X + BOOK_AX_X_PAGE_CHANGE_VALUE_UP < accel.x)
            {
                waitForReturn = true;
                changePageUp();
                debugLog("Changed page up by axis move");
            }
            else if (staAx_X - BOOK_AX_X_PAGE_CHANGE_VALUE_DOWN > accel.x)
            {
                waitForReturn = true;
                changePageDown();
                debugLog("Changed page down by axis move");
            }
        }
        else
        {
            if (staAx_X + BOOK_AX_X_TOLERANCE > accel.x && staAx_X - BOOK_AX_X_TOLERANCE < accel.x)
            {
                waitForReturn = false;
                debugLog("Reset waitForReturn");
            }
        }
    }
    else
    {
        debugLog("Failed to get accel");
    }
}

String bookGetPages()
{
    initNvsManage();
    return String(getPageNumber()) + "/" + String(BOOK_PAGES - 1);
}

void changePageUp()
{
    resetSleepDelayBook();
    int page = getPageNumber();
    page = page + 1;
    if (page > BOOK_PAGES - 1)
    {
        return void();
    }
    setPageNumber(page);
    showPage(page);
}

void changePageDown()
{
    resetSleepDelayBook();
    int page = getPageNumber();
    page = page - 1;
    if (page <= -1)
    {
        return void();
    }
    setPageNumber(page);
    showPage(page);
}

#endif
