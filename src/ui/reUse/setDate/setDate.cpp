#include "setDate.h"

int setDateDay = 0;
int setDateMonth = 0;
int setDateYear = 0;

void initSetDate()
{
    init_set_date();
    set_date_set_date(setDateYear, setDateMonth, setDateDay);
}

void loopSetDate()
{
    resetSleepDelay(SLEEP_EVERY_MS);
    slint_loop();
}

void exitSetDate()
{
    RustDate date = set_date_get_date();
    int setDateDay = date.day;
    int setDateMonth = date.month;
    int setDateYear = date.year;
    slintExit();
}