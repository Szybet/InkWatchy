#include "paint.h"

#if PAINT
#include "rtcMem.h"

uint8_t paintX = 0;
uint8_t paintY = 0;
uint8_t lastPaintX = 0;
uint8_t lastPaintY = 0;
uint8_t cursorX = 0;
uint8_t cursorY = 0;
uint8_t cursorMovementCount = 0;
bool isDrawing = false;
uint8_t *savedFramebuffer = nullptr;

int lastAccX = 0;
int lastAccY = 0;

#define PAINT_CURSOR_SIZE 2
#define FRAMEBUFFER_SIZE 5000

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
    cursorMovementCount = 0;
    isDrawing = false;
    lastAccX = 0;
    lastAccY = 0;

    cleanAllMemory();
    if (savedFramebuffer == nullptr)
    {
        savedFramebuffer = (uint8_t *)malloc(FRAMEBUFFER_SIZE);
        if (savedFramebuffer != nullptr)
        {
            memset(savedFramebuffer, 255, FRAMEBUFFER_SIZE);
        }
    }

    updateStatus();
    disUp(true);
    initAcc();
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
            cursorMovementCount = 15;

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
        }
        else
        {
            if (cursorMovementCount != 0)
            {
                cursorMovementCount -= 1;

                if (savedFramebuffer != nullptr)
                {
                    memcpy(dis->_buffer, savedFramebuffer, FRAMEBUFFER_SIZE);
                }

                updateStatus();
            }

            dis->fillCircle(cursorX, cursorY, PAINT_CURSOR_SIZE, SCWhite);
            cursorX = paintX;
            cursorY = paintY;
            dis->fillCircle(cursorX, cursorY, PAINT_CURSOR_SIZE, SCBlack);
        }

        lastPaintX = paintX;
        lastPaintY = paintY;
        dUChange = true;

        if (abs(lastAccX - moveX) > 30 || abs(lastAccY - moveY) > 30)
        {
            resetSleepDelay();
            debugLog("lastAccX is: " + String(lastAccX) + "lastAccY is:  " + String(lastAccY));
            lastAccX = acc.x;
            lastAccY = acc.y;
        }
    }

    disUp();
}

void exitPaint()
{
    if (savedFramebuffer != nullptr)
    {
        free(savedFramebuffer);
        savedFramebuffer = nullptr;
    }
}

#endif
