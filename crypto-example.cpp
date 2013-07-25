#include <stdio.h>
#include <iostream>
#include <string>

#include "Crypto.h"

//#define PRINT_KEYS

using namespace std;

int main() {
    Crypto crypto;

    #ifdef PRINT_KEYS
        crypto.writeKeyToFile(stdout, KEY_SERVER_PRI);
        crypto.writeKeyToFile(stdout, KEY_SERVER_PUB);
        crypto.writeKeyToFile(stdout, KEY_CLIENT_PUB);

        unsigned char *aesKey;
        size_t aesLength = crypto.getAESKey(&aesKey);
        printf("AES key: ");
        for(int i=0; i<aesLength; i++) {
            printf("%d", aesKey[i]);
        }
        printf("\n");
    #endif

    string msg;
    unsigned char *encMsg = NULL;
    char *decMsg          = NULL;
    int encMsgLen;
    int decMsgLen;

    unsigned char *ek;
    unsigned char *iv;
    size_t ekl;
    size_t ivl;

    while(1) {
        printf("Message to RSA encrypt: ");
        fflush(stdout);
        getline(cin, msg);

        if(!msg.compare("exit")) break;

        if((encMsgLen = crypto.rsaEncrypt((const unsigned char*)msg.c_str(), msg.size(), &encMsg, &ek, &ekl, &iv, &ivl)) == -1) {
            fprintf(stderr, "Encryption failed\n");
            return 1;
        }
        printf("%d bytes encrypted\n", encMsgLen);

        if((decMsgLen = crypto.rsaDecrypt(encMsg, (size_t)encMsgLen, ek, ekl, iv, ivl, (unsigned char**)&decMsg)) == -1) {
            fprintf(stderr, "Decryption failed\n");
            return 1;
        }
        printf("%d bytes decrypted\n", decMsgLen);
        printf("Decrypted message: %s\n", decMsg);

        free(encMsg);
        free(decMsg);
        free(ek);
        free(iv);
        encMsg = NULL;
        decMsg = NULL;
        ek     = NULL;
        iv     = NULL;

        printf("Message to AES encrypt: ");
        fflush(stdout);
        getline(cin, msg);

         if((encMsgLen = crypto.aesEncrypt((const unsigned char*)msg.c_str(), msg.size(), &encMsg)) == -1) {
            fprintf(stderr, "Encryption failed\n");
            return 1;
        }
        printf("%d bytes encrypted\n", encMsgLen);

        if((decMsgLen = crypto.aesDecrypt(encMsg, (size_t)encMsgLen, &decMsg)) == -1) {
            fprintf(stderr, "Decryption failed\n");
            return 1;
        }
        printf("%d bytes decrypted\n", decMsgLen);
        printf("Decrypted message: %s\n", decMsg);

        free(encMsg);
        free(decMsg);
        encMsg = NULL;
        decMsg = NULL;
    }

    return 0;
}