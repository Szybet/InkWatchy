#include "paint.h"

#if PAINT
#include "rtcMem.h"

uint8_t paintX = 0;
uint8_t paintY = 0;
uint8_t lastPaintX = 0;
uint8_t lastPaintY = 0;
uint8_t cursorX = 0;
uint8_t cursorY = 0;
enum PaintState
{
    STATE_DRAWING,
    STATE_CURSOR,
    STATE_PAUSED
};

PaintState currentPaintState = STATE_CURSOR;
uint8_t *savedFramebuffer = nullptr;

uint8_t veryLastPaintX = 0;
uint8_t veryLastPaintY = 0;
uint8_t loopCount = 0;

#define PAINT_CURSOR_SIZE 2
#define FRAMEBUFFER_SIZE 5000
#define SLEEP_DIFF 20

void updateStatus()
{
    dis->fillRect(0, 0, 50, 10, SCWhite);
    dis->setCursor(1, 8);
    setFont(getFont("dogicapixel4"));
    dis->setTextColor(SCBlack);
    switch (currentPaintState)
    {
    case STATE_DRAWING:
        dis->print("Drawing");
        break;
    case STATE_CURSOR:
        dis->print("Cursor");
        break;
    case STATE_PAUSED:
        dis->print("Paused");
        break;
    }
}

void initPaint()
{
    dis->fillScreen(SCWhite);
    paintX = dis->width() / 2;
    paintY = dis->height() / 2;
    lastPaintX = paintX;
    lastPaintY = paintY;
    cursorX = paintX;
    cursorY = paintY;
    currentPaintState = STATE_CURSOR;
    lastPaintX = 0;
    lastPaintY = 0;
    veryLastPaintX = 0;
    veryLastPaintY = 0;
    loopCount = 0;

    cleanAllMemory();
    savedFramebuffer = (uint8_t *)malloc(FRAMEBUFFER_SIZE);
    memset(savedFramebuffer, 255, FRAMEBUFFER_SIZE);

    updateStatus();
    disUp(true);
    initAcc();
    resetSleepDelay(SLEEP_EVERY_MS);
}

void loopPaint()
{
    buttonState btn = useButton();
    if (btn == Menu)
    {
        if (currentPaintState == STATE_DRAWING)
        {
            memcpy(savedFramebuffer, dis->_buffer, FRAMEBUFFER_SIZE);
            currentPaintState = STATE_CURSOR;
        }
        else if (currentPaintState == STATE_CURSOR)
        {
            memcpy(dis->_buffer, savedFramebuffer, FRAMEBUFFER_SIZE);
            currentPaintState = STATE_DRAWING;
        }

        updateStatus();
        lastPaintX = paintX;
        lastPaintY = paintY;
        dUChange = true;
    }
    else if (btn == LongMenu)
    {
        if (currentPaintState != STATE_PAUSED)
        {
            currentPaintState = STATE_PAUSED;
            memcpy(dis->_buffer, savedFramebuffer, FRAMEBUFFER_SIZE);
            updateStatus();
            dUChange = true;
        }
        else
        {
            currentPaintState = STATE_CURSOR;
        }
    }

    Accel acc;
    if (currentPaintState != STATE_PAUSED && rM.SBMA.getAccel(&acc))
    {
        float degX = getAxisDegrees(acc.x, acc.y, acc.z);
        float degY = getAxisDegrees(acc.y, acc.x, acc.z);

        int moveX = (int)(degX / 3.0);
        int moveY = (int)(degY / 3.0);

        paintX += moveX;
        paintY += moveY;

        if (paintX < 0)
            paintX = 0;
        if (paintX >= dis->width())
            paintX = dis->width() - 1;
        if (paintY < 0)
            paintY = 0;
        if (paintY >= dis->height())
            paintY = dis->height() - 1;

        if (currentPaintState == STATE_DRAWING)
        {
            for (int i = -2; i <= 2; i++)
            {
                dis->drawLine(lastPaintX + i, lastPaintY, paintX + i, paintY, SCBlack);
                dis->drawLine(lastPaintX, lastPaintY + i, paintX, paintY + i, SCBlack);
            }
            lastPaintX = paintX;
            lastPaintY = paintY;
        }
        else if (currentPaintState == STATE_CURSOR)
        {

            memcpy(dis->_buffer, savedFramebuffer, FRAMEBUFFER_SIZE);
            updateStatus();
            cursorX = paintX;
            cursorY = paintY;
            dis->fillCircle(cursorX, cursorY, PAINT_CURSOR_SIZE, SCBlack);
        }
        dUChange = true;

        loopCount++;
        if (loopCount % 5 == 0)
        {
            if (abs(veryLastPaintX - paintX) > SLEEP_DIFF || abs(veryLastPaintY - paintY) > SLEEP_DIFF)
            {
                resetSleepDelay(SLEEP_EVERY_MS);
                veryLastPaintX = paintX;
                veryLastPaintY = paintY;
            }
            loopCount = 0;
        }
    }
    else
    {
        resetSleepDelay(SLEEP_EVERY_MS);
    }

    disUp();
}

void exitPaint()
{

    free(savedFramebuffer);
    savedFramebuffer = nullptr;
}

#endif
