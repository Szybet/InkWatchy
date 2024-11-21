#ifndef BOOK_UI_H
#define BOOK_UI_H

#include "defines.h"

#if BOOK

void initBook();
void loopBook();
String bookGetPages(int charsPerPage = BOOK_CHARS_PER_PAGE);
void resetBookVars();
String getCurrentBook();
int getLastPageNumber();
void exitBook();
void changePageDown(int charsPerPage = BOOK_CHARS_PER_PAGE, bool regularShow = true);
void changePageUp(int charsPerPage = BOOK_CHARS_PER_PAGE, bool regularShow = true);
String showPage(int page, bool actuallyShowIt = true, int charsPerPage = BOOK_CHARS_PER_PAGE);
int getPageNumber();
extern uint16_t startHeightBook;
void calculateBookTextHeight();

#endif
#endif
