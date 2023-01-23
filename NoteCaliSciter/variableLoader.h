#pragma once

#define CANT_OPEN_FILE 2
#define CANT_CLOSE_FILE 3

struct Variable {
	string varName = "";
	double number = 0;
	Variable(string name, double num) { varName = name; number = num; };
};

class VariablesTable {
public:
	vector<Variable> table; // mabey recrete as list

	int loadVariables();

	int saveVariables();

	int saveRawComposite(wstring s); //set file content to text // budoucí pouzití okno nastanei bude vracet raw text pole pri prenastaveni


}variableTable;

int VariablesTable::loadVariables(){ // naète promìneé ze souboru
	debugLOG("LoadingVariables:");
	CSimpleIniA ini;
	SI_Error rc = ini.LoadFile("constants.nConst"); // file for custom theme 

	if (rc < 0) {
		debugLOG("failed to open file");
		saveRawComposite(L"[constants]\npi = 3.141592\nsqrttwo = 1.4142\neuler = 2.71828182846");
	};
	std::list<CSimpleIniA::Entry> list;
	ini.GetAllKeys("constants", list);
	for (CSimpleIniA::Entry x : list) {
		bool fine = true;
		string s = ini.GetValue("constants", x.pItem, "0");
		double val;
		try {
			val = atof(s.c_str());
		}catch (exception& err) {
			debugLOG("cannot convert:" + s); // nefunguje nastaví promìnou na 0.00 asi nevadí nièemu
			fine = false;
		}
		if (fine) {
			Variable var(string(x.pItem), val);
			table.push_back(var);
		}

	}

	for (size_t i = 0; i < table.size(); i++) { debugLOG(table[i].number); debugLOG(" - " + table[i].varName,false); } // otazkojd
	return 0;
};

int VariablesTable::saveVariables(){
	// asi nebude potøeba
	return 0;
};

int VariablesTable::saveRawComposite(wstring s){
	CSimpleIniA ini;
	std::ofstream appDataFile("constants.nConst");
	if (appDataFile.fail()) {
		appDataFile.close();
		debugLOG("cant crete data file ! MATER ERROR = CLOSING");
		return CANT_CLOSE_FILE;
	}

	appDataFile << s.c_str();
	appDataFile.close();

	SI_Error rc = ini.LoadFile("customTheme.ntheme");
	if (rc < 0) { debugLOG("master error on opening file"); return CANT_OPEN_FILE; } // probably masterErro
	loadVariables();
}; //set file content to text // budoucí pouzití okno nastanei bude vracet raw text pole pri prenastaveni