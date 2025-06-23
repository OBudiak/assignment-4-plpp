
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
    virtual std::ostream& print(std::ostream& os) const = 0;
};


class TextLine : public Line {
public:
    string text;
    TextLine();
    TextLine(string newText);
    virtual std::ostream& print(std::ostream& os) const = 0;
    // ... реалізація serialize()/deserialize() для рядка
};

class ContactLine : public Line {
public:
    char* firstName, lastName, email;
    ContactLine();
    ContactLine(string newText);
    virtual std::ostream& print(std::ostream& os) const = 0;
    // у serialize():
    //   [len1][bytes of firstName][len2][bytes of lastName][len3][bytes of email]
    // у deserialize(): читає ці три блоки
};

class ChecklistLine : public Line {
public:
    char* text;
    bool checked;

    ChecklistLine();
    ChecklistLine(string newText);
    virtual std::ostream& print(std::ostream& os) const = 0;
    // serialize():
    //   [lenText][bytes of text][1 byte: 0 or 1]
    // deserialize(): відповідно
};

inline std::ostream& operator<<(std::ostream& os, const Line& line) {
    return line.print(os);
}


#endif //LINE_H
