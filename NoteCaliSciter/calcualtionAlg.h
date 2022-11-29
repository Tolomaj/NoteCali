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



class MatematicalSolver {
    vector<mline> lastLines; // not used use for no need for recalculating all lines



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
            } else { // is variable
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

        switch (settings.mathType){
            case TYPE_EXPRTKMATH:
                solveWithExprtk(lines);
                break;
            case TYPE_TEXTMATH:
                debugLOG("no mathfunction creted");
                break;
            case TYPE_TREEMATH:
                debugLOG("no mathfunction creted");
                break;
            default:
                debugLOG("no math type Selected");
                break;
        }



	}

    

};
