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
    string readline();
    bool isEmpty() const;
    int powerF(int power);
    void relocateMemory(const string& newText, int x, int y);
    void saveInFile(string, int);
    void loadFromFile(string, int);
    void searchInText(string);
    void showText();

    void addText(const string&);
    void addNewLine(char lineType /*(t - text, c - contact, l - checklist)*/);

    void deleteText(int line, int index, int count);
    void copyText(int line, int index, int count);
    void cutText(int line, int index, int count);
    void pasteText(int line, int index);

    void undoText();
    void redoText();

private:
    std::vector<unique_ptr<Line>> lines;
    string clipboard;

    size_t getGlobalIndex(int line, int index);

    static const int MAX_HISTORY = 5;
    string history[MAX_HISTORY];
    int historySize;
    int historyPos;
    int redoStartingPos;

    void saveCur();
};

#endif
