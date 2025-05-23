#pragma once

#include "defines.h"

extern long long int key;

void initVault();
void loopVault();
bool totpExists();
void showVaultImage(String file);
String generateCode(String key);
void showTotpCode(String file);
void exitVault();

void initVaultTotp();
void loopVaultTotp();
void exitVaultTotp();

void initVaultTotpValue();
void loopVaultTotpValue();
void exitVaultTotpValue();

void onTotpSelect();

void hexStringToByteArray(const char* hexString, unsigned char* byteArray, size_t byteArraySize);

