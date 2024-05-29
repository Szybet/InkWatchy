#include "../../defines/config.h"
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
    //debugLog("Starting decrypting: " + String(millis()));

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
    Serial.flush();

    int baseResult = mbedtls_base64_decode(realImage, encCheckLen, &written, (const unsigned char *)encCheck.c_str(), encCheckLen);

    debugLog("Written base64 bytes: " + String(written));
    debugLog("base64 result: " + String(baseResult));

    debugLog("Original base64 image:");

    mbedtls_aes_context aes;

    unsigned char keyChar[17] = {0};

    String keyString = keyToString();
    memcpy(keyChar, keyString.c_str(), keyString.length() + 1);

    mbedtls_aes_init(&aes);
    int resultKey = mbedtls_aes_setkey_dec(&aes, keyChar, 128);
    debugLog("resultKey: " + String(resultKey));

    int resultCrypt = mbedtls_aes_crypt_ecb(&aes, MBEDTLS_AES_DECRYPT, realImage, realImage);
    debugLog("resultCrypt: " + String(resultCrypt));

    //debugLog("Finished decrypting: " + String(millis()));

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
    Serial.flush();

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

    //debugLog("Finished decrypting: " + String(millis()));

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
            showTextDialog("Key is incorrect");
            return;
        }
        int itemsInDir = fsItemsInDir("/vault/") - 1; // - conf
        debugLog("items in vault: " + String(itemsInDir));
        if (itemsInDir <= 0)
        {
            generalSwitch(textDialog);
            showTextDialog("Vault is empty?");
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
            entryMenu buttons[itemsInDir];

            File root = LittleFS.open("/vault/");
            File file = root.openNextFile();
            int counter = 0;
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

            initMenu(buttons, itemsInDir, "Vault", 1);
            generalSwitch(generalMenuPlace);
        }
        else
        {
            showVaultImage(foundMenuItemName);
            generalSwitch(imagePlace);
            lastMenuSelected = "";
        }
    }
}

void loopVault()
{
}

void exitVault()
{
    debugLog("Exiting vault");
    if (currentPlace == FIRST_PLACE)
    {
        debugLog("Cleaning key");
        key = -1;
        key = 0;
    }
}

void showVaultImage(String file)
{
    if (key != 0)
    {
        debugLog("Showing vault image");
        //debugLog("Starting decrypting: " + String(millis()));

        bufSize vaultItem = fsGetBlob(file, "/vault/");
        debugLog("VaultItem size: " + String(vaultItem.size));

        unsigned char *realImage = new unsigned char[vaultItem.size];

        size_t written = 0;

        debugLog("Before base64 encoding");
        Serial.flush();

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

        //debugLog("Finished decrypting: " + String(millis()));

        mbedtls_aes_free(&aes);

        ImageDef newImage = {realImage, 200, 200};
        writeImageN(0, 0, &newImage);
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
        Serial.flush();

        debugLog("Original base64 image:");
        for (size_t i = 0; i < testImgVaultImg_len; i++)
        {
            Serial.print(String(testImgVaultImg[i], HEX));
            Serial.print(" ");
        }
        Serial.println("");
        Serial.flush();
        */
    }
    else
    {
        debugLog("key is not available");
    }
}

void hexStringToByteArray(const char *hexString, unsigned char *byteArray, size_t byteArraySize)
{
    for (size_t i = 0; i < byteArraySize; i++)
    {
        byteArray[i] = strtoul(hexString + 2 * i, NULL, 16);
    }
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
    debugLog("Starting decrypting: " + String(millis()));
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
    debugLog("End of decrypting: " + String(millis()));
    debugLog("Result is: " + String(result)); // 0 means good

    ImageDef newImage = {realImage, 200, 200};
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
