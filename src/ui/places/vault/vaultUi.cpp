#include "defines.h"
#include "rtcMem.h"
#include "localization.h"

#if VAULT
#include "vaultUi.h"

#include "mbedtls/base64.h"
#include "mbedtls/aes.h"

long long int key = 0;

String keyToString()
{
    String keyStr = String(key);
    while (keyStr.length() < 16)
    {
        // keyStr = "0" + keyStr;
        keyStr = keyStr + "0";
    }
    debugLog("keyStr: " + keyStr);
    return keyStr;
}

bool checkKey()
{
    // debugLog("Starting decrypting: " + String(millisBetter()));

    String encCheck = fsGetString("check_enc", "", "/vault/conf/");
    if (encCheck == "")
    {
        debugLog("There is no enc check!");
        return false;
    }
    int encCheckLen = encCheck.length();

    unsigned char *realImage = new unsigned char[encCheckLen];

    size_t written = 0;

    debugLog("Before base64 encoding");

    int baseResult = mbedtls_base64_decode(realImage, encCheckLen, &written, (const unsigned char *)encCheck.c_str(), encCheckLen);

    debugLog("Written base64 bytes: " + String(written));
    debugLog("base64 result: " + String(baseResult));

    // debugLog("Original base64 image:");

    mbedtls_aes_context aes;

    unsigned char keyChar[17] = {0};

    String keyString = keyToString();
    memcpy(keyChar, keyString.c_str(), keyString.length() + 1);

    mbedtls_aes_init(&aes);
    int resultKey = mbedtls_aes_setkey_dec(&aes, keyChar, 128);
    debugLog("resultKey: " + String(resultKey));

    int resultCrypt = mbedtls_aes_crypt_ecb(&aes, MBEDTLS_AES_DECRYPT, realImage, realImage);
    debugLog("resultCrypt: " + String(resultCrypt));

    // debugLog("Finished decrypting: " + String(millisBetter()));

    mbedtls_aes_free(&aes);

    String decryptedAnswer = String(realImage, 16);
    debugLog("Decrypted string is: " + decryptedAnswer);

    delete[] realImage;
    if (decryptedAnswer == fsGetString("check_dec", "", "/vault/conf/"))
    {
        debugLog("Key is correct");
        return true;
    }
    else
    {
        debugLog("Key is wrong");
        return false;
    }
}

String getSault()
{
    debugLog("Starting getSault");
    String saultEnc = fsGetString("sault", "", "/vault/conf/");
    if (saultEnc == "")
    {
        debugLog("There is no sault");
        return "";
    }
    int saultLen = saultEnc.length();
    debugLog("Sault is: " + saultEnc);

    unsigned char *realImage = new unsigned char[saultLen];

    size_t written = 0;

    debugLog("Before base64 encoding");

    int baseResult = mbedtls_base64_decode(realImage, saultLen, &written, (const unsigned char *)saultEnc.c_str(), saultLen);

    debugLog("Written base64 bytes: " + String(written));
    debugLog("base64 result: " + String(baseResult));

    mbedtls_aes_context aes;

    unsigned char keyChar[17] = {0};

    String keyString = keyToString();
    memcpy(keyChar, keyString.c_str(), keyString.length() + 1);

    mbedtls_aes_init(&aes);
    int resultKey = mbedtls_aes_setkey_dec(&aes, keyChar, 128);
    debugLog("resultKey: " + String(resultKey));

    int resultCrypt = mbedtls_aes_crypt_ecb(&aes, MBEDTLS_AES_DECRYPT, realImage, realImage);
    debugLog("resultCrypt: " + String(resultCrypt));

    // debugLog("Finished decrypting: " + String(millisBetter()));

    mbedtls_aes_free(&aes);

    String decryptedAnswer = String(realImage, 16);
    debugLog("Decrypted sault is: " + decryptedAnswer);

    delete[] realImage;
    return decryptedAnswer;
}

void initVault()
{
    debugLog("initVault key: " + String(key));
    // key = String(VAULT_PASSWORD).toDouble();
    if (key == 0)
    {
        pinInputVar = &key;
        generalSwitch(inputPinPlace);
    }
    else
    {
        if (checkKey() == false)
        {

            debugLog("Cleaning key, launching text dialog");
            key = -1;
            key = 0;
            generalSwitch(textDialog);
            showTextDialog(VAULT_KEY_INCORRECT);
            return;
        }
        int itemsInDir = fsItemsInDir("/vault/") - 2; // - conf totp
        debugLog("items in vault: " + String(itemsInDir));
        if (itemsInDir <= 0)
        {
            generalSwitch(textDialog);
            showTextDialog(VAULT_EMPTY);
            return;
        }
        String foundMenuItemName = "";
        File root = LittleFS.open("/vault/");
        File file = root.openNextFile();
        while (file)
        {
            String fileName = String(file.name());
            debugLog("The vault filename: " + fileName);
            if (lastMenuSelected == fileName)
            {
                foundMenuItemName = fileName;
                break;
            }
            file = root.openNextFile();
        }
        file.close();
        root.close();

        if (foundMenuItemName == "")
        {   
            bool hasTotp = totpExists();
            itemsInDir+=hasTotp;
            entryMenu buttons[itemsInDir];


            int counter = 0;
            if(hasTotp) {
                buttons[counter] = {"TOTP", &emptyImgPack,switchTotpMenu};
                counter++;
            }
            File root = LittleFS.open("/vault/");
            File file = root.openNextFile();

            while (file)
            {
                if (file.isDirectory() == false)
                {
                    buttons[counter] = {String(file.name()), &emptyImgPack, switchBack};
                    counter = counter + 1;
                }
                file = root.openNextFile();
            }
            file.close();
            root.close();

            initMenu(buttons, itemsInDir, "Vault");
            generalSwitch(vaultMenu);
        }
        else
        {
            showVaultImage(foundMenuItemName);
            generalSwitch(vaultImage);
            lastMenuSelected = "";
        }
    }
}

void loopVault()
{
    useButtonBlank();
}

bool totpExists() {
    int itemsInDir = fsItemsInDir("/vault/totp/");
    debugLog("items in vault/totp: " + String(itemsInDir));
    bool totpExists = itemsInDir>0;
    return totpExists;
}

void exitVault()
{
    debugLog("Exiting vault");
    debugLog("rM.currentPlace: " + String(rM.currentPlace));
    debugLog("rM.placeTree[currentPlaceIndex]: " + String(rM.placeTree[currentPlaceIndex]));
    if (currentPlaceIndex <= 1)
    {
        debugLog("Cleaning key");
        key = -1;
        key = 0;
    }
    else
    {
        debugLog("Setting vault exit vault");
        exitFuncGlob = exitVault;
    }
}

void showVaultImage(String file)
{
    if (key != 0)
    {
        debugLog("Showing vault image");
        // debugLog("Starting decrypting: " + String(millisBetter()));

        bufSize vaultItem = fsGetBlob(file, "/vault/");
        debugLog("VaultItem size: " + String(vaultItem.size));

        unsigned char *realImage = new unsigned char[vaultItem.size];

        size_t written = 0;

        debugLog("Before base64 encoding");

        int baseResult = mbedtls_base64_decode(realImage, vaultItem.size, &written, vaultItem.buf, vaultItem.size);

        debugLog("Written base64 bytes: " + String(written));
        debugLog("base64 result: " + String(baseResult));

        mbedtls_aes_context aes;

        unsigned char keyChar[17] = {0};

        String keyString = keyToString();
        memcpy(keyChar, keyString.c_str(), keyString.length() + 1);

        unsigned char iv[16] = {0};
        hexStringToByteArray(getSault().c_str(), iv, 16);

        mbedtls_aes_init(&aes);
        int resultKey = mbedtls_aes_setkey_dec(&aes, keyChar, 128);
        debugLog("resultKey: " + String(resultKey));
        int resultCrypt = mbedtls_aes_crypt_cbc(&aes, MBEDTLS_AES_DECRYPT, written, iv, realImage, realImage);
        debugLog("resultCrypt: " + String(resultCrypt));

        // debugLog("Finished decrypting: " + String(millisBetter()));

        mbedtls_aes_free(&aes);

        ImageDef newImage = {200, 200, realImage};
        writeImageN(0, 0, &newImage);
        // This is a workarround for random pixels at the top. I didn't found the cause
        /*
            On PC every checksum, before, after decrypting looks fine
            so the issue is here, in decrypting. Maybe in the base64 part, maybe in the decrypting itself, idk
        */
        dis->drawFastHLine(0, 0, 200, GxEPD_WHITE);
        disUp(true);

        delete[] realImage;
        resetSleepDelay(30000);

        /*
        debugLog("Decrypted image in base64:");
        for (size_t i = 0; i < written; i++)
        {
            Serial.print(String(realImage[i], HEX));
            Serial.print(" ");
        }
        Serial.println("");

        debugLog("Original base64 image:");
        for (size_t i = 0; i < testImgVaultImg_len; i++)
        {
            Serial.print(String(testImgVaultImg[i], HEX));
            Serial.print(" ");
        }
        Serial.println("");
        */
    }
    else
    {
        debugLog("key is not available");
    }
}
String generateCode(String key) {
    debugLog("key=\"" + key + "\"");
    readRTC();
    unsigned long t = getUnixTime(timeRTCUTC0);
    debugLog("t=" + String(t));

    // Decode base32 key parameter to bytes, dynamically allocate buffer
    size_t decodedLen = (key.length() * 5) / 8 + 1;
    unsigned char* hmacKey = new unsigned char[decodedLen];
    size_t hmacKeyLen = 0;

    auto base32_decode_local = [](const char* encoded, unsigned char* result, size_t bufSize) -> size_t {
        const char* base32_chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZ234567";
        size_t buffer = 0, bitsLeft = 0, count = 0;
        for (const char* ptr = encoded; *ptr && count < bufSize; ++ptr) {
            char ch = *ptr;
            if (ch == '=' || ch == ' ' || ch == '\t' || ch == '\n' || ch == '\r')
                continue;
            const char* p = strchr(base32_chars, toupper(ch));
            if (!p)
                continue;
            buffer <<= 5;
            buffer |= (p - base32_chars) & 0x1F;
            bitsLeft += 5;
            if (bitsLeft >= 8) {
                result[count++] = (buffer >> (bitsLeft - 8)) & 0xFF;
                bitsLeft -= 8;
            }
        }
        return count;
    };

    hmacKeyLen = base32_decode_local(key.c_str(), hmacKey, decodedLen);
    TOTP totp = TOTP(hmacKey, hmacKeyLen);
    String code = totp.getCode(t);

    return code;
}
String decryptTotpFile(String file) {
    String encCheck = fsGetString(file, "", "/vault/totp/");
    debugLog("encCheck="+encCheck);
    if (encCheck == "")
    {
        debugLog("There is no enc check!");
        return "";
    }
    int encCheckLen = encCheck.length();

    unsigned char *realImage = new unsigned char[encCheckLen];
    size_t written = 0;
    debugLog("Before base64 encoding");
    int baseResult = mbedtls_base64_decode(realImage, encCheckLen, &written, (const unsigned char *)encCheck.c_str(), encCheckLen);
    debugLog("Written base64 bytes: " + String(written));
    debugLog("base64 result: " + String(baseResult));

    mbedtls_aes_context aes;
    unsigned char keyChar[17] = {0};
    String keyString = keyToString();
    memcpy(keyChar, keyString.c_str(), keyString.length() + 1);
    mbedtls_aes_init(&aes);
    int resultKey = mbedtls_aes_setkey_dec(&aes, keyChar, 128);
    debugLog("resultKey: " + String(resultKey));

    // Decrypt in 16-byte blocks
    size_t blockSize = 16;
    size_t numBlocks = (written + blockSize - 1) / blockSize;
    unsigned char *decrypted = new unsigned char[numBlocks * blockSize];
    for (size_t i = 0; i < numBlocks; ++i) {
        mbedtls_aes_crypt_ecb(&aes, MBEDTLS_AES_DECRYPT, realImage + i * blockSize, decrypted + i * blockSize);
    }
    mbedtls_aes_free(&aes);

    // Build string from actual decoded length (written)
    String decryptedAnswer = String((char*)decrypted, written);
    delete[] realImage;
    delete[] decrypted;
    return decryptedAnswer;
}
void showTotpCode(String file) {
    String decryptedAnswer = decryptTotpFile(file);
    debugLog("Decrypted string is: " + decryptedAnswer);
    String code = generateCode(decryptedAnswer);
    debugLog("code is "+code);
    showTextDialog("Code is: "+code);
}

void hexStringToByteArray(const char *hexString, unsigned char *byteArray, size_t byteArraySize)
{
    for (size_t i = 0; i < byteArraySize; i++)
    {
        byteArray[i] = strtoul(hexString + 2 * i, NULL, 16);
    }
}

void initVaultTotp() {
    int itemsInDir = fsItemsInDir("/vault/totp/");
    debugLog("items in vault/totp: " + String(itemsInDir));

    String foundMenuItemName = "";
    File root = LittleFS.open("/vault/totp/");
    File file = root.openNextFile();
    entryMenu buttons[itemsInDir+1];
    int counter = 0;
    while (file)
    {
        String fileName = String(file.name());
        debugLog("The vault/totp filename: " + fileName);
        debugLog("totp button counter = "+String(counter));
        buttons[counter] = {fileName, &emptyImgPack,switchTotpValue};
        file = root.openNextFile();
        counter++;
    }
    file.close();
    root.close();

    initMenu(buttons,itemsInDir,"TOTP");
    generalSwitch(vaultTotpMenu);
}
void loopVaultTotp() {
    useButton();
}
void exitVaultTotp() {
    exitVault();
}

void onTotpSelect() {
    debugLog("lastMenuSelected="+String(lastMenuSelected));

    String foundMenuItemName = "";
    File root = LittleFS.open("/vault/totp/");
    File file = root.openNextFile();
    while (file)
    {
        String fileName = String(file.name());
        debugLog("The totp filename: " + fileName);
        if (lastMenuSelected == fileName)
        {
            foundMenuItemName = fileName;
            break;
        }
        file = root.openNextFile();
    }
    file.close();
    root.close();

    if (foundMenuItemName!="")
    {
        showTotpCode(foundMenuItemName);
    }
    
}

void drawTotpValue() {
    uint16_t h;
    setFont(&FreeSansBold9pt7b);
    setTextSize(1);
    dis->setCursor(0, 1);
    String menuName = "Totp: "+lastMenuSelected;
    debugLog(menuName);
    getTextBounds(menuName, NULL, NULL, NULL, &h);
    if (containsBelowChar(menuName) == true)
    {
        h = h + 2;
    }
    maxHeight = h;
    uint16_t currentHeight = maxHeight;
    dis->setCursor(0, currentHeight - 3);
    dis->print(menuName);

    dis->fillRect(0, currentHeight, dis->width(), 3, GxEPD_BLACK);
    currentHeight = currentHeight + maxHeight;

    String decryptedKey = decryptTotpFile(lastMenuSelected);
    setFont(getFont("JackInput17"));
    setTextSize(1);
    currentHeight+=15;
    centerText(generateCode(decryptedKey), &currentHeight);
    unsigned long t = getUnixTime(timeRTCUTC0);
    float progress = (float)(t % 30) / 30 * 100;
    debugLog("Progress "+String(progress));
    drawProgressBar(5,175,190,10,progress);
    disUp(true);
}
void initVaultTotpValue() {
    
}

void loopVaultTotpValue() {
    dis->fillScreen(GxEPD_WHITE);
    drawTotpValue();
    useButton();
}
void exitVaultTotpValue() {
    
}
/*
void printHex(const unsigned char *data, size_t len)
{
    for (size_t i = 0; i < len; i++)
    {
        if (data[i] < 0x10)
        {
            Serial.print("0");
        }
        Serial.print(data[i], HEX);
        Serial.print(" ");
    }
    Serial.println();
}
*/

#endif

/*
// This kind of works
// Static key (16 bytes for AES-128)
    const unsigned char key[16] = {0x6B, 0x65, 0x79, 0x6B, 0x65, 0x79, 0x6B, 0x65, 0x79, 0x79, 0x6B, 0x65, 0x79, 0x6B, 0x65, 0x79}; // keykeykeyykeykey

    // Static input data (16 bytes for AES-128)
    const unsigned char inputData[16] = {0x77,0x68,0x6F,0x63,0x61,0x72,0x65,0x73,0x77,0x68,0x6F,0x63,0x61,0x72,0x65,0x73}; // whocareswhocares

    // Output buffer
    unsigned char outputData[16] = { 0 }; // = {43,39,39,37,38,45,41,34,41,36,43,45,45,46,46,38,44,35,41,39,37,45,30,34,45,46,42,43,39,44,34,34};


    // Encryption
    mbedtls_aes_context aes;
    mbedtls_aes_init(&aes);

    if (mbedtls_aes_setkey_enc(&aes, key, 128) != 0) {
        Serial.println("Failed to set encryption key");
        return;
    }

    if (mbedtls_aes_crypt_ecb(&aes, MBEDTLS_AES_ENCRYPT, inputData, outputData) != 0) {
        Serial.println("Encryption failed");
        return;
    }
    Serial.print("Encrypted Data: ");
    printHex(outputData, sizeof(outputData));

    // Decryption
    if (mbedtls_aes_setkey_dec(&aes, key, 128) != 0) {
        Serial.println("Failed to set decryption key");
        return;
    }

    if (mbedtls_aes_crypt_ecb(&aes, MBEDTLS_AES_DECRYPT, outputData, outputData) != 0) {
        Serial.println("Decryption failed");
        return;
    }

    Serial.print("Decrypted Data: ");
    printHex(outputData, sizeof(outputData));

    mbedtls_aes_free(&aes);

*/

// Nothing. worked.
/*
#include "mbedtls/aes.h"


if (key != 0)
{
    debugLog("Starting decrypting: " + String(millisBetter()));
    String keyStr = String(key);
    uint8_t keyBytes[KEY_LENGTH] = {0};
    keyStr.getBytes(keyBytes, KEY_LENGTH);

    String ivStr = VAULT_SAULT;
    uint8_t IvBytes[KEY_LENGTH] = {0};
    ivStr.getBytes(IvBytes, KEY_LENGTH);

    unsigned char realImage[5008] = {0};

    mbedtls_aes_context aes;
    mbedtls_aes_init(&aes);
    int resultKey = mbedtls_aes_setkey_enc(&aes, keyBytes, KEY_LENGTH * 8);
    debugLog("Result key is: " + String(resultKey)); // 0 means good

#if defined(MBEDTLS_CIPHER_MODE_CBC)
    debugLog("CBC AES is enabled");
#endif

    //debugLog("Size of bitmap: " + String(sizeof(vaultList[index].bitmap)));
    int result = mbedtls_aes_crypt_cbc(&aes, MBEDTLS_AES_DECRYPT, 5008, IvBytes, vaultList[index].bitmap, realImage);
    debugLog("End of decrypting: " + String(millisBetter()));
    debugLog("Result is: " + String(result)); // 0 means good

    ImageDef newImage = {200, 200, realImage};
    writeImageN(0, 0, newImage);
    disUp(true);
}
else
{
    debugLog("key is not available");
}

unsigned char key[17] = "0123456789abcdef";
unsigned char iv[17] = "0123456789abcdef";
//uint8_t key[16] = {30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 61, 62, 63, 64, 65, 66};
//uint8_t iv[16] = {30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 61, 62, 63, 64, 65, 66};

// Convert String to char array
uint8_t encryptedCharArray[16] = {0x7d, 0x96, 0x9a, 0x22, 0x5a, 0x81, 0x9b, 0xe0, 0x86, 0x81, 0xab, 0x92,
                                  0x4b, 0x1f, 0x37, 0x83};

// Assuming your output buffer is large enough to hold the decrypted data
uint8_t decryptedData[256];

// Decrypt directly in the loop
mbedtls_aes_context aes;
mbedtls_aes_init(&aes);

int resultKey = mbedtls_aes_setkey_dec(&aes, key, 128);
debugLog("Result key is: " + String(resultKey)); // 0 means good

int result = mbedtls_aes_crypt_cbc(&aes, MBEDTLS_AES_DECRYPT, 16, iv, encryptedCharArray, decryptedData);
debugLog("Result is: " + String(result)); // 0 means good

mbedtls_aes_free(&aes);

// Use the decryptedData as needed, print it, etc.
Serial.println("Decrypted Data: ");
String heeeeelp = String((char *)(decryptedData), 16);
Serial.println(heeeeelp);
*/