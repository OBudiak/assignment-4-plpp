//
// Created by Fujitsu-H770 on 04.07.2025.
//

#ifndef PAGES_H
#define PAGES_H

#include <map>

#include "functionality.h"
#include "line.h"

class Pages {
    public:
    static void setUp();
    static void addPage();
    static void switchToPage();
    static void removePage();
    static string getPageName();
    static Functionality *getPage();
    private:
    static void setPageName(const string& pageName);
    static map<string, unique_ptr<Functionality>> pagesDic;
    static string currentPageName;
};

#endif //PAGES_H
