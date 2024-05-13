#include "../../defines/config.h"
#if BOOK
#include "bookUi.h"
#include "../../defines/book.h"

#define BOOK_FONT &monofonto_rg10pt7b
bool isBookOk = false;
bool excOn = true;

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

void resetSleepDelayBook()
{
    resetSleepDelay(BOOK_ADD_SLEEP_DELAY_MS);
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

#define MIDDLE_MULTI 0.075
int axis_XTop;
int axis_XBott;
int axis_XMiddle;
int axis_XMiddleTop;
int axis_XMiddleBott;
int axis_YTop;
int axis_YBott;
int axis_ZTop;
int axis_ZBott;

void resetStartAxc()
{
    bma4_accel accel;
    SBMA.getAccel(accel);
    staAx_X = accel.x;
    staAx_Y = accel.y;
    staAx_Z = accel.z;
    axis_XTop = maxAxcelLimit(staAx_X, BOOK_AX_X_TOLERANCE);
    axis_XBott = minAxcelLimit(staAx_X, BOOK_AX_X_TOLERANCE);

    axis_XMiddle = ((axis_XTop + axis_XBott) / 2);
    if (axis_XMiddle > 0)
    {
        axis_XMiddleTop = axis_XMiddle * (1.0 + MIDDLE_MULTI);
        axis_XMiddleBott = axis_XMiddle * (1.0 - MIDDLE_MULTI);
    }
    else
    {
        axis_XMiddleBott = axis_XMiddle * (1.0 + MIDDLE_MULTI);
        axis_XMiddleTop = axis_XMiddle * (1.0 - MIDDLE_MULTI);
    }

    axis_YTop = maxAxcelLimit(staAx_Y, BOOK_AX_Y_BACK_VALUE);
    axis_YBott = minAxcelLimit(staAx_Y, BOOK_AX_Y_BACK_VALUE);
    axis_ZTop = maxAxcelLimit(staAx_Z, BOOK_AX_Z_BACK_VALUE);
    axis_ZBott = minAxcelLimit(staAx_Z, BOOK_AX_Z_BACK_VALUE);
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
    resetSleepDelayBook();
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

int filledAxcLine = 0;
int changedPageAxis = 0;
enum change
{
    top,
    bottom,
    none,
};
change previousChangeDirection = none;

void loopBook()
{
    switch (useButton())
    {
    case Up:
    {
        resetStartAxc();
        changePageUp();
        waitForReturn = false;
        changedPageAxis = 0;
        previousChangeDirection = none;
        break;
    }
    case Down:
    {
        resetStartAxc();
        changePageDown();
        waitForReturn = false;
        changedPageAxis = 0;
        previousChangeDirection = none;
        break;
    }
    case Menu:
    {
        break;
    }
    case LongMenu:
    {
        excOn = !excOn;
        display.fillRect(0, 0, 200, BOOK_AXC_LINE_WIDTH, GxEPD_WHITE);
        dUChange = true;
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
    if (excOn == true && SBMA.getAccel(accel))
    {
        // for button change...
        if (changedPageAxis == 0)
        {
            changedPageAxis = accel.x;
        }
        debugLog("x: " + String(accel.x));
        // debugLog("y: " + String(accel.y));
        // debugLog("z: " + String(accel.z));
        if (axis_YTop < accel.y || axis_YBott > accel.y)
        {
            if (axis_ZTop < accel.z || axis_ZBott > accel.z)
            {
                // Imit a long back button press
                setButton(LongBack);
                return void();
            }
        }

        if (waitForReturn == false)
        {
            if (axis_XTop < accel.x)
            {
                waitForReturn = true;
                changePageUp();
                changedPageAxis = accel.x;
                previousChangeDirection = top;
                debugLog("Changed page up by axis move");
            }
            else if (axis_XBott > accel.x)
            {
                waitForReturn = true;
                changePageDown();
                changedPageAxis = accel.x;
                previousChangeDirection = bottom;
                debugLog("Changed page down by axis move");
            }
        }
        else
        {
            bool hasReturned = false;
            if (previousChangeDirection == top)
            {
                if (axis_XMiddleTop > accel.x)
                {
                    hasReturned = true;
                }
            }
            else if (previousChangeDirection == bottom)
            {
                if (axis_XMiddleBott < accel.x)
                {
                    hasReturned = true;
                }
            } else {
                debugLog("Oh no");
            }
            if (hasReturned == true)
            {
                delayTask(LOOP_NO_SCREEN_WRITE_DELAY_MS / 2);
                debugLog("Reset waitForReturn: " + String(previousChangeDirection));
                waitForReturn = false;
                previousChangeDirection = none;
            }
        }
#if BOOK_AXC_LINE
        int newFilledWidth = 0;
        // int valueThatWilTriggerTop = maxAxcelLimit(staAx_X, BOOK_AX_X_TOLERANCE);
        // int valueThatWilTriggerBottom = minAxcelLimit(staAx_X, BOOK_AX_X_TOLERANCE);
        // debugLog("accel.x: " + String(accel.x));
        debugLog("staAx_X: " + String(staAx_X));
        debugLog("axis_XTop: " + String(axis_XTop));
        debugLog("axis_XBott: " + String(axis_XBott));
        debugLog("axis_XMiddleTop: " + String(axis_XMiddleTop));
        debugLog("axis_XMiddleBott: " + String(axis_XMiddleBott));

        if (waitForReturn == false)
        {
            // debugLog("middle: " + String(axis_XMiddle));
            if (axis_XMiddle < accel.x)
            {
                debugLog("Top line");
                newFilledWidth = map(accel.x, axis_XMiddle, axis_XTop, 0, 200);
            }
            else
            {
                debugLog("Below line");
                newFilledWidth = map(accel.x, axis_XBott, axis_XMiddle, 200, 0);
            }
        }
        else
        {
            debugLog("return line management");
            // debugLog("changedPageAxis: " + String(changedPageAxis));
            if (previousChangeDirection == bottom)
            {
                if (accel.x <= axis_XMiddleBott)
                {
                    debugLog("Below line return");
                    if (changedPageAxis > accel.x)
                    {
                        changedPageAxis = accel.x;
                    }
                    newFilledWidth = map(accel.x, changedPageAxis, axis_XMiddleBott, 200, 0);
                }
                else
                {
                    debugLog("Top line was previously but we can't exit it?");
                }
            }
            else if (previousChangeDirection == top)
            {
                if (accel.x >= axis_XMiddleTop)
                {
                    debugLog("Top line return");
                    if (changedPageAxis < accel.x)
                    {
                        changedPageAxis = accel.x;
                    }
                    newFilledWidth = map(accel.x, axis_XMiddleTop, changedPageAxis, 0, 200);
                }
                else
                {
                    debugLog("Below line was previously but we can't exit it?");
                }
            }
        }
        debugLog("newFilledWidth: " + String(newFilledWidth));
        if(newFilledWidth < 0 || newFilledWidth > 200) {
            newFilledWidth = 0;
            leaveFlashMessage("newFilledWidth is wrong");
        }
        if (abs(newFilledWidth - filledAxcLine) > BOOK_AXC_DIFFERENCE_CHANGE)
        {
            filledAxcLine = newFilledWidth;
            // display.drawFastHLine(0, 1, 200, GxEPD_WHITE);
            // display.drawFastHLine(0, 1, filledAxcLine, GxEPD_BLACK);
            display.fillRect(0, 0, 200, BOOK_AXC_LINE_WIDTH, GxEPD_WHITE);
            if (waitForReturn == false)
            {
                display.fillRect(0, 0, newFilledWidth, BOOK_AXC_LINE_WIDTH, GxEPD_BLACK);
            }
            else
            {
                drawProgressBar(0, 0, newFilledWidth, BOOK_AXC_LINE_WIDTH - 1, 0);
            }
            if (dUChange == false && updateCounter > 0)
            {
                updateCounter = updateCounter - 1;
            }
            dUChange = true;
        }
#endif
    }
    else
    {
        if(excOn == true) {
            debugLog("Failed to get accel");
        }
    }
    disUp();
}

String bookGetPages()
{
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
