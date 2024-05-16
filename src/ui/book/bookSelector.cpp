#include "bookSelector.h"

void initBookSelector()
{
    int books = 0;
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
    debugLog("There are so many books: " + String(books));
}

void selectedBook()
{
}