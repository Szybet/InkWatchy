#include "bookSelector.h"
#include "localization.h"

#if BOOK

void initBookSelector()
{
    debugLog("Current book: " + fsGetString(CONF_BOOK_CURRENT_BOOK, "", "/book/conf/"));
    int books = 0;
    {

        File root = LittleFS.open("/book/");
        if (!root)
        {
            debugLog("Failed to open directory book");
            return;
        }
        if (root.isDirectory() == false)
        {
            debugLog("Not a directory book?");
            return;
        }
        File file = root.openNextFile();
        while (file)
        {
            if (file.isDirectory() == false)
            {
                books = books + 1;
            }
            file = root.openNextFile();
        }
        root.close();
    }
    debugLog("There are so many books: " + String(books));
    entryMenu buttons[books];
    File root = LittleFS.open("/book/");
    File file = root.openNextFile();
    uint8_t counter = 0;
    while (file)
    {
        if (file.isDirectory() == false)
        {
            buttons[counter] = {String(file.name()), &emptyImgPack, selectedBook};
            counter = counter + 1;
        }
        file = root.openNextFile();
    }
    root.close();
    initMenu(buttons, counter, MENU_SELECT_BOOK, 1);
}

void selectedBook()
{
    String latestBook = lastMenuSelected;
    debugLog("Called selectedBook: " + latestBook);
    fsSetString(CONF_BOOK_CURRENT_BOOK, latestBook, "/book/conf/");
    setButton(Back);
    resetBookVars();
}
#endif