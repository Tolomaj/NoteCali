#pragma once

#include <vector>
#include <string>
#include "debugConsole.h"
#include "ObjectDefinitions.h"

using namespace std;

#define SPACE_SIMBOLS_TABLE_LENGHT 8
const wchar spaceSimbolsTable[VARIABLE_FORBIDENT_SIMBOLS_LENGHT] = { '[' , ']' , '{' , '}' , '=' , '&' , '@' , ' ' };   // simbols that is not character

class LineRegister {

    void registerLine(wstring* iline, bool isCompleted = true);

public:
	vector<mline> lines;

    void printLines();

    void procesInput(wstring* str);

};


void LineRegister::registerLine(wstring* iline, bool isCompleted) {
    mline separatedLine;
    separatedLine.originLine = *iline;
    separatedLine.isEnded = isCompleted;
    if ((iline->size() > 0 && iline->at(0) != L';') || iline->size() == 0) { // neobsahuje ; na prvním a není tudíš command
        if (settings.useLineModifiers) {
            size_t modificator = iline->find(L";");
            size_t modifcorection = iline->find(L"(");


            bool modificationIsvalid = (modificator != string::npos) && ((modifcorection == string::npos) || (modificator < modifcorection));//aby se nepoèítaly ; v závorkách

            if (modificationIsvalid) {
                separatedLine.lineModifier = iline->substr(0, modificator);
                separatedLine.line = iline->substr(modificator + 1, iline->length());
                lines.push_back(separatedLine);
                return;
            }
        }


        separatedLine.line = *iline;
        lines.push_back(separatedLine);
        return;
    }

    size_t space = iline->find_first_of(L" +-*/%^<>=!&|");

    if (space == string::npos) {
        separatedLine.command = iline->substr(1, iline->length());
        lines.push_back(separatedLine);
        return;
    };


    separatedLine.command = iline->substr(1, space - 1);
    separatedLine.line = iline->substr(space + (iline->at(space) == L' '), iline->size() - 1);
    lines.push_back(separatedLine);
}

void LineRegister::printLines() {
    debugLOG("PrintingSeparatedLines:");
    for (size_t i = 0; i < lines.size(); i++) {
        debugLOG(lines.at(i).command + L"," + lines.at(i).line + L";");
    };
};

void LineRegister::procesInput(wstring* str) { //used from stackowerflow https://stackoverflow.com/questions/13172158/c-split-string-by-line
    lines.clear();

    size_t pos = 0;
    size_t prev = 0;
    wstring a = L"";
    while ((pos = str->find(L"\n", prev)) != string::npos) {
        long posN = pos - prev - 1;
        if (posN < 0) {
            mline l;
            l.isComandDone = true;
            lines.push_back(l);
            prev = pos + 1;
            continue;
        }
        a = str->substr(prev, pos - prev - (str->at(posN) == L'\r'));
        registerLine(&a);
        prev = pos + 1;
    }

    if (prev < str->length()) {
        a = str->substr(prev);
        registerLine(&a, false);
    }
#if DEBUG
    printLines();
#endif
};