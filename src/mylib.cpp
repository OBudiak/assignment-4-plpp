#include "mylib.h"
#include <iostream>
#include <string.h>

const char* encrypt(const char* rawText, int key) {
    if (rawText == NULL) {
        std::cerr << "Encrypted Text is NULL" << std::endl;
        return NULL;
    }
    int len = strlen(rawText);
    char* encrypted = new char[len + 1];
    key = key % 26;
    if (key < 0) key += 26;
    for (int i = 0; i < len; i++) {
        if ('A' > rawText[i] || (rawText[i] > 'Z' && 'a' > rawText[i]) || rawText[i] > 'z') {
            encrypted[i] = rawText[i];
            continue;
        }
        if (rawText[i] >= 'a') {
            encrypted[i] = static_cast<char>((rawText[i] - 'a' + key) % 26 + 'a');
            // std::cout << '-' << encrypted[i] << std::endl;
        } else {
            encrypted[i] = static_cast<char>((rawText[i] - 'A' + key) % 26 + 'A');
            // std::cout << '-' << encrypted[i] << std::endl;
        }
    }
    encrypted[len] = '\0';
    // std::cout << encrypted << std::endl;
    return encrypted;
}

const char* decrypt(const char* rawText, int key) {
    if (rawText == NULL) {
        std::cerr << "Decrypted Text is NULL" << std::endl;
        return NULL;
    }
    return encrypt(rawText, -key);
}
