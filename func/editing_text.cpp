#include "../include/editing_text.h"

using namespace std;

EditingText::EditingText(Functionality& func) : functionality(func) {}

void EditingText::addText() {
    cout << "  -Add text-  " << endl;
    cout << "Write a text: ";
    string newText = functionality.readline();
    if (newText.empty()) return;
    functionality.addText(newText);
}

void EditingText::addNewLine() {
    cout << "  -Add new line-  " << endl;
    cout << "Line type (t)ext, (c)ontact, check(l)ist - ";
    string lineType = functionality.readline();
    while (lineType.length() > 1) {
        cout << "Incorrect line type format!" << endl;
        cout << "Line type (t)ext, (c)ontact, check(l)ist - ";
        lineType = functionality.readline();
    }
    functionality.addNewLine(lineType.front());
}

void EditingText::encryptTextInFile() {
    cout << "  -Write text in file-  " << endl;

    cout << "Enter file name: ";
    string fileName;
    getline(cin, fileName);
    if (fileName.empty()) {
        cout << "File name cannot be empty." << endl;
        return;
    }

    cout << "Write a key: ";
    string key;
    getline(cin, key);
    if (key.empty()) {
        cout << "Key cannot be empty." << endl;
        return;
    }

    int ki = 0;
    try {
        ki = stoi(key);
    } catch (...) {
        cout << "Invalid key format." << endl;
        return;
    }

    functionality.saveInFile(fileName, ki);
}

void EditingText::decryptTextInFile() {
    cout << "  -Load text from file-  " << endl;

    cout << "Enter file name: ";
    string fileName;
    getline(cin, fileName);
    if (fileName.empty()) {
        cout << "File name cannot be empty." << endl;
        return;
    }

    cout << "Write a key: ";
    string key;
    getline(cin, key);
    if (key.empty()) {
        cout << "Key cannot be empty." << endl;
        return;
    }

    int ki = 0;
    try {
        ki = stoi(key);
    } catch (...) {
        cout << "Invalid key format." << endl;
        return;
    }

    functionality.saveInFile(fileName, ki);
}


void EditingText::insertTextOnPosition() {
    cout << "  -Insert text by coordinate-  " << endl;
    cout << "Write a position (x y) - ";
    char* tmpInput = functionality.readline();
    if (!tmpInput) return;
    int x = 0, y = 0;
    if (sscanf(tmpInput, "%d %d", &x, &y) != 2) {
        cout << "Uncorrect coordinates" << endl;
        free(tmpInput);
        return;
    }
    free(tmpInput);

    cout << "Write a text: ";
    char* newText = functionality.readline();
    if (!newText) return;
    functionality.relocateMemory(newText, x, y);
}

void EditingText::deleteText() {
    cout << "  -Delete text-  " << endl;
    cout << "Choose line, index and number of symbols: ";
    string tmpInput = functionality.readline();
    if (tmpInput.empty()) return;
    int line = 0, index = 0, count = 0;
    if (sscanf(tmpInput.c_str(), "%d %d %d", &line, &index, &count) != 3) {
        cout << "Uncorrect parameters" << endl;
        tmpInput.clear();
        return;
    }
    tmpInput.clear();
    functionality.deleteText(line, index, count);
}

void EditingText::insertWithReplacement() {
    cout << "  -Insert with replacement-  " << endl;
    cout << "Choose line and index: ";
    char* tmpInput = functionality.readline();
    if (!tmpInput) return;
    int line = 0, index = 0;
    if (sscanf(tmpInput, "%d %d", &line, &index) != 2) {
        cout << "Uncorrect parameters" << endl;
        free(tmpInput);
        return;
    }
    free(tmpInput);

    cout << "Write text: ";
    char* newText = functionality.readline();
    if (!newText) return;
    functionality.insertWithReplacement(line, index, newText);
}

void EditingText::copyText() {
    cout << "  -Copy text-  " << endl;
    cout << "Choose line and index and number of symbols: ";
    char* tmpInput = functionality.readline();
    if (!tmpInput) return;
    int line = 0, index = 0, count = 0;
    if (sscanf(tmpInput, "%d %d %d", &line, &index, &count) != 3) {
        cout << "Uncorrect parameters" << endl;
        free(tmpInput);
        return;
    }
    free(tmpInput);
    functionality.copyText(line, index, count);
}

void EditingText::cutText() {
    cout << "  -Cut text-  " << endl;
    cout << "Choose line and index and number of symbols: ";
    char* tmpInput = functionality.readline();
    if (!tmpInput) return;
    int line = 0, index = 0, count = 0;
    if (sscanf(tmpInput, "%d %d %d", &line, &index, &count) != 3) {
        cout << "Uncorrect parameters" << endl;
        free(tmpInput);
        return;
    }
    free(tmpInput);
    functionality.cutText(line, index, count);
}

void EditingText::pasteText() {
    cout << "  -Paste text-  " << endl;
    cout << "Choose line and index: ";
    char* tmpInput = functionality.readline();
    if (!tmpInput) return;
    int line = 0, index = 0;
    if (sscanf(tmpInput, "%d %d", &line, &index) != 2) {
        cout << "Uncorrect parameters" << endl;
        free(tmpInput);
        return;
    }
    free(tmpInput);
    functionality.pasteText(line, index);
}