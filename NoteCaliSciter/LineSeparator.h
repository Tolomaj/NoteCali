#pragma once

#include <vector>
#include <string>

using namespace std;

class LineRegister {
    void registerLine(wstring* iline, bool isCompleted = true) {
        mline separatedLine;
        separatedLine.isEnded = isCompleted;
        if ((iline->size() > 0 && iline->at(0) != L';') || iline->size() == 0) { // neobsahuje ; a není tudíš modifikátor
            separatedLine.line = *iline;
            lines.push_back(separatedLine);
            return;
        }
        int space = iline->find(L" ");
        if (space == string::npos) {
            separatedLine.lineModifier = iline->substr(1,iline->length());
            lines.push_back(separatedLine);
            return;
        };
        separatedLine.lineModifier = iline->substr(1, space - 1);
        separatedLine.line = iline->substr(space + 1, iline->size() - 1);
        lines.push_back(separatedLine);
    }
public:
	vector<mline> lines;

    void printLines() {
        for (size_t i = 0; i < lines.size(); i++) {
            debugLOG(lines.at(i).lineModifier + L"," + lines.at(i).line);
        };
    };

    void procesInput(wstring* str) { //used from stackowerflow https://stackoverflow.com/questions/13172158/c-split-string-by-line
        lines.clear();

        size_t pos = 0;
        size_t prev = 0;
        wstring a = L"";
        while ((pos = str->find(L"\n", prev)) != string::npos) {
            debugLOG("svc");
            debugLOG((int)(pos - prev - 1));
            int posN = pos - prev - 1;
            if (posN < 0) { posN = 0; }
            debugLOG(posN);
            a = str->substr(prev, pos - prev - (str->at(posN) == L'\r'));
                debugLOG(L"psolw");
            registerLine(&a);
            prev = pos + 1;
            debugLOG("pods");
        }
        debugLOG("podss");
        if (prev < str->length()) {
            a = str->substr(prev);
            registerLine(&a, false);
        }

    };


};
