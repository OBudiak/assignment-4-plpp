//
// Created by Fujitsu-H770 on 23.06.2025.
//

#include "line.h"

 TextLine::TextLine(){
     text = NULL;
 }
TextLine::TextLine(string newText){
     text = newText;
 }

std::ostream& TextLine::print(std::ostream& os) const override {
     return os << text;
 }


ContactLine::ContactLine() {
     firstName = NULL, lastName = NULL, email = NULL;
}
ContactLine::ContactLine(string newText) {
     firstName = NULL, lastName = NULL, email = NULL;
 }
std::ostream& ContactLine::print(std::ostream& os) const override {
     return os << "Contacts: "
               << firstName << ' '
               << lastName  << " - <"
               << email     << '>';
 }


ChecklistLine::ChecklistLine() {
    text = NULL;
     checked = false;
}

std::ostream& ChecklistLine::print(std::ostream& os) const override {
     return os << '[' << (checked ? '0' : ' ') << "] "
               << text;
 }

