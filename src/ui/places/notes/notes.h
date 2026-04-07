#pragma once

#include "defines.h"

#if NOTES_APP

void initNotes();
void loopNotes();
void exitNotes();
void hostBleKeyboardNotify(
    BLERemoteCharacteristic *pBLERemoteCharacteristic,
    uint8_t *pData,
    size_t length,
    bool isNotify);

#endif
