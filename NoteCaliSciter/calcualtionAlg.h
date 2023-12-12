#include <vector>
#include <string>
#include <cstdio>



#define MATHERR_INVALID_NUMBER_FOR_OPERARTION 1
#define MATHERR_INVALID_VARIABLE 2
#define MATHERR_INVALID_FUNCTION 3
#define MATHERR_INVALID_SINTAX_TTMARH 3
#define MATHERR_INVALID_SINTAX 4
#define MATHERR_INVALID_VARIABLE_NAME 5
#define MATHERR_INVALID_LINE_JUMP_NUM 6

#define END_LINE_SOLVING 25

#define VARIABLE_FORBIDENT_SIMBOLS_LENGHT 8
const wchar forbidentVariableSimbols[VARIABLE_FORBIDENT_SIMBOLS_LENGHT] = { '[' , ']' , '{' , '}' , '=' , '&' , '@' , ' '};

#define FAST_SAME false

#define POINTER_CHAR L"@"

class MatematicalSolver {
    typedef ttmath::Big<1, 3> MyBig;
    ttmath::Parser<MyBig> parser;
    ttmath::Objects varTable;
    ttmath::Objects systemVariableTable;
    ttmath::Objects functionTable;//not used jet
    ttmath::Conv ConvertionRole;
    bool lineUsingMetrics;

    vector<mline>* procesedLines;

    bool isSameLine(mline* line1, mline* line2);

    bool hasAnyVariable(mline* line);

    int findEqualition(wstring* line);// vrátí true když opsahuje samostatné = ,index je hejo pozice

    int solveLine(wstring line, wstring* solution, wstring* solutionNoRound, wstring* errorText); // vyřeší linku která neobsahuje žádné custom věci // později budwe řešit proměné a funkce

    bool isValidVariableName(wstring name);

    void creteErrorLineSolution(mline* line, wstring text, int errorCode);

    void removeBeforeAfterSpaces(wstring* line);

    bool solveVariableLine(mline* line, int eqPos);

    void executeMathComand(mline* line);

    void addMetricsToTable();

    int modifyMathRules(wstring* s);

    bool setDefaultMathRules();

    void calculateSumForLinesAbove( int lineNum);

public:
    
    void begin();

    void loadVariablesFromTable();

    int solve(vector<mline>* lines);

};

void MatematicalSolver::loadVariablesFromTable() {
    varTable.Clear();
    systemVariableTable.Clear();
    parser.SetVariables(&varTable);

    for (size_t i = 0; i < variableTable.table.size(); i++) {
        debugLOG(variableTable.table.at(i).varName);
        systemVariableTable.Add(variableTable.table.at(i).varName, variableTable.table.at(i).number);
    }

    varTable = systemVariableTable;
}

void MatematicalSolver::begin() {
    loadVariablesFromTable();

    //functionTable.Add(L"pof(x)", L"x+4+x*2"); //<not work
    //parser.SetFunctions(&functionTable);
}

void MatematicalSolver::calculateSumForLinesAbove(int lineNum) {

    wstring lineFormula = L"";

    //vytvoří formulu všeho nad
    int momental = lineNum - 1;
    while (momental >= 0) {

        // sčítéme linky jen po sumbreak comand
        if (procesedLines->at(momental).isSumBrake == true) {
            break;
        }

        //pokud linka nemá číslo k řešení není možná jí sčítat takže přeskakujeme
        if (procesedLines->at(momental).solutionNoRound == L"") {
            momental--;
            continue;
        }

        // pro první číslo znaménko vynecháváme
        if (momental != lineNum) {
            lineFormula = lineFormula + L"+";
        }

        lineFormula = lineFormula + procesedLines->at(momental).solutionNoRound;


        momental--;
    }


    if (lineFormula != L"") {
        ttmath::ErrorCode err = parser.Parse(lineFormula);

        if (err == 0) {
            varTable.EditValue("sum", parser.stack[0].value.ToString());

            debugLOG("suma pro linku - " + to_string(lineNum) + " : " + parser.stack[0].value.ToString());
        } else {
            debugLOG("error sumarizace - " + to_string(lineNum) + " : ",false);
            debugLOG(lineFormula);
        }

    }
};

int MatematicalSolver::solve(vector<mline>* lines) { 
    procesedLines = lines;
    //nastavení pravidel pro řešení linek
    setDefaultMathRules();

    if (settings.useMetrics) {
        addMetricsToTable();
    }

    parser.SetDegRadGrad(settings.useRadians);

    //pokud se používá sum tak přidáme proměnou
    if (settings.useSumVariable) {
        varTable.Add("sum", "0");
    };

    debugLOG("<------ SOLVING LINES ------>");

    for (size_t i = 0; i < lines->size(); i++) {

        // linka obsahuje příkaz co ukončí řešení momentální linky
        if (settings.useLineModifiers && modifyMathRules(&lines->at(i).lineModifier) == END_LINE_SOLVING) { 
            continue; 
        };

        // linka je už vyřešená nebo příkaz
        if (lines->at(i).completlySolved || lines->at(i).isComandDone) { 
            continue; 
        }  

        //linka obsahuje matematický příkaz
        if (lines->at(i).command != L"") { 
            executeMathComand(&lines->at(i));
        }
        // pokud matematický příkaz vyřešil linku
        if (lines->at(i).completlySolved ) { 
            continue; 
        }


        // pokud je zaplé používání sum proměnné tak se vypočítá
        if (settings.useSumVariable) {
            calculateSumForLinesAbove(i);
        }


        int eqPos = findEqualition(&lines->at(i).line);
        if (eqPos >= 0) { //is variable 
            solveVariableLine(&lines->at(i),eqPos);
        } else { // is not variable
            wstring error = L"Line Unsolvable";
            if (solveLine(lines->at(i).line, &lines->at(i).solution, &lines->at(i).solutionNoRound, &error) != 0) { //error check
                creteErrorLineSolution(&lines->at(i), error, MATHERR_INVALID_NUMBER_FOR_OPERARTION);
            }else {
                lines->at(i).completlySolved = true;
            }
        };

        if (lines->at(i).lineModifier != L"") {
            setDefaultMathRules();
        }

        debugLOG(L"výsledek linky - " + to_wstring(i) + L" : " + lines->at(i).solution + L"\n");


    }
    lineUsingMetrics = false;
    varTable = systemVariableTable;
    debugLOG("<---- SOLVING LINES DONE ---->");
   
    return 0;
}


int MatematicalSolver::solveLine(wstring line, wstring* solution, wstring* solutionNoRound,wstring * errorText) { // vyřeší linku která neobsahuje žádné custom věci // později budwe řešit proměné a funkce
    
    
    if (settings.useMetrics || lineUsingMetrics) { // check to somethink
        size_t ocur = line.find(L" to ");
        while (ocur != wstring::npos){
            line = L"(" + line;
            line.replace(ocur+1, 4, L")/");
            ocur = line.find(L"to ");
        }
        //line.replace(L"to",L"/");
    }

    // korekce neuzavřených závorek & ignorování veikosti písmen
    if (settings.ignoreHightDiference || settings.corectParenthesis) {
        size_t parCountOP = 0,parCountCL = 0;
        for (size_t i = 0; i < line.length(); i++){
            if (settings.ignoreHightDiference) {
                line.at(i) = towlower(line.at(i));      //set all characters to small
            }
            if (settings.corectParenthesis) {
                if (line.at(i) == L'(') {
                    parCountOP++;
                }else if (line.at(i) == L')') {
                    parCountCL++;
                }
            }
        }

        if (settings.corectParenthesis) {
            if (parCountOP > parCountCL) {
                for (size_t i = 0; i < parCountOP - parCountCL; i++){
                    line = line + L")";
                }
            }
        }
    }

    // pokud jsou zaplé ljumps anhradí skok výsledkem
    if (settings.allowLineJump) {
        size_t lastSeenEq = line.find(POINTER_CHAR);
        while (lastSeenEq != string::npos) {  
            for (int i = lastSeenEq; i < line.length(); i++) {
                bool isOnEnd = i == line.length() - 1;
                if (isOnEnd || !iswdigit(line.at(i + 1))) {
                    try {
                        int lineId = stoi(line.substr(lastSeenEq + 1, i - lastSeenEq));
                        if (lineId >= procesedLines->size() - 1) {
                            *errorText = L"Invalid Line Jump";
                            return MATHERR_INVALID_LINE_JUMP_NUM;
                        }
                        if (settings.useNoroundPointers) {
                            line = line.substr(0, lastSeenEq) + L"(" + procesedLines->at(lineId).solutionNoRound + L")" + line.substr(i + 1, line.length() - i);
                        } else {
                            line = line.substr(0, lastSeenEq) + L"(" + procesedLines->at(lineId).solution + L")" + line.substr(i + 1, line.length() - i);
                        }
                    }
                    catch (exception& err) {
                        *errorText = L"No Line Jump Number";
                        return MATHERR_INVALID_LINE_JUMP_NUM;
                    }
                    break;
                }
            }
            lastSeenEq = line.find(POINTER_CHAR, ++lastSeenEq);
        }
    }

    // vyřešit linku
    ttmath::ErrorCode err = parser.Parse(line); // actuali solve line


    // nastavit řešení
    if (err == 0) {
        *solution = parser.stack[0].value.ToWString(ConvertionRole);
        *solutionNoRound = parser.stack[0].value.ToWString();
    }else {
        *errorText = L"Line Unsolvable";
    }

    return err;
}

wstring getNumFrom(wstring * s,int index) {
    wstring num = L"";
    for (int i = index; i < s->length();i++) {
        if (iswdigit(s->at(i))) {
            num = num + s->at(i);
        } else {
            return num;
        }
    }
    return num;
}

int MatematicalSolver::modifyMathRules(wstring * rules){
    size_t rulePos = rules->find_last_of(L'r');
    if (rulePos != string::npos) {
        try {
            ConvertionRole.round = std::stoi(getNumFrom(rules, rulePos + 1));
        }
        catch (std::exception const& e) {
            debugLOG("error modifier INT r");
        }
    }


    rulePos = rules->find_last_of(L'b');
    if (rulePos != string::npos) {
        try {
            ConvertionRole.base = std::stoi(getNumFrom(rules, rulePos + 1));
        }
        catch (std::exception const& e) {
            debugLOG("error error modifier INT b");
        }
    }

    rulePos = rules->find(L'c');
    if (rulePos != string::npos) {
        return END_LINE_SOLVING;
    }
    


    //chceck for Coment


    return 0;
};

bool MatematicalSolver::setDefaultMathRules() {
    ConvertionRole.round = settings.roundToDec;
    ConvertionRole.group = ' ' * (bool)settings.numberGrouping;
    ConvertionRole.group_digits = settings.numberGrouping;
    ConvertionRole.scient = settings.useSientific;
    ConvertionRole.base_round = false;
    ConvertionRole.scient_from = 100; //pokud je nad 100 míst použije se sience number i když je vyplé (kvuli tomu aby se nepoužívalo dřív)
    ConvertionRole.base = 10;
    return true;
};


void MatematicalSolver::addMetricsToTable() { // zkontrolovat převod (:
    if (!(varTable.IsDefined("km") || varTable.IsDefined("den") || varTable.IsDefined("mg"))) {
        varTable.Add("km", "1000");
        varTable.Add("m", "1");
        varTable.Add("dm", "0.1");
        varTable.Add("cm", "0.01");
        varTable.Add("mm", "0.001");

        varTable.Add("den", "86400");
        varTable.Add("day", "86400");
        varTable.Add("h", "3600");
        varTable.Add("min", "60");
        varTable.Add("s", "1");

        varTable.Add("mg", "0.001");
        varTable.Add("g", "1");
        varTable.Add("dag", "10");
        varTable.Add("kg", "1000");
        varTable.Add("t", "1000000");


        //přidat měny


    }
    
}


void MatematicalSolver::removeBeforeAfterSpaces(wstring* line) {
    size_t start = 0;
    for (size_t i = 0; i < line->length(); i++) {
        if (line->at(i) != L' ') {
            start = i;
            break;
        }
    }
    for (int i = line->length() - 1; i >= 0; i--) {
        if (line->at(i) != L' ') {
            *line = line->substr(start, i - start + 1);
            return;
        }
    }
}



bool MatematicalSolver::solveVariableLine(mline* line, int eqPos) {

    wstring beforeEQ = line->line.substr(0, eqPos);

    if (settings.ignoreHightDiference) {
        size_t parCountOP = 0, parCountCL = 0;
        for (size_t i = 0; i < beforeEQ.length(); i++) {
            beforeEQ.at(i) = towlower(beforeEQ.at(i));      //set all characters to small
        }
    }

    if (settings.useSumVariable && beforeEQ == L"sum") {
        creteErrorLineSolution(line, L"Redefintion of system Sum variable", MATHERR_INVALID_VARIABLE_NAME);
        return false;
    }
    

    removeBeforeAfterSpaces(&beforeEQ);

    wstring afterEQ = line->line.substr(eqPos + 1, line->line.length());

    if (!isValidVariableName(beforeEQ)) { //ošetření neplatného jména
        creteErrorLineSolution(line, L"Variable Name contains Forbident simbols", MATHERR_INVALID_VARIABLE_NAME);
        return false;
    }

    wstring errorText = L"Math Err";

    if (solveLine(afterEQ, &line->solution,&line->solutionNoRound, &errorText) != 0) { // is is error
        creteErrorLineSolution(line, errorText, MATHERR_INVALID_NUMBER_FOR_OPERARTION);
        return false;
    }
    else {
        line->completlySolved = true;
        line->localVariableName = L"📋";// beforeEQ; 

        wstring solutionVtext;
        if (settings.useNoroundPointers) {
            solutionVtext = line->solutionNoRound;
        }else {
            solutionVtext = line->solution;
        }

        if (varTable.IsDefined(beforeEQ)) {
            varTable.EditValue(beforeEQ, solutionVtext);
        } else {
            varTable.Add(beforeEQ, solutionVtext);
        };
    }
    return true;
}



void MatematicalSolver::executeMathComand(mline* line) {


    if (line->command == L"sumbrake" || line->command == L"rstsum") {
        line->solution = L"🚮";
        line->isComandDone = true;
        line->completlySolved = true;
        line->isSumBrake = true;
    }else if (line->command == L"sum") {
        wstring textnumber = L"";
        for (size_t i = 0; i < procesedLines->size(); i++) {
            if (&procesedLines->at(i) == line) { break; } // sum only lines abo;
            if (procesedLines->at(i).isSumBrake == true) { textnumber = L""; }
            if (procesedLines->at(i).completlySolved && procesedLines->at(i).localVariableName == L"") {
                textnumber.append(procesedLines->at(i).solution + L" + ");
            }
            debugLOG(textnumber);
        }

        textnumber.append(L"0");
        ttmath::ErrorCode err = parser.Parse(textnumber);
        if (err == 0) {
            //line->solution = parser.stack[0].value.ToWString();
            line->line = parser.stack[0].value.ToWString() + L" " + line->line;
            line->isComandDone = true;
            line->completlySolved = false;
        }
        else {
            line->solution = L"countingProblem";
            line->isComandDone = true;
            line->completlySolved = true;
        }
    }
    else if (line->command == L"sumV") {
        wstring textnumber = L"";
        for (size_t i = 0; i < procesedLines->size(); i++) {
            if (&procesedLines->at(i) == line) { break; } // sum only lines abo;
            if (procesedLines->at(i).command == L"rstsum") { textnumber = L""; }
            if (procesedLines->at(i).completlySolved) {
                textnumber.append(procesedLines->at(i).solution + L" + ");
            }
            debugLOG(textnumber);
        }

        textnumber.append(L"0");
        ttmath::ErrorCode err = parser.Parse(textnumber);
        if (err == 0) {
            line->line = parser.stack[0].value.ToWString() + L" " + line->line;
            line->isComandDone = true;
            line->completlySolved = false;
        }
        else {
            line->solution = L"countingProblem";
            line->completlySolved = true;
        }
    }
    else if (line->command == L"rand") {
        double i = (float)rand() / (RAND_MAX);
        line->line = to_wstring(i) + L" " + line->line;
        line->isComandDone = true;
        line->completlySolved = false;
    }
    else if (line->command == L"randi") {
        int i = rand();
        line->line = to_wstring(i) + L" " + line->line;
        line->isComandDone = true;
        line->completlySolved = false;
    }
    else if (line->command == L"randn") {
        float i = ((float)rand() / (RAND_MAX) - 0.5)*2;
        line->line = to_wstring(i) + L" " + line->line;
        line->isComandDone = true;
        line->completlySolved = false;
    }
    else if (line->command == L"metrics") {
        addMetricsToTable();
        lineUsingMetrics = true;
        line->solution = L"Metrics Added";
        line->isComandDone = true;
        line->completlySolved = true;
    }
}



bool MatematicalSolver::isSameLine(mline* line1, mline* line2) {
    size_t lineSize1 = line1->line.size();
    size_t lineSize2 = line2->line.size();

    if (line1->line.size() != line2->line.size() || line1->command.size() != line2->command.size()) {
        return false;
    }
    else if (FAST_SAME) {
        return true;
    }

    return line1->line.compare(line2->line) && line1->command.compare(line2->command);

}



bool MatematicalSolver::hasAnyVariable(mline* line) {
    return false;
}



int MatematicalSolver::findEqualition(wstring* line) { // vrátí true když opsahuje samostatné = ,index je hejo pozice
    size_t lastSeenEq = line->find(L"=");

    while (lastSeenEq != string::npos) {

        // line contains -> (  line->at(lastSeenEq + 1) == L'=' ||  )
        //test = behind
        // test alos before

        if (lastSeenEq + 1 < line->length() && line->at(lastSeenEq + 1) != L'=') {
            if (lastSeenEq - 1 >= 0 && line->at(lastSeenEq - 1) != L'<' && line->at(lastSeenEq - 1) != L'<' && line->at(lastSeenEq - 1) != L'!') { // check if = is not operation
                return lastSeenEq;  // vrtí pozici =
            }

        }
        lastSeenEq = line->find(L"=", ++lastSeenEq);
    }
    return -1;
};




bool MatematicalSolver::isValidVariableName(wstring name) { // var neobsahuje []{}=&@ a ani číslo // mabey rerete variableTable.IsNameCorrect
    if (iswdigit(name.at(0))) { return false; }

    for (size_t i = 0; i < name.length(); i++) {
        wchar simbol = name.at(i);
        for (size_t c = 0; c < VARIABLE_FORBIDENT_SIMBOLS_LENGHT; c++) {
            if (simbol == forbidentVariableSimbols[c]) {
                return false;
            }
        }
    }
    return true;
}



void MatematicalSolver::creteErrorLineSolution(mline* line, wstring text, int errorCode) {
    line->completlySolved = false;
    line->solutionModifier = L"⚠";
    line->solution = text;
    line->error.type = errorCode;
}