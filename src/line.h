
//
// Created by Fujitsu-H770 on 23.06.2025.
//

#ifndef LINE_H
#define LINE_H
#include <cstdint>
#include <vector>
#include <iostream>
#include <sstream>
#include <memory>

using namespace std;

class Line {
public:
    virtual ~Line() = default;
    virtual vector<uint8_t> serialize() const = 0;
    virtual string deserialize(const vector<uint8_t>& data, size_t offset) = 0;
    virtual ostream& print(ostream& os) const = 0;
    virtual string getString() const;
    virtual uint8_t getCode() const;
};


class TextLine : public Line {
public:
    string text;
    const uint8_t code = 1;

    uint8_t getCode() const override;;

    TextLine();
    TextLine(string newText);
    vector<uint8_t> serialize() const override;
    string deserialize(const vector<uint8_t>& data, size_t offset) override;

    static unique_ptr<Line> createFrom(const vector<uint8_t>& data, size_t& offset);
    string getString() const override;
    ostream& print(ostream& os) const override;

    // ... реалізація serialize()/deserialize() для рядка
};

class ContactLine : public Line {
public:
    char* firstName, lastName, email;
    const uint8_t code = 2;

    uint8_t getCode() const override;
    ContactLine();
    ContactLine(string newText);
    vector<uint8_t> serialize() const override;
    string deserialize(const vector<uint8_t>& data, size_t offset) override;

    static unique_ptr<Line> createFrom(const vector<uint8_t>& data, size_t& offset);
    string getString() const override;
    std::ostream& print(std::ostream& os) const override;
    // у serialize():
    //   [len1][bytes of firstName][len2][bytes of lastName][len3][bytes of email]
    // у deserialize(): читає ці три блоки
};

class ChecklistLine : public Line {
public:
    char* text;
    bool checked;
    const uint8_t code = 3;

    uint8_t getCode() const override;
    ChecklistLine();
    ChecklistLine(string newText);
    vector<uint8_t> serialize() const override;
    string deserialize(const vector<uint8_t>& data, size_t offset) override;

    static unique_ptr<Line> createFrom(const vector<uint8_t>& data, size_t& offset);
    string getString() const override;
    std::ostream& print(std::ostream& os) const override;
    // serialize():
    //   [lenText][bytes of text][1 byte: 0 or 1]
    // deserialize(): відповідно
};

inline std::ostream& operator<<(std::ostream& os, const Line& line) {
    return line.print(os);
}


#endif //LINE_H
