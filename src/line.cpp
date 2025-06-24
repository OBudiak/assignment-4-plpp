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

vector<uint8_t> TextLine::serialize() const {
     vector<uint8_t> bytes;
     uint32_t len = text.size();
     uint8_t* pLen = reinterpret_cast<uint8_t*>(&len);
     bytes.insert(bytes.end(), pLen, pLen + sizeof(len));
     bytes.insert(bytes.end(), text.begin(), text.end());
     return bytes;
 }

string deserialize(const vector<uint8_t>& data, size_t offset) {
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
ostream& TextLine::print(ostream& os) const {
     return os << text;
 }


ContactLine::ContactLine() {
     firstName = NULL, lastName = NULL, email = NULL;
}
ContactLine::ContactLine(string newText) {
     firstName = NULL, lastName = NULL, email = NULL;
 }
uint8_t ContactLine::getCode() const{
    return code;
}

ostream& ContactLine::print(ostream& os) const {
     return os << "Contacts: "
               << firstName << ' '
               << lastName  << " - <"
               << email     << '>';
 }


ChecklistLine::ChecklistLine() {
    text = NULL;
     checked = false;
}
uint8_t ChecklistLine::getCode() const {
    return code;
}


ostream& ChecklistLine::print(ostream& os) const {
     return os << '[' << (checked ? '0' : ' ') << "] "
               << text;
 }

