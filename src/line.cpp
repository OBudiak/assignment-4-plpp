//
// Created by Fujitsu-H770 on 23.06.2025.
//

#include "line.h"

Line::~Line() { }

TextLine::TextLine() : text(){}

TextLine::~TextLine(){
    text.clear();
}

TextLine::TextLine(string newText){
     text = newText;
 }
uint8_t TextLine::getCode() const {
    return code;
}

void TextLine::setText( string& t)  {
    text = text.empty() ? t : text + t;
}

vector<uint8_t> TextLine::serialize() const {
    vector<uint8_t> out;
    uint32_t len = static_cast<uint32_t>(text.size());
    out.insert(out.end(), reinterpret_cast<uint8_t*>(&len),
               reinterpret_cast<uint8_t*>(&len) + sizeof len);
    out.insert(out.end(), text.begin(), text.end());
    return out;
}

string TextLine::deserialize(const vector<uint8_t>& data, size_t& offset)
{
    auto need = [&](size_t bytes, const char* what) {
        if (offset + bytes > data.size())
            throw runtime_error(string("Corrupted record (no ") + what + ')');
    };

    need(sizeof(uint32_t), "length field");
    uint32_t len;
    memcpy(&len, &data[offset], sizeof len);
    offset += sizeof len;

    need(len, "payload");
    string s(reinterpret_cast<const char*>(&data[offset]), len);
    offset += len;
    return s;
}

unique_ptr<Line> TextLine::createFrom(const vector<uint8_t>& data, size_t&) {
    size_t p = 0;
    auto ptr   = make_unique<TextLine>();
    ptr->text  = ptr->deserialize(data, p);
    return ptr;
}

string TextLine::getString() const {
    ostringstream oss;
    print(oss);
    return oss.str();
}

ostream& TextLine::print(ostream& os) const {
    return os << text;
}


ContactLine::ContactLine() : firstName(), lastName(), email(){}

ContactLine::~ContactLine() {
    firstName.clear(), lastName.clear(), email.clear();
}
ContactLine::ContactLine(string newText) {
    istringstream iss(newText);
    iss >> firstName >> lastName >> email;
}

uint8_t ContactLine::getCode() const{
    return code;
}

void ContactLine::setText( string& t)  {
    istringstream iss(t);
    iss >> firstName >> lastName >> email;
}

ostream& ContactLine::print(ostream& os) const {
return os << "Contacts: "
       << firstName << ' '
       << lastName  << " - <"
       << email     << '>';
}

string ContactLine::getString() const {
    ostringstream oss;
    print(oss);
    return oss.str();
}


vector<uint8_t> ContactLine::serialize() const {
    auto dump = [](vector<uint8_t>& v, const string& s) {
        uint16_t len = static_cast<uint16_t>(s.size());
        v.insert(v.end(), reinterpret_cast<uint8_t*>(&len),
                 reinterpret_cast<uint8_t*>(&len) + sizeof len);
        v.insert(v.end(), s.begin(), s.end());
    };

    vector<uint8_t> out;
    dump(out, firstName);
    dump(out, lastName);
    dump(out, email);
    return out;
}

string ContactLine::deserialize(const vector<uint8_t>& data, size_t& offset) {
    auto read = [&](string& tgt) {
        uint16_t len;
        memcpy(&len, &data[offset], sizeof len);
        offset += sizeof len;
        tgt.assign(reinterpret_cast<const char*>(&data[offset]), len);
        offset += len;
    };
    string dummy;
    read(firstName);
    read(lastName);
    read(email);
    return dummy;
}

unique_ptr<Line> ContactLine::createFrom(const vector<uint8_t>& data, size_t&) {
    size_t p = 0;
    auto ptr = make_unique<ContactLine>();
    ptr->deserialize(data, p);
    return ptr;
}

ChecklistLine::ChecklistLine() : text(), checked(false){}

ChecklistLine::ChecklistLine(string newText) {
    text = newText;
    checked = false;
}

ChecklistLine::~ChecklistLine() {
    text.clear();
}

void ChecklistLine::setText(string& t)  {
    text = text.empty() ? t : text + t;
}

uint8_t ChecklistLine::getCode() const {
    return code;
}


ostream& ChecklistLine::print(ostream& os) const {
    return os << '[' << (checked ? '0' : ' ') << "] " << text;
}

string ChecklistLine::getString() const {
    ostringstream oss;
    print(oss);
    return oss.str();
}

vector<uint8_t> ChecklistLine::serialize() const {
    vector<uint8_t> out;
    out.push_back(static_cast<uint8_t>(checked));
    auto len = static_cast<uint32_t>(text.size());
    out.insert(out.end(), reinterpret_cast<uint8_t*>(&len),
               reinterpret_cast<uint8_t*>(&len) + sizeof len);
    out.insert(out.end(), text.begin(), text.end());
    return out;
}

string ChecklistLine::deserialize(const vector<uint8_t>& data, size_t& offset) {
    checked = data[offset++] ? true : false;
    uint32_t len;
    memcpy(&len, &data[offset], sizeof len);
    offset += sizeof len;
    text.assign(reinterpret_cast<const char*>(&data[offset]), len);
    offset += len;
    return {};
}

unique_ptr<Line> ChecklistLine::createFrom(const vector<uint8_t>& data, size_t&) {
    size_t p = 0;
    auto ptr = make_unique<ChecklistLine>();
    ptr->deserialize(data, p);
    return ptr;
}

