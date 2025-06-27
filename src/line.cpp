//
// Created by Fujitsu-H770 on 23.06.2025.
//

#include "line.h"

TextLine::TextLine(){
     text = nullptr;
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
     vector<uint8_t> bytes;
     uint32_t len = text.size();
     uint8_t* pLen = reinterpret_cast<uint8_t*>(&len);
     bytes.insert(bytes.end(), pLen, pLen + sizeof(len));
     bytes.insert(bytes.end(), text.begin(), text.end());
     return bytes;
 }

string TextLine::deserialize(const vector<uint8_t>& data, size_t offset) {
     uint32_t len;
     memcpy(&len, data.data() + offset, sizeof(len));
     offset += sizeof(len);
     string s(data.begin() + offset, data.begin() + offset + len);
     offset += len;
     return s;
 }

unique_ptr<Line> TextLine::createFrom(const vector<uint8_t>& data, size_t& offset) {
     auto ptr = make_unique<TextLine>();
     ptr->deserialize(data, offset);
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


ContactLine::ContactLine() {
     firstName = nullptr, lastName = nullptr, email = nullptr;
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
    vector<uint8_t> bytes;
    string text = getString();
    uint32_t len = text.size();
    uint8_t* pLen = reinterpret_cast<uint8_t*>(&len);
    bytes.insert(bytes.end(), pLen, pLen + sizeof(len));
    bytes.insert(bytes.end(), text.begin(), text.end());
    return bytes;
}

string ContactLine::deserialize(const vector<uint8_t>& data, size_t offset) {
    uint32_t len;
    memcpy(&len, data.data() + offset, sizeof(len));
    offset += sizeof(len);
    string s(data.begin() + offset, data.begin() + offset + len);
    offset += len;
    return s;
}

unique_ptr<Line> ContactLine::createFrom(const vector<uint8_t>& data, size_t& offset) {
    auto ptr = make_unique<ContactLine>();
    ptr->deserialize(data, offset);
    return ptr;
}



ChecklistLine::ChecklistLine() {
    text = nullptr;
     checked = false;
}

ChecklistLine::ChecklistLine(string newText) {
    text = newText;
    checked = false;
}

void ChecklistLine::setText(string& t)  {
    text = text.empty() ? t : text + t;;
    checked = false;
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
    vector<uint8_t> bytes;
    string text = getString();
    uint32_t len = text.size();
    uint8_t* pLen = reinterpret_cast<uint8_t*>(&len);
    bytes.insert(bytes.end(), pLen, pLen + sizeof(len));
    bytes.insert(bytes.end(), text.begin(), text.end());
    return bytes;
}

string ChecklistLine::deserialize(const vector<uint8_t>& data, size_t offset) {
    uint32_t len;
    memcpy(&len, data.data() + offset, sizeof(len));
    offset += sizeof(len);
    string s(data.begin() + offset, data.begin() + offset + len);
    offset += len;
    return s;
}

unique_ptr<Line> ChecklistLine::createFrom(const vector<uint8_t>& data, size_t& offset) {
    auto ptr = make_unique<ChecklistLine>();
    string full = ptr->deserialize(data, offset);

    if (full.size() >= 4 && full[0] == '[' && full[2] == ']') {
        ptr->checked = (full[1] == '1');
        ptr->text = full.substr(4);
    } else {
        ptr->checked = false;
        ptr->text = full;
    }
    return ptr;
}

