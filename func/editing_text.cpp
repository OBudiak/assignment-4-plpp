#include "../include/editing_text.h"

using namespace std;

EditingText::EditingText(Functionality* func) : functionality(func) {}

void EditingText::setFunctionality(Functionality* func) {
    functionality = func;
}

void EditingText::addText() {
    cout << "  -Add text-  " << endl;
    cout << "Write a text: ";
    string newText = functionality->readline();
    if (newText.empty()) return;
    functionality->addText(newText);
}

void EditingText::addNewLine() {
    cout << "  -Add new line-  " << endl;
    cout << "Line type (t)ext, (c)ontact, check(l)ist - ";
    string lineType = functionality->readline();
    while (lineType.length() > 1) {
        cout << "Incorrect line type format!" << endl;
        cout << "Line type (t)ext, (c)ontact, check(l)ist - ";
        lineType = functionality->readline();
    }
    functionality->addNewLine(lineType.front());
}

void EditingText::checkTask() {
    cout << "  -Check task-  " << endl;
    cout << "Line index - ";
    string lineType = functionality->readline();
    int intLineIndex = 0;
    sscanf(lineType.c_str(), "%d", &intLineIndex);
    cout << "Set task checked(1) or unchecked(0) - ";
    string isChecked = functionality->readline();
    int intIsChecked = 0;
    sscanf(isChecked.c_str(), "%d", &intIsChecked);
    functionality->setCheckStatus(intLineIndex, intIsChecked);
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

    functionality->saveInFile(fileName, ki);
}

void EditingText::decryptTextFromFile() {
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

    functionality->loadFromFile(fileName, ki);
}

void EditingText::searchInText() {
    cout << "  -Search in text-  " << endl;
    if (functionality->isEmpty()) {
        cout << "Cannot search in empty text" << endl << endl;
        return;
    }
    cout << "Enter text: ";
    string phrase = functionality->readline();
    if (phrase.empty() || phrase[0] == '\0') {
        cout << "Empty search string" << endl << endl;
        phrase.clear();
        return;
    }
    functionality->searchInText(phrase);
}



void EditingText::insertTextOnPosition() {
    cout << "  -Insert text by coordinate-  " << endl;
    cout << "Write a position (x y) - ";
    string tmpInput = functionality->readline();
    if (tmpInput.empty()) return;
    int x = 0, y = 0;
    if (sscanf(tmpInput.c_str(), "%d %d", &x, &y) != 2) {
        cout << "Uncorrect coordinates" << endl;
        tmpInput.clear();
        return;
    }
    tmpInput.clear();

    cout << "Write a text: ";
    string newText = functionality->readline();
    if (newText.empty()) return;
    functionality->relocateMemory(newText, x, y);
}

void EditingText::deleteText() {
    cout << "  -Delete text-  " << endl;
    cout << "Choose line, index and number of symbols: ";
    string tmpInput = functionality->readline();
    if (tmpInput.empty()) return;
    int line = 0, index = 0, count = 0;
    if (sscanf(tmpInput.c_str(), "%d %d %d", &line, &index, &count) != 3) {
        cout << "Uncorrect parameters" << endl;
        tmpInput.clear();
        return;
    }
    tmpInput.clear();
    functionality->deleteText(line, index, count);
}

void EditingText::insertWithReplacement() {
    cout << "  -Insert with replacement-  " << endl;
    cout << "Choose line and index: ";
    string tmpInput = functionality->readline();
    if (tmpInput.empty()) return;
    int line = 0, index = 0;
    if (sscanf(tmpInput.c_str(), "%d %d", &line, &index) != 2) {
        cout << "Uncorrect parameters" << endl;
        tmpInput.clear();
        return;
    }
    tmpInput.clear();

    cout << "Write text: ";
    string newText = functionality->readline();
    if (newText.empty()) return;
    functionality->deleteText(line, index, newText.length());
    functionality->relocateMemory(newText, index, line);
}

void EditingText::copyText() {
    cout << "  -Copy text-  " << endl;
    cout << "Choose line and index and number of symbols: ";
    string tmpInput = functionality->readline();
    if (tmpInput.empty()) return;
    int line = 0, index = 0, count = 0;
    if (sscanf(tmpInput.c_str(), "%d %d %d", &line, &index, &count) != 3) {
        cout << "Uncorrect parameters" << endl;
        tmpInput.clear();
        return;
    }
    tmpInput.clear();
    functionality->copyText(line, index, count);
}

void EditingText::cutText() {
    cout << "  -Cut text-  " << endl;
    cout << "Choose line and index and number of symbols: ";
    string tmpInput = functionality->readline();
    if (tmpInput.empty()) return;
    int line = 0, index = 0, count = 0;
    if (sscanf(tmpInput.c_str(), "%d %d %d", &line, &index, &count) != 3) {
        cout << "Uncorrect parameters" << endl;
        tmpInput.clear();
        return;
    }
    tmpInput.clear();
    functionality->cutText(line, index, count);
}

void EditingText::pasteText() {
    cout << "  -Paste text-  " << endl;
    cout << "Choose line and index: ";
    string tmpInput = functionality->readline();
    if (tmpInput.empty()) return;
    int line = 0, index = 0;
    if (sscanf(tmpInput.c_str(), "%d %d", &line, &index) != 2) {
        cout << "Uncorrect parameters" << endl;
        tmpInput.clear();
        return;
    }
    tmpInput.clear();
    functionality->pasteText(line, index);
}