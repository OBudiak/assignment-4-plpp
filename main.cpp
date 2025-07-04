#include <windows.h>
#include <iostream>
#include <cstring>
#include "include/functionality.h"
#include "include/editing_text.h"
#include "include/pages.h"
// #include "include/mylib.h"

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
    cout << "Check a task - 15" << endl;
    cout << "Add/Switch To/Delete page - 18, 19, 20" << endl;
    cout << "Instruction - i" << endl;
    cout << "Exit - 0" << endl << endl;
}

int chooseCommand(int command, EditingText& editor) {
    Functionality& func = *Pages::getPage();
    switch (command) {
        case 1:
            editor.addText();
            break;
        case 2:
            editor.addNewLine();
            break;
        case 3:
            editor.encryptTextInFile();
            break;
        case 4:
            editor.decryptTextFromFile();
            break;
        case 5:
            func.showText();
            break;
        case 6:
            editor.insertTextOnPosition();
            break;
        case 7:
            editor.searchInText();
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
        case 15:
            editor.checkTask();
            break;
        case 18:
            Pages::addPage();
            editor.setFunctionality(Pages::getPage());
            break;
        case 19:
            Pages::switchToPage();
            editor.setFunctionality(Pages::getPage());
            break;
        case 20:
            Pages::removePage();
            editor.setFunctionality(Pages::getPage());
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
    cout << "Welcome to text Editor V0.1" << endl;
    instruction();

    Pages::setUp();
    Functionality* current = Pages::getPage();
    EditingText editor(current);

    while (true) {
        cout << "Write a command - ";
        int count;
        string input = Functionality::readline();
        (void)sscanf(input.c_str(), "%d", &count);
        if (input.empty()) break;
        if (input[0] == '\0') {
            input.clear();
            continue;
        }

        // char command = input[0];
        (void)chooseCommand(count, editor);
        input.clear();

        if (count == 0) break;
        count = -1;
    }
    return 0;
}
