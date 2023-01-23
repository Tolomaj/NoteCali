#include <vector>
#include <string>
#include <cstdio>


class TextMathSolver {


    bool isNumber(wchar c) {
        if (c >= '0' && c <= '9' || c == ',' || c == '.') {
            return true;
        }
        return false;
    }

    wstring getNumAndOperation(wstring s, size_t operationIndex) {
        wstring num;
        for (size_t i = operationIndex; i > 0; i--) { // cant be >= 0 because size_t is unsigned
            if (isNumber(s.at(i - 1))) {
                num = s.at(i - 1) + num;
            }
            else if (i == operationIndex && s.at(i - 1) == ' ') {
            }
            else {
                break;
            }
        }
        wstring num2;

        for (size_t i = operationIndex + 1; i < s.length(); i++) {
            if (isNumber(s.at(i))) {
                num2 = num2 + s.at(i);
            }
            else if (i == operationIndex + 1 && s.at(i) == ' ') {
            }
            else {
                break;
            }
        }

        debugLOG(num + L" -X- " + num2);

        return L"";
    }

    wstring solveBasicLine() {

    }




public:

    void solve(vector<mline>* lines) {
        for (size_t i = 0; i < lines->size(); i++) {
            size_t EqPos = 0;

            if ((EqPos = lines->at(i).line.find(L"+")) != string::npos) { // obashuje =
                getNumAndOperation(lines->at(i).line, EqPos);
            }
        }


        for (size_t i = 0; i < lines->size(); i++) {
            size_t EqPos = 0;

            if ((EqPos = lines->at(i).line.find(L"=")) != string::npos) { // obashuje =
                debugLOG("je ll");
            }
            else {
                debugLOG("je pos");
            }



        }

    }


};
