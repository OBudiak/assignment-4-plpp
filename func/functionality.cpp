#include "../include/functionality.h"

using namespace std;

Functionality::Functionality() {
    clipboard = NULL;
    historySize = 0;
    historyPos = -1;
    redoStartingPos = -1;
    for (int i = 0; i < MAX_HISTORY; i++) {
        history[i] = NULL;
    }
}

Functionality::~Functionality() {
    free(clipboard);
    for (int i = 0; i < historySize; i++) {
        free(history[i]);
    }
    for (int i = 0; i < lines.size(); i++) {
        delete lines[i];
    }
}

char* Functionality::readline() {
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
    if (!lines) return 0;
    size_t idx = 0, curX = 0, curY = 0;
    size_t oldLen = 0;
    while (lines[oldLen]) oldLen++;

    while (idx < oldLen && curY < (size_t)line) {
        if (lines[idx] == '\n') {
            curY++;
            curX = 0;
        } else {
            curX++;
        }
        idx++;
    }
    curX = 0;
    while (idx < oldLen && curX < (size_t)index) {
        if (lines[idx] == '\n') break;
        curX++;
        idx++;
    }
    if (idx > oldLen) idx = oldLen;
    return idx;
}

void Functionality::addText(char* newText) {
    if (!lines.empty()) {
        auto& last = lines.back();
        auto textPtr = dynamic_cast<TextLine*>(last.get());
        if (textPtr) {
            textPtr->text = newText;
            return;
        }
        char* buf = nullptr;
        int i = 0;
        while (i < strlen(textPtr->text)) {
            buf[i] = textPtr->text[i];
            i++;
        }
        while (i < strlen(newText) + strlen(textPtr->text)) {
            buf[i] = newText[i];
            i++;
        }
        textPtr->text = (char*)realloc(buf, i + 1);
    }
    lines.push_back(std::make_unique<TextLine>(newText));
}


void Functionality::relocateMemory(char* newText, int x, int y) {
    size_t oldLen = 0;

    size_t addLen = 0;
    while (newText[addLen]) addLen++;


    char* tmp = (char*)realloc(lines, oldLen + addLen + 1);
    if (!tmp) {
        free(newText);
        return;
    }
    lines = tmp;

    size_t idx = 0, curX = 0, curY = 0;
    if (x >= 0 && y >= 0) {
        while (idx < oldLen && (curY < (size_t)y || curX < (size_t)x)) {
            if (lines[idx] == '\n') {
                curY++;
                curX = 0;
            } else {
                curX++;
            }
            idx++;
        }
        if (idx > oldLen) idx = oldLen;
    } else {
        idx = oldLen;
    }

    char* src = lines + idx;
    char* dest = lines + idx + addLen;
    size_t moveLen = oldLen - idx + 1;

    if (dest > src) {
        size_t i = moveLen;
        while (i--) {
            dest[i] = src[i];
        }
    } else {
        for (size_t i = 0; i < moveLen; i++) {
            dest[i] = src[i];
        }
    }

    for (size_t i = 0; i < addLen; i++) {
        lines[idx + i] = newText[i];
    }
    lines[oldLen + addLen] = '\0';
    free(newText);
    saveCur();
}

void Functionality::saveInFile() {
    cout << "  -Save in file-  " << endl;
    cout << "Enter file name: ";
    char* fileName = readline();
    if (!fileName) return;
    FILE* file = fopen(fileName, "w");
    if (file != NULL) {
        fputs(lines, file);
        fclose(file);
    }
    free(fileName);
}

void Functionality::loadFromFile() {
    cout << "  -Load from file-  " << endl;
    cout << "Enter file name: ";
    char* fileName = readline();
    if (!fileName) return;

    FILE* file = fopen(fileName, "r");
    free(fileName);
    if (!file) {
        cout << "Error opening file" << endl;
        return;
    }

    size_t size = 128, len = 0;
    char* buf = (char*)malloc(size);
    if (!buf) {
        fclose(file);
        return;
    }

    int c;
    while ((c = fgetc(file)) != EOF) {
        if (len + 1 >= size) {
            size *= 2;
            char* tmpBuf = (char*)realloc(buf, size);
            if (!tmpBuf) {
                free(buf);
                fclose(file);
                return;
            }
            buf = tmpBuf;
        }
        buf[len++] = (char)c;
    }
    buf[len] = '\0';
    fclose(file);
    free(lines);
    lines = buf;
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
    if (!lines) {
        cout << "Text is empty" << endl << endl;
        free(phrase);
        return;
    }

    char* t = lines;
    int line = 0, col = 0, found = 0;
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

    if (!found) {
        cout << "\"" << phrase << "\" was not found" << endl;
    }
    cout << endl;
    free(phrase);
}

void Functionality::showText() {
    cout << "  -Show text-  " << endl;
    if (lines == NULL) {
        cout << "[Текст порожній]" << endl << endl;
    } else {
        cout << lines << endl;
    }
}

void Functionality::deleteText(int line, int index, int count) {
    if (!lines || count <= 0) return;
    size_t oldLen = strlen(lines);
    size_t idx = getGlobalIndex(line, index);
    if (idx >= oldLen) return;

    if ((size_t)count > oldLen - idx) {
        count = (int)(oldLen - idx);
    }

    size_t newLen = oldLen - count;
    memmove(lines + idx, lines + idx + count, oldLen - idx - count + 1);

    char* tmp = (char*)realloc(lines, newLen + 1);
    if (tmp) {
        lines = tmp;
    }
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