#include "setTime.h"

int setTimeMinute = 0;
int setTimeHour = 0;

int indexNumber = 0;
int oneNumber = 0;
int twoNumber = 0;
int threeNumber = 0;
int fourNumber = 0;

#define CLOCK_HEIGHT 100
#define INDEX_INIT_X 20
#define INDEX_INIT_Y CLOCK_HEIGHT + 7
#define INDEX_JUMP 40
#define INDEX_JUMP_MIDDLE 33
#define SET_TIME_FONT getFont("Mono33")

#define INDEX_LINE_WIDTH 10

void drawIndex(int x, int y)
{
    // dis->drawPixel(x, y, GxEPD_BLACK);
    // dis->drawCircle(x, y, 10, GxEPD_BLACK);
    dis->fillTriangle(x, y, x + INDEX_LINE_WIDTH, y + INDEX_LINE_WIDTH, x - INDEX_LINE_WIDTH, y + INDEX_LINE_WIDTH, GxEPD_BLACK);
}

void drawThings()
{
    dis->fillScreen(GxEPD_WHITE);
    uint16_t h = CLOCK_HEIGHT;
    writeTextCenterReplaceBack(String(oneNumber) + String(twoNumber) + ":" + String(threeNumber) + String(fourNumber), h, GxEPD_BLACK);

    int x = INDEX_INIT_X;
    int y = INDEX_INIT_Y;
    x = x + (INDEX_JUMP * indexNumber);
    if (indexNumber > 1)
    {
        x = x + INDEX_JUMP_MIDDLE;
    }
    drawIndex(x, y);

    dUChange = true;
}

void initSetTime()
{
    String hour = String(setTimeHour);
    while(hour.length() < 2) {
        hour = "0" + hour;
    }
    String minute = String(setTimeMinute);
    while(minute.length() < 2) {
        minute = "0" + minute;
    }
    oneNumber = String(hour.charAt(0)).toInt();
    twoNumber = String(hour.charAt(1)).toInt();
    threeNumber = String(minute.charAt(0)).toInt();
    fourNumber = String(minute.charAt(1)).toInt();
    indexNumber = 0;
    setFont(SET_TIME_FONT);
    drawThings();
}

void checkMaxMinSetTime()
{
    checkMaxMin(&indexNumber, 3);

    checkMaxMin(&oneNumber, 2);   // Hour 1
    if(oneNumber <= 1) {
        checkMaxMin(&twoNumber, 9);   // Hour 2
    } else {
        checkMaxMin(&twoNumber, 3);
    }
    checkMaxMin(&threeNumber, 5); // Minute 1
    checkMaxMin(&fourNumber, 9);  // Minute 2
}

void addNumbers(bool up)
{
    int number = 1;
    if (up == false)
    {
        number = -1;
    }
    if (indexNumber == 0)
    {
        oneNumber = oneNumber + number;
    }
    if (indexNumber == 1)
    {
        twoNumber = twoNumber + number;
    }
    if (indexNumber == 2)
    {
        threeNumber = threeNumber + number;
    }
    if (indexNumber == 3)
    {
        fourNumber = fourNumber + number;
    }
    checkMaxMinSetTime();
    drawThings();
}

void loopSetTime()
{
    buttonState btn = useButton();
    switch (btn)
    {
    case Menu:
    {
        indexNumber = indexNumber + 1;
        checkMaxMinSetTime();
        drawThings();
        break;
    }
    case Up:
    {
        addNumbers(true);
        break;
    }
    case Down:
    {
        addNumbers(false);
        break;
    }
    }

    disUp();
}

void exitSetTime()
{
    setTimeHour = String(String(oneNumber) + String(twoNumber)).toInt();
    setTimeMinute = String(String(threeNumber) + String(fourNumber)).toInt();
}