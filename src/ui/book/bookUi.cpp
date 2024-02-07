#include "../../defines/config.h"
#if BOOK
#include "bookUi.h"
#include "../../defines/book.h"

#define BOOK_FONT &monofonto_rg10pt7b
bool isBookOk = false;

void resetSleepDelayBook()
{
    sleepDelayMs = millis() + BOOK_ADD_SLEEP_DELAY_MS;
    debugLog("sleepDelayMs is:" + String(sleepDelayMs));
    debugLog("millis is:" + String(millis()));
}

void setPageNumber(int page)
{
    NVS.setInt(NVS_BOOK_CURRENT_PAGE, page, false); // We commit in exit
}

int getPageNumber()
{
    if (isBookOk == false)
    {
        if (NVS.getString(NVS_BOOK_HASH) != BOOK_HASH)
        {
            setPageNumber(0);
            NVS.setString(NVS_BOOK_HASH, BOOK_HASH, true);
        }
        isBookOk = true;
    }
    return NVS.getInt(NVS_BOOK_CURRENT_PAGE, 0);
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
    dUChange = true;
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
    // startHeight = startHeight - 3;
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
    if (BOOK_ON_EXIT_GO_PAGE_BACK == true)
    {
        debugLog("Going page back because of BOOK_ON_EXIT_GO_PAGE_BACK");
        changePageDown();
    }
    bool comm = NVS.commit();
    debugLog("Commit status: " + BOOL_STR(comm));
    deInitAxc();
}

#define MAX_AXC_VALUE 1000
#define MIN_AXC_VALUE -1000
int maxAxcelLimit(int value, int add)
{
    int max = value + add;
    if (max > MAX_AXC_VALUE)
    {
        max = MAX_AXC_VALUE;
    }
    return max;
}

int minAxcelLimit(int value, int sub)
{
    int min = value - sub;
    if (min < MIN_AXC_VALUE)
    {
        min = MIN_AXC_VALUE;
    }
    return min;
}

int filledAxcLine = 0;
int changedPageAxis = 0;

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
        debugLog("x: " + String(accel.x));
        // debugLog("y: " + String(accel.y));
        // debugLog("z: " + String(accel.z));
        if (maxAxcelLimit(staAx_Y, BOOK_AX_Y_BACK_VALUE) < accel.y || minAxcelLimit(staAx_Y, BOOK_AX_Y_BACK_VALUE) > accel.y)
        {
            if (maxAxcelLimit(staAx_Z, BOOK_AX_Z_BACK_VALUE) < accel.z || minAxcelLimit(staAx_Z, BOOK_AX_Z_BACK_VALUE) > accel.z)
            {
                // Imit a long back button press
                setButton(LongBack);
                return void();
            }
        }

        if (waitForReturn == false)
        {
            if (maxAxcelLimit(staAx_X, BOOK_AX_X_PAGE_CHANGE_VALUE_UP) < accel.x)
            {
                waitForReturn = true;
                changePageUp();
                changedPageAxis = accel.x;
                debugLog("Changed page up by axis move");
            }
            else if (minAxcelLimit(staAx_X, BOOK_AX_X_PAGE_CHANGE_VALUE_DOWN) > accel.x)
            {
                waitForReturn = true;
                changePageDown();
                changedPageAxis = accel.x;
                debugLog("Changed page down by axis move");
            }
        }
        else
        {
            if (maxAxcelLimit(staAx_X, BOOK_AX_X_TOLERANCE) > accel.x && minAxcelLimit(staAx_X, BOOK_AX_X_TOLERANCE) < accel.x)
            {
                waitForReturn = false;
                delayTask(LOOP_NO_SCREEN_WRITE_DELAY_MS);
                debugLog("Reset waitForReturn");
            }
        }
#if BOOK_AXC_LINE        
        int newFilledWidth;
        int valueThatWilTriggerTop = maxAxcelLimit(staAx_X, BOOK_AX_X_TOLERANCE);
        int valueThatWilTriggerBottom = minAxcelLimit(staAx_X, BOOK_AX_X_TOLERANCE);
        debugLog("accel.x: " + String(accel.x));
        debugLog("staAx_X: " + String(staAx_X));
        debugLog("valueThatWilTriggerTop: " + String(valueThatWilTriggerTop));
        debugLog("valueThatWilTriggerBottom: " + String(valueThatWilTriggerBottom));
        if (waitForReturn == false)
        {
            int middle = (valueThatWilTriggerTop + valueThatWilTriggerBottom) / 2;
            debugLog("middle: " + String(middle));
            if (middle < accel.x)
            {
                debugLog("Top line");
                newFilledWidth = map(accel.x, middle, valueThatWilTriggerTop, 0, 200);
            }
            else
            {
                debugLog("Below line");
                newFilledWidth = map(accel.x, valueThatWilTriggerBottom, middle, 200, 0);
            }
        }
        else
        {
            debugLog("changedPageAxis: " + String(changedPageAxis));
            if (accel.x < valueThatWilTriggerBottom)
            {
                debugLog("Below line return");
                if(changedPageAxis > accel.x) {
                    changedPageAxis = accel.x;
                }
                newFilledWidth = map(accel.x, changedPageAxis, valueThatWilTriggerBottom, 200, 0);
            }
            else if (accel.x > valueThatWilTriggerTop)
            {
                debugLog("Top line return");
                if(changedPageAxis < accel.x) {
                    changedPageAxis = accel.x;
                }
                newFilledWidth = map(accel.x, valueThatWilTriggerTop, changedPageAxis, 0, 200);
            }
        }
        debugLog("newFilledWidth: " + String(newFilledWidth));
        if (abs(newFilledWidth - filledAxcLine) > BOOK_AXC_DIFFERENCE_CHANGE)
        {
            filledAxcLine = newFilledWidth;
            // display.drawFastHLine(0, 1, 200, GxEPD_WHITE);
            // display.drawFastHLine(0, 1, filledAxcLine, GxEPD_BLACK);
            display.fillRect(0, 0, 200, BOOK_AXC_LINE_WIDTH, GxEPD_WHITE);
            display.fillRect(0, 0, newFilledWidth, BOOK_AXC_LINE_WIDTH, GxEPD_BLACK);
            dUChange = true;
        }
#endif
    }
    else
    {
        debugLog("Failed to get accel");
    }
    disUp();
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
