#ifndef FUNCTIONALITY_H
#define FUNCTIONALITY_H

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <memory>

#include "src/line.h"
using namespace std;

class Functionality {
public:
    Functionality();
    ~Functionality();
    char* readline();
    int powerF(int power);
    void relocateMemory(char* newText, int x, int y);
    void saveInFile();
    void loadFromFile();
    void searchText();
    void showText();
    void addText(char*);

    void deleteText(int line, int index, int count);
    // void insertWithReplacement(int line, int index, const char* newText);
    // void copyText(int line, int index, int count);
    // void cutText(int line, int index, int count);
    // void pasteText(int line, int index);

    void undoText();
    void redoText();

private:
    std::vector<unique_ptr<Line>> lines;
    char* clipboard;

    size_t getGlobalIndex(int line, int index);

    static const int MAX_HISTORY = 5;
    char* history[MAX_HISTORY];
    int historySize;
    int historyPos;
    int redoStartingPos;

    void saveCur();
};

#endif
