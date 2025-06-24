#ifndef EDITINGTEXT_H
#define EDITINGTEXT_H

#include "functionality.h"
#include <iostream>
#include <cstdio>
#include <cstdlib>

class EditingText {
public:
    EditingText(Functionality& func);
    void addText();
    void addNewLine();
    void insertTextOnPosition();
    void insertWithReplacement();

    void encryptTextInFile();
    void decryptTextInFile();

    void deleteText();
    void copyText();
    void pasteText();
    void cutText();

private:
    Functionality& functionality;
};

#endif