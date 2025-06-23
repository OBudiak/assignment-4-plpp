
//
// Created by Fujitsu-H770 on 23.06.2025.
//

#ifndef LINE_H
#define LINE_H
#include <cstdint>
#include <vector>
#include <iostream>
#include <cstring>

using namespace std;

class Line {
public:
    virtual ~Line() = default;
    virtual vector<uint8_t> serialize() const = 0;
    virtual size_t deserialize(const vector<uint8_t>& data, size_t offset) = 0;
};


class TextLine : public Line {
public:
    char* text;
    TextLine();
    TextLine(char* newText);
    TextLine(string newText);
    // ... реалізація serialize()/deserialize() для рядка
};

class ContactLine : public Line {
public:
    char* firstName, lastName, email;
    ContactLine();
    ContactLine(char* newText);
    ContactLine(string newText);
    // у serialize():
    //   [len1][bytes of firstName][len2][bytes of lastName][len3][bytes of email]
    // у deserialize(): читає ці три блоки
};

class ChecklistLine : public Line {
public:
    char* text;
    bool checked;

    ChecklistLine();
    ChecklistLine(char* newText);
    ChecklistLine(string newText);
    // serialize():
    //   [lenText][bytes of text][1 byte: 0 or 1]
    // deserialize(): відповідно
};


#endif //LINE_H
