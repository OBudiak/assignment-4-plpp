#include "../include/editing_text.h"

using namespace std;

EditingText::EditingText(Functionality& func) : functionality(func) {}

void EditingText::addText() {
    cout << "  -Add text-  " << endl;
    cout << "Write a text: ";
    char* newText = functionality.readline();
    if (!newText) return;
    functionality.addText(newText, -1, -1);
}

void EditingText::addNewLine() {
    cout << "  -Add new line-  " << endl;
    char* newText = (char*)malloc(2);
    if (!newText) return;
    newText[0] = '\n';
    newText[1] = '\0';
    functionality.relocateMemory(newText, -1, -1);
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
    char* tmpInput = functionality.readline();
    if (!tmpInput) return;
    int line = 0, index = 0, count = 0;
    if (sscanf(tmpInput, "%d %d %d", &line, &index, &count) != 3) {
        cout << "Uncorrect parameters" << endl;
        free(tmpInput);
        return;
    }
    free(tmpInput);
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