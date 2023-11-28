#include "debug.h"

#if DEBUG && EINK_COUNTER
#define SKIP_CYCLES 7
bool randomChoosed = false;
int counterX;
int counterY;
int countSkip = SKIP_CYCLES;
int counter = 0;
void showEinkCounter()
{
    log("countSkip: " + String(countSkip));
    if (countSkip == SKIP_CYCLES)
    {
        countSkip = 0;
        setTextSize(2);
        if (randomChoosed == false)
        {
            randomChoosed = true;
            counterX = random(50, 150);
            counterY = random(50, 150);
        }
        display.setCursor(counterX, counterY);
        counter = counter + 1;
        log("Showing einkCounter: " + String(counter));
        writeTextReplaceBack(String(counter), counterX, counterY);
        display.display(PARTIAL_UPDATE);
    }
    countSkip = countSkip + 1;
}
#endif