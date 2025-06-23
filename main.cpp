#include <windows.h>
#include <iostream>
#include <cstring>
#include "func/functionality.cpp"
#include "func/editing_text.cpp"
#include "mylib.h"

using namespace std;

void instruction() {
    cout << endl << "Instruction:" << endl;
    cout << "Add text/new line - 1, 2" << endl;
    cout << "Save in/load from file - 3, 4" << endl;
    cout << "Show text - 5" << endl;
    cout << "Insert text by coordinates - 6" << endl;
    cout << "Search in text - 7" << endl;
    cout << "Delete text - 8" << endl;
    cout << "Undo/Redo text - 9, 10" << endl;
    cout << "Cut/Paste/Copy text - 11, 12, 13" << endl;
    cout << "Insert with replacement - 14" << endl;
    cout << "Instruction - i" << endl;
    cout << "Exit - 0" << endl << endl;
}

int chooseCommand(int command, EditingText& editor, Functionality& func) {
    switch (command) {
        case 1:
            editor.addText();
            break;
        case 2:
            editor.addNewLine();
            break;
        case 3:
            func.saveInFile();
            break;
        case 4:
            func.loadFromFile();
            break;
        case 5:
            func.showText();
            break;
        case 6:
            editor.insertTextOnPosition();
            break;
        case 7:
            func.searchText();
            break;
        case 8:
            editor.deleteText();
            break;
        case 9:
            func.undoText();
            break;
        case 10:
            func.redoText();
            break;
        case 11:
            editor.cutText();
            break;
        case 12:
            editor.pasteText();
            break;
        case 13:
            editor.copyText();
            break;
        case 14:
            editor.insertWithReplacement();
            break;
        case 'i':
            instruction();
            break;
        case 0:
            return 0;
        default:
            cout << "Invalid command, please try again" << endl;
            return -1;
    }
    return command;
}

int main() {
    HINSTANCE h = LoadLibrary(TEXT("caesar.dll"));
    if (!h) {
        std::cerr << "Cannot load DLL\n";
        return 1;
    }

    typedef char* (*enc_t)(char*, int);
    typedef char* (*dec_t)(char*, int);

    enc_t enc = (enc_t)GetProcAddress(h, "encrypt");
    dec_t dec = (dec_t)GetProcAddress(h, "decrypt");
    if (!enc || !dec) {
        std::cerr << "Function not found in DLL\n";
        FreeLibrary(h);
        return 1;
    }
    char text[] = "Hello, World!";
    char* e = enc(text, 3);
    std::cout << "Encrypted: " << e << "\n";

    char* d = dec(e, 3);
    std::cout << "Decrypted: " << d << "\n";

    delete[] e;
    delete[] d;
    FreeLibrary(h);

    cout << "Welcome to text Editor V0.1" << endl;
    instruction();

    Functionality func;
    EditingText editor(func);

    while (true) {
        cout << "Write a command - ";
        int count;
        char* input = func.readline();
        (void)sscanf(input, "%d", &count);
        if (!input) break;
        if (input[0] == '\0') {
            free(input);
            continue;
        }

        // char command = input[0];
        (void)chooseCommand(count, editor, func);
        free(input);

        if (count == 0) break;
        count = -1;
    }
    return 0;
}
