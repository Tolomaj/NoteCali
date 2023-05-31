#pragma once

#define CANT_OPEN_FILE 2
#define CANT_CLOSE_FILE 3

struct TextVariable {
	string varName = "";
	string number = "";
	TextVariable(string name, string num) { varName = name; number = num; };
};

class VariablesTable {
public:
	vector<TextVariable> table; // mabey recrete as list

	int loadVariables();

	int saveVariables();

	int saveRawComposite(string s); //set file content to text // budoucí pouzití okno nastanei bude vracet raw text pole pri prenastaveni


}variableTable;

int VariablesTable::loadVariables(){ // naète promìneé ze souboru
	debugLOG("Loading Variables:");
	CSimpleIniA ini;
	string s = getenv("APPDATA") + string("\\NoteCali") + string("\\constants.nConst");
	SI_Error rc = ini.LoadFile(s.c_str()); // file for constants
	table.clear();
	if (rc < 0) {
		debugLOG("Failed to open file. Reseting It.");
		saveRawComposite("[constants]\nzero = 0\none = 1\nsqrttwo = 1.4142\n");
	};
	std::list<CSimpleIniA::Entry> list;
	ini.GetAllKeys("constants", list);
	for (CSimpleIniA::Entry x : list) {
		bool fine = true;
		string s = ini.GetValue("constants", x.pItem, "0");
		TextVariable var(string(x.pItem), s);
		table.push_back(var);
	}
	
	for (size_t i = 0; i < table.size(); i++) { debugLOG(table[i].number); debugLOG(" - " + table[i].varName,false); } // otazkojd
	debugLOG("Variables loaded.");
	return 0;
};

int VariablesTable::saveVariables(){
	// 
	return 0;
};

int VariablesTable::saveRawComposite(string s){
	CSimpleIniA ini;
	std::ofstream appDataFile((getenv("APPDATA") + string("\\NoteCali") + string("\\constants.nConst")).c_str());
	if (appDataFile.fail()) {
		appDataFile.close();
		debugLOG("cant crete data file ! MATER ERROR = CLOSING");
		debugERROR(L"Cant find file at" + StrToWstr(getenv("APPDATA") + string("\\NoteCali") + string("\\constants.nConst")) + L". And cant file create.\n(You can try crete thys file by hand)");//only one handle
		return CANT_CLOSE_FILE;
	}

	appDataFile << s.c_str();
	appDataFile.close();

}; //set file content to text // budoucí pouzití okno nastanei bude vracet raw text pole pri prenastaveni