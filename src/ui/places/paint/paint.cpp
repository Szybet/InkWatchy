#include "paint.h"

#if PAINT
#include "rtcMem.h"

uint8_t paintX = 0;
uint8_t paintY = 0;
uint8_t lastPaintX = 0;
uint8_t lastPaintY = 0;
uint8_t cursorX = 0;
uint8_t cursorY = 0;
bool isDrawing = false;
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
    dis->print(isDrawing ? "Drawing" : "Paused");
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
    isDrawing = false;
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
    if (useButton() == Menu)
    {
        isDrawing = !isDrawing;

        if (isDrawing)
        {
            if (savedFramebuffer != nullptr)
            {
                memcpy(dis->_buffer, savedFramebuffer, FRAMEBUFFER_SIZE);
            }
        }
        else
        {
            if (savedFramebuffer != nullptr)
            {
                memcpy(savedFramebuffer, dis->_buffer, FRAMEBUFFER_SIZE);
            }
        }

        updateStatus();
        lastPaintX = paintX;
        lastPaintY = paintY;
        dUChange = true;
    }

    Accel acc;
    if (rM.SBMA.getAccel(&acc))
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

        if (isDrawing)
        {
            for (int i = -2; i <= 2; i++)
            {
                dis->drawLine(lastPaintX + i, lastPaintY, paintX + i, paintY, SCBlack);
                dis->drawLine(lastPaintX, lastPaintY + i, paintX, paintY + i, SCBlack);
            }
            lastPaintX = paintX;
            lastPaintY = paintY;
        }
        else
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

    disUp();
}

void exitPaint()
{

    free(savedFramebuffer);
    savedFramebuffer = nullptr;
}

#endif
