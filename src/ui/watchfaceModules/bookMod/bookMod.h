#ifndef BOOKMOD_H
#define BOOKMOD_H

#include "../../../defines/defines.h"

#if BOOK_MODULE_ENABLED

extern RTC_DATA_ATTR wfModule wfBook;
void bitcoinSync();

#endif
#endif