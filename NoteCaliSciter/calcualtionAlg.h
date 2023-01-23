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

#include "textMath.h"

class MatematicalSolver {
    TextMathSolver textSolver;
    vector<mline> lastLines; // not used use for no need for recalculating all lines
    



    void solveWithExprtk(vector<mline>* lines) { // using Exprtk.h // předěla nefunguje multi variable
        symbol_table_t symbol_table;
        parser_t parser;

        
        vector<string> vname; // převedené jmeno na string
        vector<double> vval; // převedené jmeno na string
        for (size_t i = 0; i < variableTable.table.size(); i++) {
            vname.push_back(variableTable.table[i].varName);
            symbol_table.add_variable(vname.at(vname.size()-1), variableTable.table[i].number);
        }

        for (int i = 0; i < lines->size(); i++) {
            size_t eqlPos = lines->at(i).line.find(L"=");
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
                    double D = expression.value();
                    vval.push_back(D);
                    debugLOG("idk");
                    lines->at(i).solutionModifier = L"📒";

                    string sVarName(varName.begin(), varName.end());

                    debugLOG("msize" + to_string(vval.size()));
                    lines->at(i).solution = dtos(vval.at(vval.size()-1)) + L" : " + StrToWstr(sVarName); //alternatives: 🧮📕📒📄📥📁📋
                    debugLOG(dtos(vval[vval.size()-1]) + L" : " + StrToWstr(sVarName));
                    
                    debugLOG("bodp");
                    symbol_table.add_variable(sVarName, vval[vval.size()-1]);
                }
                else {
                    lines->at(i).solutionModifier = L"⚠"; // 💥👎🕸⚡⚠⛔🚫💢
                    continue;
                }
                
                    
            }
        }

        for (size_t i = 0; i < vval.size(); i++)
        {
            debugLOG(to_wstring(i) + L" : " + dtos(vval[i]));
        }

    };


#define FAST_SAME false
    bool isSameLine(mline *line1,mline *line2) {
        size_t lineSize1 = line1->line.size();
        size_t lineSize2 = line2->line.size();

        if (line1->line.size() != line2->line.size() || line1->lineModifier.size() != line2->lineModifier.size()) {
            return false;
        }
        else if (FAST_SAME) {
            return true;
        }

        return line1->line.compare(line2->line) && line1->lineModifier.compare(line2->lineModifier);
        
    }
    
    bool hasAnyVariable(mline *line) {
        return false;
    }




public:
    

	void solve(vector<mline>* lines) { // TODO
        debugLOG("starting line Calculations \n\n\n\n\n\n\n");

        size_t smalerSize = std::min(lines->size(), lastLines.size());

        auto start = chrono::high_resolution_clock::now();
        ios_base::sync_with_stdio(false);


        for (size_t i = 0; i < smalerSize; i++){
            if (isSameLine(&lines->at(i), &lastLines.at(i)) && !hasAnyVariable(&lines->at(i))) {
                lines->at(i).completlySolved = true;
                lines->at(i).solution = lastLines.at(i).solution;
            }
        }
        
            
        switch (settings.mathType){
            case TYPE_EXPRTKMATH:
                solveWithExprtk(lines);
                break;
            case TYPE_TEXTMATH:
                textSolver.solve(lines);
                break;
            case TYPE_TREEMATH:
                debugLOG("no mathfunction creted");
                break;
            default:
                debugLOG("no math type Selected");
                break;
        }
        debugLOG("mathTiope: " + to_string(settings.mathType));

        lastLines = *lines;


        auto end = chrono::high_resolution_clock::now();
        double time_taken = chrono::duration_cast<chrono::nanoseconds>(end - start).count();

        debugLOG("Time taken: " + to_string(time_taken));
     

	}

    

};
