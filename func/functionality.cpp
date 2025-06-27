#include "../include/functionality.h"

#include <algorithm>
#include <chrono>
#include <memory>
#include <sstream>
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#define _RPCNDR_H_
#include <windows.h>

using namespace std;

Functionality::Functionality() {
    // clipboard = "";
    historySize = 0;
    historyPos = -1;
    redoStartingPos = -1;
    for (auto & i : history) {
        i = nullptr;
    }

    HINSTANCE h = LoadLibrary(TEXT("libcaesar.dll"));
    if (!h) {
        cerr << "Cannot load DLL" << endl;
        cerr << "Reload an app after fixing issue" << endl;
    }
    else {
        typedef vector<uint8_t> (*enc_t)( vector<uint8_t> const&, int);
        typedef vector<uint8_t> (*dec_t)( vector<uint8_t> const&, int);

        encrypt = (enc_t)GetProcAddress(h, "encrypt");
        decrypt = (dec_t)GetProcAddress(h, "decrypt");
        if (!encrypt || !decrypt) {
            cerr << "Function not found in DLL" << endl;
            FreeLibrary(h);
        }
    }
}

Functionality::~Functionality() {
    clipboard.clear();
    history.clear();
    changes.clear();
    lines.clear();
}

bool Functionality::isEmpty() const {
    return lines.empty() || lines.size() == 0 || ((lines.size() < 2) && lines.front() == nullptr);
}


string Functionality::readline() {
    string textLine;
    getline(cin, textLine);
    /*size_t size = 64;
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
    buf[len] = '\0';*/
    return textLine;
}

int Functionality::powerF(int power) {
    return (int)pow(10, power);
}

size_t Functionality::getGlobalIndex(int line, int index) const {
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

void Functionality::setCheckStatus(int index, int isChecked) {
    if (index >= lines.size()) {
        cout << "Index out of bounds" << endl;
        return;
    }
    auto textPtr = dynamic_cast<ChecklistLine*>(lines[index].get());
    if (!textPtr) {
        cout << "Cannot check this type of line" << endl;
        return;
    }
    if (isChecked == 1) {
        textPtr->checked = true;
    }
    else if (isChecked == 0) {
        textPtr->checked = false;
    }
    else {
        cout << "Incorrect status (expect: 1 or 0)" << endl;
    }
}

void Functionality::addText(string &newText) {
    if (isEmpty()) {
        lines.push_back(make_unique<TextLine>());
    }
    auto& last = lines.back();
    last.get()->setText(newText);

    // int lineType = last.get()->getCode();
    // if (!lines.empty()) {
    //     auto& last = lines.back();
    //     auto textPtr = dynamic_cast<TextLine*>(last.get());
    //     if (textPtr) {
    //         textPtr->text += newText;
    //         return;
    //     }
    //     textPtr->text = newText;}
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
    // lines.push_back(std::make_unique<TextLine>(newText));
}

void Functionality::addNewLine(char lineType) {
    switch (lineType) {
        case 't':
            lines.push_back(make_unique<TextLine>());
            break;
        case 'c':
            lines.push_back(make_unique<ContactLine>());
            break;
        case 'l':
            lines.push_back(make_unique<ChecklistLine>());
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
        cout << "Cannot insert in not Text Line" << endl;
        return;
        // Creating new line if current is not TextLine.
        /*
        auto newLine = make_unique<TextLine>(nullptr);
        textPtr = newLine.get();
        if (y >= 0 && static_cast<size_t>(y) <= lines.size()) {
            lines.insert(lines.begin() + y, move(newLine));
        } else {
            lines.push_back(move(newLine));
        }
        */
    }
    if (x == -1 && y == -1) {
        textPtr->text += newText;
    } else {
        size_t pos = (x >= 0 && static_cast<size_t>(x) <= textPtr->text.size())
                         ? static_cast<size_t>(x)
                         : textPtr->text.size();
        textPtr->text.insert(pos, newText);
    }
    saveCur(make_unique<TextLine>(*textPtr), y, false);
}


void Functionality::saveInFile(const string& fileName, int key) {
    /*cout << "-Save in file-" << endl;
    cout << "Enter file name: ";
    string fileName;
    getline(cin, fileName);*/
    if (fileName.empty()) return;
    FILE* f = fopen(fileName.c_str(), "wb");
    if (!f) return;

    for (auto& line : lines) {
        auto payload = encrypt(line->serialize(), key);
        uint8_t typeId = line->getCode();
        uint32_t len = payload.size();

        fwrite(&typeId, 1, 1, f);
        fwrite(&len, sizeof(len), 1, f);
        fwrite(payload.data(), 1, payload.size(), f);
    }

    fclose(f);
}


void Functionality::loadFromFile(const string& fileName, int key) {
    // cout << "-Load from file-" << endl;
    // cout << "Enter file name: ";
    // string fileName;
    // getline(cin, fileName);
    // if (fileName.empty()) return;
    FILE* f = fopen(fileName.c_str(), "rb");
    if (!f) return;

    cout << "Reading from file " << fileName << endl;

    lines.clear();
    size_t offset = 0;
    while (true) {
        uint8_t  typeId;
        if (fread(&typeId, 1, 1, f) != 1) break;

        uint32_t len;
        if (fread(&len, sizeof len, 1, f) != 1) break;

        vector<uint8_t> buffer(len);
        fread(buffer.data(), 1, len, f);
        vector<uint8_t> decoded = decrypt(buffer, key);

        size_t pos = 0;
        unique_ptr<Line> obj;
        switch (typeId) {
            case 1: obj = TextLine::createFrom(decoded, pos); break;
            case 2: obj = ContactLine::createFrom(decoded, pos); break;
            case 3: obj = ChecklistLine::createFrom(decoded, pos); break;
            default: continue;
        }
        lines.push_back(std::move(obj));
    }
    fclose(f);
}

void Functionality::searchInText(string phrase) {
    int line = 0, col = 0, found = 0;
    for (auto& textLine : lines) {
        string t = textLine->getString();
        size_t len_text = t.size();
        size_t len_phrase = phrase.size();

        for (size_t i = 0; i < len_text; i++) {
            if (i + len_phrase <= len_text && strncmp(&t[i], phrase.c_str(), len_phrase) == 0) {
                cout << "\"" << phrase << "\" - " << line << " " << col << endl;
                found = 1;
            }
            col++;
        }
        line++;
        col = 0;
    }

    if (!found) {
        cout << "\"" << phrase << "\" was not found" << endl;
    }
    cout << endl;
    phrase.clear();
}

void Functionality::showText() {
    cout << "  -Show text-  " << endl;
    if (isEmpty()) {
        cout << "[Text is empty]" << endl << endl;
    } else {
        for (auto& line : lines) {
            cout << line->getString() << endl;
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
    saveCur(make_unique<TextLine>(*tl), line, true);
}

void Functionality::copyText(int line, int index, int count) {
    if (lines.empty() || count <= 0) return;
    string phrase = lines[line].get()->getString();
    size_t oldLen = phrase.size();
    if (index >= oldLen) return;
    count = min(static_cast<size_t>(count), oldLen - index);
    clipboard = phrase.substr(index, count);
}

void Functionality::cutText(int line, int index, int count) {
    if (isEmpty() || count <= 0) return;
    clipboard.clear();
    auto& last = lines[line];
    auto textPtr = dynamic_cast<TextLine*>(last.get());
    if (!textPtr) {
        cout << "Can't cut this line" << endl;
        return;
    }
    string phrase = lines[line].get()->getString();
    size_t oldLen = phrase.size();
    if (index >= oldLen) return;
    if ((size_t)count > oldLen - index) {
        count = (int)(oldLen - index);
    }
    clipboard = phrase.substr(index, count);
    textPtr->text.erase(index, count);
    saveCur(move(lines[line]), line, false);
}

void Functionality::pasteText(int line, int index) {
    if (clipboard.empty()) return;
    string copyText = clipboard;
    relocateMemory(copyText, index, line);
}

void Functionality::saveCur(unique_ptr<Line> line, int index, bool needDelete) {

    if (historySize < MAX_HISTORY) {
        historySize++;
        historyPos = historySize - 1;
    } else {
        history.erase(history.begin());
        changes.erase(changes.begin());
        historyPos = MAX_HISTORY - 1;
    }
    history.push_back(std::move(line));
    changes.emplace_back(index, needDelete);
    historySize = static_cast<int>(history.size());
    historyPos = historySize - 1;
    /*if (!isEmpty()) {
        history[historyPos] = strdup(lines);
    } else {
        cout << "Nothing to save" << endl;
    }*/

    redoStartingPos = -1;
}

void Functionality::undoText() {
    if (historySize == 0 || historyPos <= 0) {
        cout << "Nothing to undo" << endl;
        return;
    }
    int targetPos = historyPos - 1;;
    auto lineIt = lines.begin() + changes[targetPos].first;
        lines.insert(lineIt, move(history[targetPos]));
    historyPos = targetPos;
    redoStartingPos = historyPos + 1;
}

void Functionality::redoText() {
    if (redoStartingPos < 0 || redoStartingPos >= historySize) {
        cout << "Nothing to redo" << endl;
        return;
    }
    if (changes[redoStartingPos].second) {
        lines.erase(lines.begin() + changes[redoStartingPos].first);
    }
    else {
        auto lineIt = lines.begin() + changes[redoStartingPos].first;
        lines.insert(lineIt, move(history[redoStartingPos]));
        // history.erase(history.begin() + redoStartingPos);
        // changes.erase(changes.begin() + redoStartingPos);

    }
    historyPos = redoStartingPos;
    redoStartingPos = historyPos + 1 < historySize ? historyPos + 1 : -1;
}