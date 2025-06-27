#include "../include/mylib.h"
#include <iostream>

vector<uint8_t> encrypt(const vector<uint8_t>& bytes, int key) {
        if (bytes.empty()) {
            cerr << "Encrypted data is NULL" << endl;
            return {};
        }
        key &= 0xFF;
        vector<uint8_t> encrypted(bytes.size());
        for (size_t i = 0; i < bytes.size(); ++i) {
            encrypted[i] = bytes[i] ^ static_cast<uint8_t>(key);
        }
        return encrypted;
    }

vector<uint8_t> decrypt(const vector<uint8_t>& bytes, int key) {
        if (bytes.empty()) {
            cerr << "Decrypted Text is NULL" << endl;
            return {};
        }
        return encrypt(bytes, key);
}