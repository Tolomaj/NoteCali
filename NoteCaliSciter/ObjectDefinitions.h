#pragma once
#pragma once
using namespace std;
#include <string>
#include <vector>


struct MathError {
	int pos = 0;
	int lenght = 0;
	int type = 0;
};


struct mline { // mabey add solution preponder
	bool isEnded = true;	
	bool isSumBrake = false;
	bool isComandDone = false;	// comand on this line is done
	bool completlySolved = false;	// if is there any solution and is proceed
	bool isHiden = false;
	// isComandDome || completlySolved || (error.type > 0) means it is peoceed << mabey error is typed as comment
	wstring originLine = L"";

	wstring command = L"";
	wstring lineModifier = L""; // comand for program or comand that modify line behavior
	wstring line = L"";		// line to solve

	wstring localVariableName = L"";
	wstring solution = L""; // copiable solution to line
	wstring solutionNoRound = L""; // copiable solution to line
	wstring solutionModifier = L""; // not copiable coment to solution

	wstring solutionUnits = L"";

	MathError error;
};

wstring compositeLines(vector<mline>* lines) {
	wstring output = L"";
	for (size_t i = 0; i < lines->size(); i++) {
		if (lines->at(i).command != L"") {
			output += L";" + lines->at(i).command + L" ";
		}
		output += lines->at(i).line;
		if (i < lines->size() - 1) {
			output += L"\n";
		}
	}
	return output;
}


struct Function {
	wstring funcName = L"";
	wstring funcDec = L"";
};