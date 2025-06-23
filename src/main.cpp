#include <cstdio>
#include <iostream>
#include <windows.h>
#include "mylib.h"


int main() {

    std::string input;
    int key;
    std::cout << "Enter text: ";
    std::getline(std::cin, input);
    std::cout << "Enter key: ";
    std::cin >> key;
    std::cin.ignore();

    char choice;
    std::cout << "(e)ncrypt or (d)ecrypt? - ";
    std::cin >> choice;

    const char* resultCstr = nullptr;
    if (choice == 'e') {
        resultCstr = encrypt(input.c_str(), key);
    } else {
        resultCstr = decrypt(input.c_str(), key);
    }

    std::cout << "Result: " << resultCstr << "\n";

    // FreeLibrary(hLib);
    return 0;

    // char* text = encrypt("My name is Zak", -44);
    // char* result = decrypt(text, -44);
    // for (int i = 0; i <= 255; i++) printf("%d - %c\n", i, i);
}
