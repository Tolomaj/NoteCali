#pragma once

#include <vector>
#include <string>

using namespace std;

#define SPACE_SIMBOLS_TABLE_LENGHT 8
const wchar spaceSimbolsTable[VARIABLE_FORBIDENT_SIMBOLS_LENGHT] = { '[' , ']' , '{' , '}' , '=' , '&' , '@' , ' ' };   // simbols that is not character

class LineRegister {
    void registerLine(wstring* iline, bool isCompleted = true) {
        mline separatedLine;
        separatedLine.isEnded = isCompleted;
        if ((iline->size() > 0 && iline->at(0) != L';') || iline->size() == 0) { // neobsahuje ; na prvním a není tudíš command

            size_t modificator = iline->find(L";");
            // má modifikátopr ?
            if (modificator != string::npos ) {
                separatedLine.lineModifier = iline->substr(0, modificator);
                separatedLine.line = iline->substr(modificator + 1, iline->length());
                lines.push_back(separatedLine);
                return;
            }

            separatedLine.line = *iline;
            lines.push_back(separatedLine);
            return;
        }

        size_t space = iline->find_first_of(L" +-*/%^<>=!&|");

        if (space == string::npos) {
            separatedLine.command = iline->substr(1,iline->length());
            lines.push_back(separatedLine);
            return;
        };


        separatedLine.command = iline->substr(1, space - 1);
        separatedLine.line = iline->substr(space + (iline->at(space) == L' '), iline->size() - 1);
        lines.push_back(separatedLine);
    }
public:
	vector<mline> lines;

    void printLines() {
        debugLOG("PrintingSeparatedLines:");
        for (size_t i = 0; i < lines.size(); i++) {
            debugLOG(lines.at(i).command + L"," + lines.at(i).line);
        };
    };

    void procesInput(wstring* str) { //used from stackowerflow https://stackoverflow.com/questions/13172158/c-split-string-by-line
        lines.clear();

        size_t pos = 0;
        size_t prev = 0;
        wstring a = L"";
        while ((pos = str->find(L"\n", prev)) != string::npos) {
            long posN = pos - prev - 1;
            debugLOG((int)posN);
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
        debugLOG("motobyke");
        if (prev < str->length()) {
            a = str->substr(prev);
            registerLine(&a, false);
        }
        printLines();
    };


};
