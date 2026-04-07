#include "notes.h"

#if NOTES_APP

String notesContent = "";
bool notesChanged = false;
uint8_t contentLine = 0;

int8_t batteryLevel = -1;
bool batteryLevelChanged = false;

String getBatteryLevel()
{
    if (batteryLevel == -1)
    {
        return "?%";
    }
    else
    {
        return String(batteryLevel) + "%";
    }
}

char keycodeToChar(uint8_t keycode, bool shift)
{
    if (keycode >= 4 && keycode <= 29)
    { // a-z
        return (shift ? 'A' : 'a') + (keycode - 4);
    }
    if (keycode >= 30 && keycode <= 38)
    { // 1-9
        if (shift)
        {
            const char shift_nums[] = "!@#$%^&*(";
            return shift_nums[keycode - 30];
        }
        return '1' + (keycode - 30);
    }
    if (keycode == 39)
        return shift ? ')' : '0';
    if (keycode == 40)
        return '\n'; // Enter
    if (keycode == 42)
        return 8; // Backspace (handled specially)
    if (keycode == 44)
        return ' '; // Space

    return 0;
}

void onKeyboardData(uint8_t keycode, bool shift)
{
    char c = keycodeToChar(keycode, shift);
    // debugLog("Received keycode is: " + String(c));
    if (c == 8)
    { // Backspace
        if (notesContent.length() > 0)
        {
            notesContent.remove(notesContent.length() - 1);
            notesChanged = true;
        }
    }
    else if (c != 0)
    {
        notesContent += c;
        notesChanged = true;
    }
}

uint8_t lastKeys[6] = {0, 0, 0, 0, 0, 0};
void hostBleKeyboardNotify(
    BLERemoteCharacteristic *pBLERemoteCharacteristic,
    uint8_t *pData,
    size_t length,
    bool isNotify)
{
    auto uuid = pBLERemoteCharacteristic->getUUID();

    if (uuid.equals(BLEUUID((uint16_t)0x2A19)))
    {
        // Battery Level
        if (length == 1)
        {
            batteryLevelChanged = true;
            batteryLevel = pData[0];
        }
    }
    else if (uuid.equals(BLEUUID((uint16_t)0x2A4D)))
    {
        if (length >= 8)
        {
            uint8_t modifier = pData[0];
            bool shift = (modifier & 0x02) || (modifier & 0x20);
            uint8_t currentKeys[6];

            for (int i = 0; i < 6; i++)
            {
                currentKeys[i] = pData[i + 2];
            }

            // Check for new keys
            for (int i = 0; i < 6; i++)
            {
                uint8_t key = currentKeys[i];
                if (key == 0)
                    continue;

                bool isNew = true;
                for (int j = 0; j < 6; j++)
                {
                    if (key == lastKeys[j])
                    {
                        isNew = false;
                        break;
                    }
                }

                if (isNew)
                {
                    onKeyboardData(key, shift);
                }
            }

            for (int i = 0; i < 6; i++)
            {
                lastKeys[i] = currentKeys[i];
            }
        }
    }
}

bool noteDeleted = false;
void deleteNote()
{
    fsRemoveFile(String(NOTES_DIR) + "/" + lastMenuSelected);
    noteDeleted = true;
    switchBack();
}

void initNotes()
{
    debugLog("Init notes called");
    init_general_page(1);
    GeneralPageButton button = GeneralPageButton{"Delete note", deleteNote};
    general_page_set_buttons(&button, 1);

    general_page_set_title(String("Notes: " + getBatteryLevel()).c_str());
    // fsListDir(NOTES_DIR, 1);
    notesContent = fsGetString(lastMenuSelected, "", String(NOTES_DIR) + "/");
    contentLine = genpage_add(notesContent.c_str());
    general_page_set_main();
}

void loopNotes()
{
    resetSleepDelay(SLEEP_EVERY_MS);

    if (notesChanged)
    {

        genpage_change(notesContent.c_str(), contentLine);
    }
    if (batteryLevelChanged)
    {
        general_page_set_title(String("Notes: " + getBatteryLevel()).c_str());
    }
    if (notesChanged || batteryLevelChanged)
    {
        notesChanged = false;
        batteryLevelChanged = false;
        general_page_set_main();
    }

    slint_loop();
}

void exitNotes()
{
    debugLog("Exit notes called");
    if (noteDeleted == false)
    {
        fsSetString(lastMenuSelected, notesContent, String(NOTES_DIR) + "/");
    }
    notesContent.clear();
}

#endif
