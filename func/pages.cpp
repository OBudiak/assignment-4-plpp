//
// Created by Fujitsu-H770 on 04.07.2025.
//

#include "../include/pages.h"

string Pages::currentPageName;
map<string, unique_ptr<Functionality>> Pages::pagesDic;

void Pages::setUp() {
    pagesDic["basePage"] = make_unique<Functionality>();
    currentPageName = "basePage";
}

void Pages::addPage() {
    string pageName;
    cout << "  -Adding page-  " << endl;
    cout << "Write a page name - ";
    getline(cin, pageName);
    setPageName(pageName);
}

void Pages::setPageName(const string& pageName) {
    if (pagesDic.find(pageName) != pagesDic.end()) {
        cout << "Page already exists";
        return;
    }
    pagesDic[pageName] = make_unique<Functionality>();
    currentPageName = pageName;
}

void Pages::switchToPage() {
    string pageName;
    cout << "  -Switch to page-  " << endl;
    cout << "Write a page name - ";
    getline(cin, pageName);
    if (pagesDic.find(pageName) == pagesDic.end()) {
        cout << "Page doesn't exist";
        return;
    }
    currentPageName = pageName;
}

void Pages::removePage() {
    string pageName;
    cout << "  -Remove page-  " << endl;
    cout << "Write a page name - ";
    getline(cin, pageName);
    if (pagesDic.find(pageName) == pagesDic.end()) {
        cout << "Page doesn't exist" << endl;
        return;
    }
    pagesDic.erase(pageName);
    if (currentPageName == pageName) {
        switchToPage();
    }
}

string Pages::getPageName() {
    return currentPageName;
}

Functionality *Pages::getPage() {
    return pagesDic[currentPageName].get();
}
