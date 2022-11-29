#include "library/SimpleIni.h" // https://github.com/pulzed/mINI
#include <fstream>

#define CUSTOM 3
#define DARK 2
#define LIGHT 1
#define AUTO 0

#define TYPE_TEXTMATH 0
#define TYPE_TREEMATH 1
#define TYPE_EXPRTKMATH 2


#define SHOW_APP_NAME_DEFAULT true
#define HIGHLITE_ERR_DEFAULT true
#define HIGHLITE_VAR_DEFAULT true
#define HIGHLITE_SUPERLINE_DEFAULT true
#define CLICK_TO_COPY_DEFAULT true
#define SHOW_LINE_NUMBERS_DEFAULT false
#define ALL_SUPERLINE_DEFAUTL false
#define SHOW_LINE_END_DEFAULT false

#define BACKGROUND_COLOR_DEFAULT "#d9d9d9"
#define DIVIDER_LINE_COLOR_DEFAULT "#10819A"
#define FONT_COLOR_DEFAULT "#1F2937"
#define SOLUTION_FONT_COLOR_DEFAULT "#0E2235"
#define CLICK_COLOR_DEFAULT "#2F71AF"
#define HOWER_COLOR_DEFAULT "#1C456B"
#define BAR_ICON_DEFAULT "#ffffff"


#define TRANSPARENT_MODE_DEFAULT 100
#define FONT_SIZE_DEFAULT 18
#define FONT_PADDING_DEFAULT 2
#define MATHTYPE_DEFAULT TYPE_EXPRTKMATH



//exit codes//
#define SETINGS_FILE_NOT_FOUND 2
#define SETINGS_LOADED_SCUESFUL 0


//exit codes END//




struct Variable {
	wstring varName = L"";
	double number = 0;
	Variable(wstring name, double num) { varName = name; number = num; };
};

struct Function {
	wstring funcName = L"";
	wstring funcDec = L"";
};

class SettingsOBJ {
public:  
	vector<Variable> globalVariables;
	vector<Variable> userVariables;
	vector<Function> userFunctions;

	int stylescheme = AUTO;
	int mathType = MATHTYPE_DEFAULT;
	
	bool showAppName = SHOW_APP_NAME_DEFAULT; 
	bool highliteERR = HIGHLITE_ERR_DEFAULT;
	bool highliteVAR = HIGHLITE_VAR_DEFAULT;
	bool highliteSUPER = HIGHLITE_SUPERLINE_DEFAULT;
	bool clickToCopy = CLICK_TO_COPY_DEFAULT;
	bool showLineEnd = SHOW_LINE_END_DEFAULT;
	bool showLineNumbers = SHOW_LINE_NUMBERS_DEFAULT;
	bool isAllLinesSuperlines = ALL_SUPERLINE_DEFAUTL;
	bool countingOnLineEnd = false;
	bool ignoreVarCapitalLetters = true; // not implemented in setting process

private:
	#define BOOL_VAR_NUM 9 // idk jak jinak to dìlat ve foru
	bool systemBoolValue[BOOL_VAR_NUM] =		  { true                , true               , true               , true                       , true                  , true                  , false                     , true            	   , true				 };
	bool * boolValPointers[BOOL_VAR_NUM] =        { &showAppName        , &highliteERR       , &highliteVAR       , &highliteSUPER             , &clickToCopy          , &showLineEnd          , &showLineNumbers          , &isAllLinesSuperlines , &countingOnLineEnd  };
	std::string boolVariableNames[BOOL_VAR_NUM] = { "showAppName"       , "highliteERR"      , "highliteVAR"      , "highliteSUPER"            , "clickToCopy"         , "showLineEnd"         , "showLineNumbers"         , "isAllLinesSuperlines", "countingOnLineEnd" };
	bool boolValDefault[BOOL_VAR_NUM] =           {SHOW_APP_NAME_DEFAULT,HIGHLITE_ERR_DEFAULT,HIGHLITE_VAR_DEFAULT, HIGHLITE_SUPERLINE_DEFAULT , CLICK_TO_COPY_DEFAULT , SHOW_LINE_END_DEFAULT , SHOW_LINE_NUMBERS_DEFAULT , ALL_SUPERLINE_DEFAUTL , false               };

	#define INT_VAR_NUM 2 // idk jak jinak to dìlat ve foru
	bool systemIntValue[INT_VAR_NUM] =                { true         , true };
	int* systemIntValPointers[INT_VAR_NUM] =          { &stylescheme , &mathType };
	std::string systemIntVariableNames[INT_VAR_NUM] = { "stylescheme", "mathType"     };
	int systemIntValDefault[INT_VAR_NUM] =            { AUTO         , MATHTYPE_DEFAULT };



public:
	double transparencity = TRANSPARENT_MODE_DEFAULT;
	double fontSize = FONT_SIZE_DEFAULT;
	double fontPadding = FONT_PADDING_DEFAULT;

private:
	#define DOUBLE_VAR_NUM 3 // idk jak jinak to dìlat ve foru
	bool systemDoubleValue[BOOL_VAR_NUM] =            { false                     , true             , true                 };
	double* doubleValPointers[DOUBLE_VAR_NUM] =       { &transparencity          , &fontSize         , &fontPadding         };
	std::string doubleVariableNames[DOUBLE_VAR_NUM] = { "transparencity"         , "fontSize"        , "fontPadding"        };
	double doubleValDefault[DOUBLE_VAR_NUM] =         { TRANSPARENT_MODE_DEFAULT , FONT_SIZE_DEFAULT , FONT_PADDING_DEFAULT };




public:
	std::string backgroudColor = BACKGROUND_COLOR_DEFAULT;
	std::string dividerLineColor = DIVIDER_LINE_COLOR_DEFAULT;
	std::string fontColor = FONT_COLOR_DEFAULT;
	std::string solutionFontColor = SOLUTION_FONT_COLOR_DEFAULT;
	std::string clickColor = CLICK_COLOR_DEFAULT;
	std::string howerColor = HOWER_COLOR_DEFAULT;
	std::string barIconColor = BAR_ICON_DEFAULT;
private:

	/*
		std::string backgroudColor = ;
	*/

	#define TEXT_VAR_NUM 7 // idk jak jinak to dìlat ve foru
	bool systemTextValue[TEXT_VAR_NUM] =          { false                    , false                      , false              , false                       , false               , false               , false         };
	std::string * textValPointers[TEXT_VAR_NUM] = { &backgroudColor          , &dividerLineColor,           &fontColor,          &solutionFontColor,           &clickColor,          &howerColor         , &barIconColor };
	std::string textVariableNames[TEXT_VAR_NUM] = { "backgroudColor"         , "dividerLineColor",          "fontColor",         "solutionFontColor",          "clickColor",         "howerColor"        , "barIconColor"};
	std::string textValDefault[TEXT_VAR_NUM] =    { BACKGROUND_COLOR_DEFAULT , DIVIDER_LINE_COLOR_DEFAULT , FONT_COLOR_DEFAULT , SOLUTION_FONT_COLOR_DEFAULT , CLICK_COLOR_DEFAULT , HOWER_COLOR_DEFAULT , BAR_ICON_DEFAULT };




public:



	bool ishighlitingOn() {
		return highliteERR || highliteVAR;
	}

	int loadSystemSettings();

	int loadSettings();

	bool saveSettings();
	bool SaveSystemSettings();
	bool SaveThemeSettings();

	bool resetSettingsToDefault();

	bool resetSettingsFiles(int file);

	bool setSingleSettingsbyName(std::string name, bool value);
	bool setSingleSettingsbyName(std::string name, double value);
	bool setSingleSettingsbyName(std::string name, int value);
	bool setSingleSettingsbyName(std::string name, std::string value);

	bool setSetting(std::string name, std::string value);

	wstring getSettingAsWstring(std::string name);

	string compositeDefaultSystemSettingsString();

	bool is_light_theme();

}settings;


string SettingsOBJ::compositeDefaultSystemSettingsString() {
	return "none";
}


wstring SettingsOBJ::getSettingAsWstring(std::string name) {
	wstring result = L"";
	for (size_t i = 0; i < BOOL_VAR_NUM; i++) {
		if (boolVariableNames[i] == name) {
			return *boolValPointers[i]? L"false" : L"true";
		}
	}
	for (size_t i = 0; i < DOUBLE_VAR_NUM; i++) {
		if (doubleVariableNames[i] == name) {
			return to_wstring(*doubleValPointers[i]);
		}
	}
	for (size_t i = 0; i < INT_VAR_NUM; i++) {
		if (systemIntVariableNames[i] == name) {
			return to_wstring(*systemIntValPointers[i]);
		}
	}
	for (size_t i = 0; i < TEXT_VAR_NUM; i++) {
		if (textVariableNames[i] == name) {
			return StrToWstr(*textValPointers[i]);
		}
	}
	debugLOG("settingNotFound by name!");
	return L"none";
};

bool SettingsOBJ::setSetting(std::string name, std::string value) {
	for (size_t i = 0; i < BOOL_VAR_NUM; i++) {
		if (boolVariableNames[i] == name) {
			*boolValPointers[i] = stringToBool(value);
			return true;
		}
	}
	for (size_t i = 0; i < DOUBLE_VAR_NUM; i++) {
		if (doubleVariableNames[i] == name) {
			*doubleValPointers[i] = std::stod(value);
			return true;
		}
	}
	for (size_t i = 0; i < INT_VAR_NUM; i++) {
		if (systemIntVariableNames[i] == name) {
			*systemIntValPointers[i] = std::stoi(value);
			return true;
		}
	}
	for (size_t i = 0; i < TEXT_VAR_NUM; i++) {
		if (textVariableNames[i] == name) {
			*textValPointers[i] = value;
			return true;
		}
	}
	debugLOG("settingNotFound by name!");
	return false;
}



bool SettingsOBJ::setSingleSettingsbyName(std::string name,bool value) {
	for (size_t i = 0; i < BOOL_VAR_NUM; i++) {
		if (boolVariableNames[i] == name) {
			*boolValPointers[i] = value;
			return true;
		}
	}
	debugLOG("settingNotFound by name!");
	return false;
};

bool SettingsOBJ::setSingleSettingsbyName(std::string name, double value) {
	for (size_t i = 0; i < DOUBLE_VAR_NUM; i++) {
		if (doubleVariableNames[i] == name) {
			*doubleValPointers[i] = value;
			return true;
		}
	}
	debugLOG("settingNotFound by name!");
	return false;

};

bool SettingsOBJ::setSingleSettingsbyName(std::string name, int value) {
	for (size_t i = 0; i < INT_VAR_NUM; i++) {
		if (systemIntVariableNames[i] == name) {
			*systemIntValPointers[i] = value;
			return true;
		}
	}
	debugLOG("settingNotFound by name!");
	return false;

};

bool SettingsOBJ::setSingleSettingsbyName(std::string name, std::string value) {
	for (size_t i = 0; i < TEXT_VAR_NUM; i++) {
		if (textVariableNames[i] == name) {
			*textValPointers[i] = value;
			return true;
		}
	}
	debugLOG("settingNotFound by name!");
	return false;
};



bool SettingsOBJ::resetSettingsToDefault() {
	for (size_t i = 0; i < INT_VAR_NUM; i++) {
		*systemIntValPointers[i] = systemIntValDefault[i];
	}
	for (size_t i = 0; i < BOOL_VAR_NUM; i++) {
		*boolValPointers[i] = boolValDefault[i];
	}
	for (size_t i = 0; i < DOUBLE_VAR_NUM; i++) {
		*doubleValPointers[i] = doubleValDefault[i];
	}
	for (size_t i = 0; i < TEXT_VAR_NUM; i++) {
		*textValPointers[i] = textValDefault[i];
	}

	return 0;
};

#define DATAFILE 1
#define STYLEFILE 2
#define BOATH 0

bool SettingsOBJ::resetSettingsFiles(int file  = 0) { // DATAFILE,STYLEFILE,BOATH
	debugLOG("resetingSettings");
	std::string defaultLightTheme = "showLineNumbers = false\ntransparencity = 100.000000\nfontSize = 18.000000\nfontPadding = 2.000000\nbackgroudColor = #d9d9d9\ndividerLineColor = #10819A\nfontColor = #1F2937\nsolutionFontColor = #0E2235\nclickColor = #2F71AF\nhowerColor = #1C456B\n";
	if (file == 0 || file == DATAFILE) {
		std::string defaultDarkTheme = "showLineNumbers = false\ntransparencity = 100.000000\nfontSize = 18.000000\nfontPadding = 2.000000\nbackgroudColor = #d9d9d9\ndividerLineColor = #10819A\nfontColor = #1F2937\nsolutionFontColor = #0E2235\nclickColor = #2F71AF\nhowerColor = #1C456B\n";
		std::ofstream appDataFile("appData.ini");
		if (appDataFile.fail()) {
			appDataFile.close();
			debugLOG("cant crete data file ! MATER ERROR = CLOSING");
			exit(1);
		}



		appDataFile << "[settings]\nstylescheme = 0\nmathType = 1\n[defaultDarkTheme]\n" + defaultLightTheme + "\n[defaultLightTheme]\n" + defaultDarkTheme;
		appDataFile.close();
	}
	if (file == 0 || file == STYLEFILE) {
		std::ofstream CustomThemeFile("customTheme.ntheme");
		if (CustomThemeFile.fail()) {
			CustomThemeFile.close();
			debugLOG("cant crete theme file ! MATER ERROR = CLOSING");
			exit(1);
		}
		CustomThemeFile << "[theme]\n" + defaultLightTheme;
		CustomThemeFile.close();
	}

	return 0;
};

bool is_file_exist(const char* fileName){
	std::ifstream infile(fileName);
	return infile.good();
}


int SettingsOBJ::loadSystemSettings() {
	CSimpleIniA ini;
	ini.SetUnicode();

	SI_Error rc = ini.LoadFile("appData.ini"); // file for system setings settings 

	if (rc < 0) {
		debugLOG("failed to open file. Reseting Settings!");
		resetSettingsFiles(DATAFILE);
		SI_Error rc = ini.LoadFile("appData.ini");					//try load data second time / default data expected 
		if (rc < 0) { debugLOG("master error on opening file");  return SETINGS_FILE_NOT_FOUND; } // probably masterError
	};

	for (size_t i = 0; i < INT_VAR_NUM; i++) {	// save system varables
		if (systemIntValue[i]) {
			*systemIntValPointers[i] = std::stoi(ini.GetValue("settings", systemIntVariableNames[i].c_str(), std::to_string(systemIntValDefault[i]).c_str()));
		}
	}
	for (size_t i = 0; i < BOOL_VAR_NUM; i++) {	// save system bools non apereance 
		if (systemBoolValue[i]) {
			*boolValPointers[i] = ini.GetBoolValue("settings", boolVariableNames[i].c_str(), boolValDefault[i]);
		}
	}
	for (size_t i = 0; i < DOUBLE_VAR_NUM; i++) {	// save system doubles non apereance 
		if (systemDoubleValue[i]) {
			*doubleValPointers[i] = ini.GetDoubleValue("settings", doubleVariableNames[i].c_str(), doubleValDefault[i]);
		}
	}
};



int SettingsOBJ::loadSettings(){

	loadSystemSettings();

	CSimpleIniA ini;

	std::string styleDestinatio;

	if (stylescheme == CUSTOM) {
		SI_Error rc = ini.LoadFile("customTheme.ntheme"); // file for custom theme 
		debugLOG(rc);

		if (rc < 0) {
			debugLOG("failed to open file");
			resetSettingsFiles(STYLEFILE); // probìhne ale neprobìhne nvm co to je // nic nevypíše a nedá return ale soubor vytvorí. // ????
			SI_Error rc = ini.LoadFile("customTheme.ntheme");
			if (rc < 0) { debugLOG("master error on opening file");  return SETINGS_FILE_NOT_FOUND; } // probably masterError
		};
		styleDestinatio = "theme";
	}
	else {
		SI_Error rc = ini.LoadFile("appData.ini"); // file for custom theme 
		if (rc < 0) {
			debugLOG("failed to open file");
			resetSettingsFiles(STYLEFILE); // probìhne ale neprobìhne nvm co to je // nic nevypíše a nedá return ale soubor vytvorí. // ????
			SI_Error rc = ini.LoadFile("appData.ini");
			if (rc < 0) { debugLOG("master error on opening file");  return SETINGS_FILE_NOT_FOUND; } // probably masterError
		};
		styleDestinatio = is_light_theme() ? "defaultLightTheme" : "defaultDarkTheme";
		if (stylescheme > AUTO) { // is forsed
			styleDestinatio = (stylescheme == LIGHT) ? "defaultDarkTheme" : "defaultLightTheme";
		}
	}

	// add forsedc 
	debugLOG("loading from : " + styleDestinatio);
	for (size_t i = 0; i < BOOL_VAR_NUM; i++) {
		if (!systemBoolValue[i]) {
			*boolValPointers[i] = ini.GetBoolValue(styleDestinatio.c_str(), boolVariableNames[i].c_str(), boolValDefault[i]);
		}
	}
	for (size_t i = 0; i < DOUBLE_VAR_NUM; i++) {
		if (!systemDoubleValue[i]) {
			*doubleValPointers[i] = ini.GetDoubleValue(styleDestinatio.c_str(), doubleVariableNames[i].c_str(), doubleValDefault[i]);
		}
	}
	for (size_t i = 0; i < TEXT_VAR_NUM; i++) {
		if (!systemTextValue[i]) {
			*textValPointers[i] = ini.GetValue(styleDestinatio.c_str(), textVariableNames[i].c_str(), textValDefault[i].c_str());
		}
	}

	return 0;
};


bool SettingsOBJ::SaveSystemSettings() {
	CSimpleIniA ini;
	ini.SetUnicode();

	SI_Error rc = ini.LoadFile("appData.ini");
	for (size_t i = 0; i < INT_VAR_NUM; i++) {
		if (systemIntValue[i]) {
			ini.SetValue("settings", systemIntVariableNames[i].c_str(), std::to_string(*systemIntValPointers[i]).c_str());
		}
	}
	for (size_t i = 0; i < BOOL_VAR_NUM; i++) {
		if (systemBoolValue[i]) {
			ini.SetBoolValue("settings", boolVariableNames[i].c_str(), *boolValPointers[i]);
		}
	}
	ini.SaveFile("appData.ini");

	return 0;
}


bool SettingsOBJ::SaveThemeSettings() {
	CSimpleIniA ini;
	ini.SetUnicode();

	if (stylescheme == CUSTOM) { // only custom theme can be saved // saves all settings
		for (size_t i = 0; i < BOOL_VAR_NUM; i++) {
			if (!systemBoolValue[i]) {	// IF VARIABLE IS THEME 
				ini.SetBoolValue("theme", boolVariableNames[i].c_str(), *boolValPointers[i]);
			}
		}
		for (size_t i = 0; i < DOUBLE_VAR_NUM; i++) {
			if (!systemDoubleValue[i]) {
				ini.SetDoubleValue("theme", doubleVariableNames[i].c_str(), *doubleValPointers[i]);
			}
		}
		for (size_t i = 0; i < TEXT_VAR_NUM; i++) {
			if (!systemTextValue[i]) {
				ini.SetValue("theme", textVariableNames[i].c_str(), textValPointers[i]->c_str());
			}
		}
		ini.SaveFile("customTheme.ntheme");
		debugLOG("saving CustomTheme");
	}
	else {
		debugLOG("cantSaveNonCustomSettings");
	}

	return 0;
}

bool SettingsOBJ::saveSettings(){
	SaveSystemSettings();
	SaveThemeSettings();
	return 0;
};

bool SettingsOBJ::is_light_theme() {
	// based on https://stackoverflow.com/questions/51334674/how-to-detect-windows-10-light-dark-mode-in-win32-application

	// The value is expected to be a REG_DWORD, which is a signed 32-bit little-endian
	auto buffer = std::vector<char>(4);
	auto cbData = static_cast<DWORD>(buffer.size() * sizeof(char));
	auto res = RegGetValueW(HKEY_CURRENT_USER,L"Software\\Microsoft\\Windows\\CurrentVersion\\Themes\\Personalize",L"AppsUseLightTheme",
		RRF_RT_REG_DWORD /* expected value type*/, nullptr,buffer.data(),&cbData);

	if (res != ERROR_SUCCESS) {
		debugLOG("problem vuth loading light thme!"); return 0;
	}

	// convert bytes written to our buffer to an int, assuming little-endian
	auto i = int(buffer[3] << 24 | buffer[2] << 16 | buffer[1] << 8 | buffer[0]);

	return i == 1;
}