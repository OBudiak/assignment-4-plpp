//
// Created by Fujitsu-H770 on 23.06.2025.
//

#include "line.h"

 TextLine::TextLine(){
     text = NULL;
 }
TextLine::TextLine(char* newText){
     text = newText;
 }

ContactLine::ContactLine() {
     firstName = NULL, lastName = NULL, email = NULL;
}
ContactLine::ContactLine(char* newText) {
     firstName = NULL, lastName = NULL, email = NULL;
 }

ChecklistLine::ChecklistLine() {
    text = NULL;
     checked = false;
}

