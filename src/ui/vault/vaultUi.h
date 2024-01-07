#ifndef VAULT_UI_H
#define VAULT_UI_H

#include "../../defines/defines.h"

extern long long int key;

void initVault();
void loopVault();
void showVaultImage(int index);
void exitVault();

#if DEBUG
void hexStringToByteArray(const char* hexString, unsigned char* byteArray, size_t byteArraySize);
#endif

#endif
