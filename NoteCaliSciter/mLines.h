#pragma once
using namespace std;
#include <string>
#include <vector>

struct mline {
	bool isEnded = true;
	bool isComandDone = false;
	bool completlySolved = false;
	std::wstring lineModifier = L"";
	std::wstring line = L"";

	std::wstring localVariableName = L"";
	std::wstring solution = L"none";
};

wstring compositeLines(vector<mline>* lines) {
	wstring output = L"";
	for (size_t i = 0; i < lines->size(); i++) {
		if (lines->at(i).lineModifier != L"") {
			output += L";" + lines->at(i).lineModifier + L" ";
		}
		output += lines->at(i).line;
		if (i < lines->size() - 1) {
			output += L"\n";
		}
	}
	return output;
}