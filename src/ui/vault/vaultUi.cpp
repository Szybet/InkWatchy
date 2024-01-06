#include "../../defines/config.h"
#if VAULT
#include "vaultUi.h"

#include "../../defines/vault.h"
#include "Cipher.h"
#include "mbedtls/base64.h"

RTC_DATA_ATTR int key = String(VAULT_PASSWORD).toInt();

void initVault()
{
}

void loopVault()
{
}

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

#define KEY_LENGTH 16
void showVaultImage(int index)
{
    // if (key != 0)
    if (true == true)
    {
        debugLog("Starting decrypting: " + String(millis()));

        unsigned char realImage[testImgVaultImg_len] = {0};

        /*
        for (size_t i = 0; i < VAULT_IMAGE_LIST_SIZE; i++)
        {
            realImage[i] = vaultList[index].bitmap[i];
        }
        */
        // base64::decode(realImage, vaultList[index].bitmap, 5024);
        size_t written;
        int whoa = mbedtls_base64_decode(realImage, testImgVaultImg_len, &written, testImgVaultImg, testImgVaultImg_len);

        debugLog("Written bytes: " + String(written));
        debugLog("base64 result: " + String(whoa));

        mbedtls_aes_context aes;

        const unsigned char key[16] = {0x6B, 0x65, 0x79, 0x6B, 0x65, 0x79, 0x6B, 0x65, 0x79, 0x79, 0x6B, 0x65, 0x79, 0x6B, 0x65, 0x79};
        unsigned char iv[16] = {0x6B, 0x65, 0x79, 0x6B, 0x65, 0x79, 0x6B, 0x65, 0x79, 0x79, 0x6B, 0x65, 0x79, 0x6B, 0x65, 0x79};
        // debugLog("Key: " + key));

        mbedtls_aes_init(&aes);
        int resultKey = mbedtls_aes_setkey_dec(&aes, key, 128);

        mbedtls_aes_crypt_cbc(&aes, MBEDTLS_AES_DECRYPT, written, iv, realImage, realImage);
        //mbedtls_aes_crypt_ecb(&aes, MBEDTLS_AES_DECRYPT, realImage, realImage);

        /*
        // Iterate over each block
        for (size_t i = 0; i < num_blocks; i++)
        {
            int result = mbedtls_aes_crypt_ecb(&aes, MBEDTLS_AES_DECRYPT, realImage + i * 16, realImage + i * 16);

            if (result != 0)
            {
                debugLog("Result key is: " + String(result)); // 0 means good
                return;
            }
        }
        */

        debugLog("Decrypted image in base64:");
        for (size_t i = 0; i < written; i++)
        {
            Serial.print(String(realImage[i], HEX));
            Serial.print(" ");
        }
        Serial.println("");
        Serial.flush();

        mbedtls_aes_free(&aes);

        ImageDef newImage = {realImage, 200, 200};
        writeImageN(0, 0, newImage);
        disUp(true);

        debugLog("Original base64 image:");
        for (size_t i = 0; i < testImgVaultImg_len; i++)
        {
            Serial.print(String(testImgVaultImg[i], HEX));
            Serial.print(" ");
        }
        Serial.println("");
        Serial.flush();
    }
    else
    {
        debugLog("key is not available");
    }
}

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
