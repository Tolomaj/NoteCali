#include <vector>
#include <string>
#include <cstdio>

// this undefinition is becouse sciter is defining this and exprtk.h dont like that
#undef min
#undef max
#include "exprtk.h"

typedef double T; // numeric type (float, double, mpfr etc...)
typedef exprtk::symbol_table<T> symbol_table_t;
typedef exprtk::expression<T>   expression_t;
typedef exprtk::parser<T>       parser_t;

void trig_function(){
    std::string expression_string = "x := z -y + z*(y +8)" ;
    T x = T(10);
    T y = T(2);
    T z = T(5);
    symbol_table_t symbol_table;
    symbol_table.add_variable("x", x);
    symbol_table.add_variable("y", y);
    symbol_table.add_variable("z", z);
    expression_t expression;
    expression.register_symbol_table(symbol_table);
    parser_t parser;
    if (!parser.compile(expression_string, expression)){
        debugLOG(L"Compilation error...");
        return;
    }
    T result = expression.value();
    //printf(" %19.15f\t %19.15f\t %19.15f\n", x, y, z);
    debugLOG("papre");
    debugLOG(x);
    debugLOG(y);
    debugLOG(z);
    debugLOG(result);
    return;
}


class MatematicalSolver {
    vector<mline> lastLines; // not used use for no need for recalculating all lines

    wstring dtos(double val) { // double to string without useful nulls // not mine
        wstring str = to_wstring(val);
        str.erase(str.find_last_not_of('0') + 1, std::string::npos);
        str.erase(str.find_last_not_of('.') + 1, std::string::npos);
        return str;
    }



    void solveWithExprtk(vector<mline>* lines) { // using Exprtk.h
        symbol_table_t symbol_table;
        parser_t parser;

        
        vector<string> vname; // převedené jmeno na string
        for (size_t i = 0; i < settings.globalVariables.size(); i++) {
            vname.push_back(WstrToStr(settings.globalVariables[i].varName));
            symbol_table.add_variable(vname.at(vname.size()-1), settings.globalVariables[i].number);
        }


        for (int i = 0; i < lines->size(); i++) {
            int eqlPos = lines->at(i).line.find(L"=");
            if (eqlPos == string::npos) { // not variable
                expression_t expression;
                expression.register_symbol_table(symbol_table); // register variables

                string expression_string(lines->at(i).line.begin(), lines->at(i).line.end());

                if (parser.compile(expression_string, expression)) {
                    lines->at(i).solution = dtos(expression.value());
                }
                else {
                    lines->at(i).solutionModifier = L"⚠";
                    continue; // nelze vyřešit
                    //alternative characters for error: 💥👎🕸⚡⚠⛔🚫💢
                }

            }
            else { // is variable
                if (eqlPos - 1 < 0) {
                    lines->at(i).solutionModifier = L"⚠";
                    lines->at(i).solution = L"!="; 
                    continue; //nic není před =
                }
                wstring varName = lines->at(i).line.substr(0, eqlPos);

                if (eqlPos + 1 == lines->at(i).line.length()) {
                    lines->at(i).solutionModifier = L"⚠";
                    lines->at(i).solution = L"=!"; 
                    continue; // nic není za =
                }

                wstring pLine = lines->at(i).line.substr(eqlPos + 1, lines->at(i).line.length());

                if (pLine.find(L"=") != string::npos) { // pokud obsahuje víc jak jedno = // není potřeba pravděpoboně by selhal proces řešení rovnice ale za upozornění to stojí
                    lines->at(i).solutionModifier = L"⚠";
                    lines->at(i).solution = L"!2 *= "; 
                    continue;
                }

                expression_t expression;
                expression.register_symbol_table(symbol_table); // register variables

                string expression_string(pLine.begin(), pLine.end());

                if (parser.compile(expression_string, expression)) {
                    T result = expression.value();
                    lines->at(i).solutionModifier = L"📒";
                    lines->at(i).solution = dtos(result); //alternatives: 🧮📕📒📄📥📁📋

                    string sVarName(varName.begin(), varName.end());

                    symbol_table.add_variable(sVarName, result);
                }
                else {
                    lines->at(i).solutionModifier = L"⚠"; // 💥👎🕸⚡⚠⛔🚫💢
                    continue;
                }

            }
        }
    };

    void solveWithTextLines(vector<mline>* lines) {

    };

public:
    

	void solve(vector<mline>* lines) { // TODO
        debugLOG("starting line Calculations \n\n\n\n\n\n\n");
        solveWithExprtk(lines);

	}

    

};
