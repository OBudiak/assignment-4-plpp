#include "../include/functionality.h"

#include <algorithm>
#include <chrono>
#include <sstream>

using namespace std;

Functionality::Functionality() {
    clipboard = nullptr;
    historySize = 0;
    historyPos = -1;
    redoStartingPos = -1;
    for (int i = 0; i < MAX_HISTORY; i++) {
        history[i] = nullptr;
    }
}

Functionality::~Functionality() {
    clipboard.clear();
    for (int i = 0; i < historySize; i++) {
        history[i].clear();
    }
    for (int i = 0; i < lines.size(); i++) {
        delete lines[i];
    }
}

string Functionality::readline() {
    size_t size = 64;
    size_t len = 0;
    char* buf = (char*)malloc(size);
    if (!buf) return NULL;

    int c;
    while ((c = fgetc(stdin)) != EOF && c != '\n') {
        if (len + 1 >= size) {
            size += 64;
            char* newbuf = (char*)realloc(buf, size);
            if (!newbuf) {
                free(buf);
                return NULL;
            }
            buf = newbuf;
            free(newbuf);
        }
        buf[len++] = c;
    }
    buf[len] = '\0';
    return buf;
}

int Functionality::powerF(int power) {
    return (int)pow(10, power);
}

size_t Functionality::getGlobalIndex(int line, int index) {
    if (lines.empty()) return 0;
    auto textPtr = dynamic_cast<TextLine*>(lines[line].get());
    if (!textPtr) {
        cout << "Cannot change not text typed line with text" << endl;
        return -1;
    }
    if (index > textPtr->text.size()) {
        cout << "Index out of bounds" << endl;
        return -1;
    }
    return index;
}

void Functionality::addText(const string& newText) {
    if (!lines.empty()) {
        auto& last = lines.back();
        auto textPtr = dynamic_cast<TextLine*>(last.get());
        if (textPtr) {
            textPtr->text += newText;
            return;
        }
        textPtr->text = newText;
        /*string buf = nullptr;
        int i = 0;
        while (i < textPtr->text.length()) {
            buf[i] = textPtr->text[i];
            i++;
        }
        while (i < newText.length() + textPtr->text.length()) {
            buf[i] = newText[i];
            i++;
        }
        textPtr->text = buf;
        */
    }
    lines.push_back(std::make_unique<TextLine>(newText));
}

void Functionality::addNewLine(char lineType) {
    switch (lineType) {
        case 't':
            lines.push_back(make_unique<TextLine>(nullptr));
            break;
        case 'c':
            lines.push_back(make_unique<ContactLine>(nullptr));
            break;
        case 'l':
            lines.push_back(make_unique<ChecklistLine>(nullptr));
            break;
        default:
            cout << "Invalid line type" << endl;
            break;
    }
}


void Functionality::relocateMemory(const string& newText, int x, int y) {
    TextLine* textPtr = nullptr;
    if (y >= 0 && static_cast<size_t>(y) < lines.size()) {
        textPtr = dynamic_cast<TextLine*>(lines[y].get());
    }
    if (!textPtr) {
        auto newLine = make_unique<TextLine>(nullptr);
        textPtr = newLine.get();
        if (y >= 0 && static_cast<size_t>(y) <= lines.size()) {
            lines.insert(lines.begin() + y, move(newLine));
        } else {
            lines.push_back(move(newLine));
        }
    }
    if (x == -1 && y == -1) {
        textPtr->text += newText;
    } else {
        size_t pos = (x >= 0 && static_cast<size_t>(x) <= textPtr->text.size())
                         ? static_cast<size_t>(x)
                         : textPtr->text.size();
        textPtr->text.insert(pos, newText);
    }
    saveCur();
}


void Functionality::saveInFile() {
    cout << "-Save in file-" << endl;
    cout << "Enter file name: ";
    string fileName;
    getline(cin, fileName);
    if (fileName.empty()) return;
    FILE* f = fopen(fileName.c_str(), "wb");
    if (!f) return;

    for (auto& line : lines) {
        auto payload = line->serialize();
        uint8_t typeId = line->getCode();
        uint32_t len = payload.size();

        fwrite(&typeId, 1, 1, f);
        fwrite(&len, sizeof(len), 1, f);
        fwrite(payload.data(), 1, payload.size(), f);
    }

    fclose(f);
}


void Functionality::loadFromFile() {
    cout << "-Load from file-" << endl;
    cout << "Enter file name: ";
    string fileName;
    getline(cin, fileName);
    if (fileName.empty()) return;
    FILE* f = fopen(fileName.c_str(), "rb");
    if (!f) return;

    lines.clear();
    size_t offset = 0;
    while (true) {
        uint8_t  typeId;
        if (fread(&typeId, 1, 1, f) != 1) break;
        uint32_t len;
        fread(&len, sizeof(len), 1, f);

        vector<uint8_t> buffer(len);
        fread(buffer.data(), 1, len, f);

        unique_ptr<Line> obj;
        switch (typeId) {
            case 1: obj = TextLine::createFrom(buffer, offset); break;
            case 2: obj = ContactLine::createFrom(buffer, offset); break;
            case 3: obj = ChecklistLine::createFrom(buffer, offset); break;
            default: continue;
        }
        offset += len;
        lines.push_back(move(obj));
    }

    fclose(f);
}

void Functionality::searchText() {
    cout << "  -Search in text-  " << endl;
    cout << "Enter text: ";
    char* phrase = readline();
    if (!phrase || phrase[0] == '\0') {
        cout << "Empty search string" << endl << endl;
        free(phrase);
        return;
    }
    if (lines.empty()) {
        cout << "Text is empty" << endl << endl;
        free(phrase);
        return;
    }

    int line = 0, col = 0, found = 0;
    for (auto& line : lines) {
        char* t = line;
        size_t len_text = strlen(t);
        size_t len_phrase = strlen(phrase);

        for (size_t i = 0; i < len_text; i++) {
            if (t[i] == '\n') {
                line++;
                col = 0;
                continue;
            }
            if (i + len_phrase <= len_text && strncmp(&t[i], phrase, len_phrase) == 0) {
                cout << "\"" << phrase << "\" - " << line << " " << col << endl;
                found = 1;
            }
            col++;
        }
    }

    if (!found) {
        cout << "\"" << phrase << "\" was not found" << endl;
    }
    cout << endl;
    free(phrase);
}

void Functionality::showText() {
    cout << "  -Show text-  " << endl;
    if (lines.empty()) {
        cout << "[Текст порожній]" << endl << endl;
    } else {
        for (auto& line : lines) {
            cout << line << endl;
        }
    }
}

void Functionality::deleteText(int line, int index, int count) {
    if (lines.empty() || count <= 0) return;
    size_t idx = getGlobalIndex(line, index);
    if (idx == -1) return;
    auto tl = dynamic_cast<TextLine*>(lines[line].get());
    size_t oldLen = tl->text.length();
    if (index + count > oldLen) {
        count = static_cast<int>(oldLen) - index;
    }
    tl->text.erase(index, count);
    saveCur();
}

void Functionality::saveCur() {
    if (historySize < MAX_HISTORY) {
        historySize++;
        historyPos = historySize - 1;
    } else {
        free(history[0]);
        for (int i = 1; i < MAX_HISTORY; i++) {
            history[i - 1] = history[i];
        }
        historyPos = MAX_HISTORY - 1;
    }

    if (lines) {
        history[historyPos] = strdup(lines);
    } else {
        history[historyPos] = strdup("");
    }

    redoStartingPos = -1;
}

void Functionality::undoText() {
    if (historySize == 0 || historyPos <= 0) {
        cout << "Nothing to undo" << endl;
        return;
    }
    int targetPos = historyPos - 1;
    free(lines);
    lines = strdup(history[targetPos]);
    historyPos = targetPos;
    redoStartingPos = historyPos + 1;
}

void Functionality::redoText() {
    if (redoStartingPos < 0 || redoStartingPos >= historySize) {
        cout << "Nothing to redo" << endl;
        return;
    }
    free(lines);
    lines = strdup(history[redoStartingPos]);
    historyPos = redoStartingPos;
    redoStartingPos = historyPos + 1 < historySize ? historyPos + 1 : -1;
}