#ifndef FUNCTIONALITY_H
#define FUNCTIONALITY_H

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <memory>

#include "../src/line.h"
#include "mylib.h"

using namespace std;

class Functionality {
public:
    // typedef vector<uint8_t> (*enc_t)(const vector<uint8_t>&, int);
    // typedef vector<uint8_t> (*dec_t)(const vector<uint8_t>&, int);

    vector<uint8_t> (*encrypt)(vector<uint8_t> const&, int);

    vector<uint8_t> (*decrypt)(vector<uint8_t> const&, int);

    Functionality();
    ~Functionality();

    void setCheckStatus(int index, int isChecked);

    static string readline();
    bool isEmpty() const;
    int powerF(int power);
    void relocateMemory(const string& newText, int x, int y);
    void saveInFile(const string&, int);
    void loadFromFile(const string&, int);
    void searchInText(string);
    void showText();

    void addText(string &);
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

    size_t getGlobalIndex(int line, int index) const;

    static const int MAX_HISTORY = 5;
    std::vector<unique_ptr<Line>> history;
    std::vector<pair<int, bool>> changes;
    int historySize;
    int historyPos;
    int redoStartingPos;

    void saveCur(unique_ptr<Line> line, int index, bool needDelete);
};

#endif
