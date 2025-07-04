
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
    virtual vector<uint8_t> serialize() const = 0;
    virtual void setText( string&) = 0;
    virtual string deserialize(const vector<uint8_t>& data, size_t& offset) = 0;
    virtual ostream& print(ostream& os) const = 0;
    virtual string getString() const = 0;
    virtual uint8_t getCode() const = 0;
    virtual unique_ptr<Line> clone() const = 0;
    virtual ~Line();
};


class TextLine : public Line {
public:
    string text;
    const uint8_t code = 1;

    uint8_t getCode() const override;
    void setText( string& t)  override;
    unique_ptr<Line> clone() const override;

    TextLine();
    TextLine(string newText);
    ~TextLine() override;

    vector<uint8_t> serialize() const override;
    string deserialize(const vector<uint8_t>& data, size_t& offset) override;

    static unique_ptr<Line> createFrom(const vector<uint8_t>& data, size_t& offset);
    string getString() const override;
    ostream& print(ostream& os) const override;
};

class ContactLine : public Line {
public:
    string firstName, lastName, email;
    const uint8_t code = 2;

    uint8_t getCode() const override;
    void setText( string &t)  override;
    unique_ptr<Line> clone() const override;

    ContactLine();
    ContactLine(string newText);
    ~ContactLine() override;

    vector<uint8_t> serialize() const override;
    string deserialize(const vector<uint8_t>& data, size_t& offset) override;

    static unique_ptr<Line> createFrom(const vector<uint8_t>& data, size_t& offset);
    string getString() const override;
    ostream& print(ostream& os) const override;
};

class ChecklistLine : public Line {
public:
    string text;
    bool checked;
    const uint8_t code = 3;

    ChecklistLine();
    ChecklistLine(string);
    ~ChecklistLine() override;

    uint8_t getCode() const override;
    void setText( string &t)  override;
    unique_ptr<Line> clone() const override;

    vector<uint8_t> serialize() const override;
    string deserialize(const vector<uint8_t>& data, size_t& offset) override;

    static unique_ptr<Line> createFrom(const vector<uint8_t>& data, size_t& offset);
    string getString() const override;
    ostream& print(ostream& os) const override;
};

inline ostream& operator<<(ostream& os, const Line& line) {
    return line.print(os);
}


#endif //LINE_H
