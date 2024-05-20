#ifndef BOOK_UI_H
#define BOOK_UI_H

#include "../../defines/defines.h"

void initBook();
void loopBook();
String bookGetPages();
void resetBookVars();
String getCurrentBook();
int getLastPageNumber();
void exitBook();
void changePageDown();
void changePageUp();

#endif
