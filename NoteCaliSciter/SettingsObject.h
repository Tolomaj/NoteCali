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

#define ALLOW_LINE_JUMP_DEFAULT false
#define USE_RADIANS_DEFAULT false
#define USE_MATRICS_DEFAULT false
#define IGNORE_HIGHT_DIFERENCE_DEFAULT false
#define PARENTHESIS_CORECTION_DEFAULT false
#define SHOW_ERR_TEXT_DEFAULT false
#define USE_MODIFIERS_DEFAULT false
#define POITER_IS_NOROUND_DEFAULT true



#define BACKGROUND_COLOR_DEFAULT "#d9d9d9"
#define DIVIDER_LINE_COLOR_DEFAULT "#10819A"
#define FONT_COLOR_DEFAULT "#1F2937"
#define SOLUTION_FONT_COLOR_DEFAULT "#0E2235"
#define CLICK_COLOR_DEFAULT "#2F71AF"
#define HOWER_COLOR_DEFAULT "#1C456B"
#define BAR_ICON_DEFAULT "#ffffff"
#define TITLE_DEFAULT_COLOR "#0a1016"


#define TRANSPARENT_MODE_DEFAULT 100
#define FONT_SIZE_DEFAULT 18
#define FONT_PADDING_DEFAULT 2
#define ROUND_DEC_NUM_DEFAULT 4
#define DIVIDER_LINE_POS_DEFAULT 70 

#define USE_SIENCE_DEFAULT false
#define GROUP_DEFAULT 0



//exit codes//
#define SETINGS_FILE_NOT_FOUND 2
#define SETINGS_LOADED_SCUESFUL 0


//exit codes END//

#define DATAFILE 1
#define STYLEFILE 2
#define BOATH 0
#define PRINT_SETINGS_SETT false







class SettingsOBJ {
public:  
	vector<Function> userFunctions;

	int stylescheme = AUTO;
	int roundToDec = ROUND_DEC_NUM_DEFAULT;
	int numberGrouping = GROUP_DEFAULT;
	int dividerLinePos = DIVIDER_LINE_POS_DEFAULT;
	
	bool showAppName = SHOW_APP_NAME_DEFAULT; 
	bool highliteERR = HIGHLITE_ERR_DEFAULT;
	bool highliteVAR = HIGHLITE_VAR_DEFAULT;
	bool highliteSUPER = HIGHLITE_SUPERLINE_DEFAULT;
	bool clickToCopy = CLICK_TO_COPY_DEFAULT;
	bool showLineEnd = SHOW_LINE_END_DEFAULT;
	bool showLineNumbers = SHOW_LINE_NUMBERS_DEFAULT;
	bool isAllLinesSuperlines = ALL_SUPERLINE_DEFAUTL;
	bool countingOnLineEnd = false;
	bool allowLineJump = ALLOW_LINE_JUMP_DEFAULT;
	bool useRadians = USE_RADIANS_DEFAULT;
	bool useMetrics = USE_MATRICS_DEFAULT;
	bool ignoreHightDiference = IGNORE_HIGHT_DIFERENCE_DEFAULT;
	bool corectParenthesis = PARENTHESIS_CORECTION_DEFAULT;
	bool useSientific = USE_SIENCE_DEFAULT;
	bool showErrText = SHOW_ERR_TEXT_DEFAULT;

	bool useLineModifiers = USE_MODIFIERS_DEFAULT;
	bool useNoroundPointers = POITER_IS_NOROUND_DEFAULT;

private:

	#define BOOL_VAR_NUM 18 // idk jak jinak to dìlat ve foru
	bool systemBoolValue[BOOL_VAR_NUM] =		  { true                , true               , true               , true                       , true                  , true                  , false                     , true            	   , true				 , true                    , true               , true               , true                          , true                          ,true              , true                 ,true                  ,true                      };
	bool * boolValPointers[BOOL_VAR_NUM] =        { &showAppName        , &highliteERR       , &highliteVAR       , &highliteSUPER             , &clickToCopy          , &showLineEnd          , &showLineNumbers          , &isAllLinesSuperlines , &countingOnLineEnd  , &allowLineJump          , &useRadians        , &useMetrics        , &ignoreHightDiference         , &corectParenthesis            ,&useSientific     , &showErrText         ,&useLineModifiers     ,&useNoroundPointers       };
	std::string boolVariableNames[BOOL_VAR_NUM] = { "showAppName"       , "highliteERR"      , "highliteVAR"      , "highliteSUPER"            , "clickToCopy"         , "showLineEnd"         , "showLineNumbers"         , "isAllLinesSuperlines", "countingOnLineEnd" , "allowLineJump"         , "useRadians"       , "useMetrics"       , "ignoreHightDiference"        , "corectParenthesis"           ,"useSientific"    , "showErrText"        ,"useLineModifiers"    ,"useNoroundPointers"      };
	bool boolValDefault[BOOL_VAR_NUM] =           {SHOW_APP_NAME_DEFAULT,HIGHLITE_ERR_DEFAULT,HIGHLITE_VAR_DEFAULT, HIGHLITE_SUPERLINE_DEFAULT , CLICK_TO_COPY_DEFAULT , SHOW_LINE_END_DEFAULT , SHOW_LINE_NUMBERS_DEFAULT , ALL_SUPERLINE_DEFAUTL , false               , ALLOW_LINE_JUMP_DEFAULT , USE_RADIANS_DEFAULT, USE_MATRICS_DEFAULT, IGNORE_HIGHT_DIFERENCE_DEFAULT, PARENTHESIS_CORECTION_DEFAULT ,USE_SIENCE_DEFAULT, SHOW_ERR_TEXT_DEFAULT,USE_MODIFIERS_DEFAULT ,POITER_IS_NOROUND_DEFAULT };
	
#define INT_VAR_NUM 4 // idk jak jinak to dìlat ve foru
	bool systemIntValue[INT_VAR_NUM] =                { true         , true		 	        , true            ,true                     };
	int* systemIntValPointers[INT_VAR_NUM] =          { &stylescheme , &roundToDec          , &numberGrouping ,&dividerLinePos          };
	std::string systemIntVariableNames[INT_VAR_NUM] = { "stylescheme", "roundToDec"         , "numberGrouping","dividerLinePos"         };
	int systemIntValDefault[INT_VAR_NUM] =            { AUTO         , ROUND_DEC_NUM_DEFAULT, GROUP_DEFAULT   ,DIVIDER_LINE_POS_DEFAULT };



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
	std::string titleColor = TITLE_DEFAULT_COLOR;
private:

	#define TEXT_VAR_NUM 8 // idk jak jinak to dìlat ve foru
	bool systemTextValue[TEXT_VAR_NUM] =          { false                    , false                      , false              , false                       , false               , false               , false            , false };
	std::string * textValPointers[TEXT_VAR_NUM] = { &backgroudColor          , &dividerLineColor,           &fontColor,          &solutionFontColor,           &clickColor,          &howerColor         , &barIconColor    , &titleColor };
	std::string textVariableNames[TEXT_VAR_NUM] = { "backgroudColor"         , "dividerLineColor",          "fontColor",         "solutionFontColor",          "clickColor",         "howerColor"        , "barIconColor"   , "titleColor"};
	std::string textValDefault[TEXT_VAR_NUM] =    { BACKGROUND_COLOR_DEFAULT , DIVIDER_LINE_COLOR_DEFAULT , FONT_COLOR_DEFAULT , SOLUTION_FONT_COLOR_DEFAULT , CLICK_COLOR_DEFAULT , HOWER_COLOR_DEFAULT , BAR_ICON_DEFAULT , TITLE_DEFAULT_COLOR};




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
	//debugLOG("Setting Not Found by name! Name: " + name);
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
		//debugLOG(systemIntVariableNames[i] + " : " + name);
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
	//debugLOG("Setting Not Found by name! Name: " + name + " ->ALL_TYPES");
	return false;
}



bool SettingsOBJ::setSingleSettingsbyName(std::string name,bool value) {
	for (size_t i = 0; i < BOOL_VAR_NUM; i++) {
		if (boolVariableNames[i] == name) {
			*boolValPointers[i] = value;
			return true;
		}
	}
	//debugLOG("Setting Not Found by name! Name: " + name + " ->BOOL_TYPE");
	return false;
};

bool SettingsOBJ::setSingleSettingsbyName(std::string name, double value) {
	for (size_t i = 0; i < DOUBLE_VAR_NUM; i++) {
		if (doubleVariableNames[i] == name) {
			*doubleValPointers[i] = value;
			return true;
		}
	}
	//debugLOG("Setting Not Found by name! Name: " + name + " ->DOUBLE_TYPE");
	return false;

};

bool SettingsOBJ::setSingleSettingsbyName(std::string name, int value) {
	for (size_t i = 0; i < INT_VAR_NUM; i++) {
		if (systemIntVariableNames[i] == name) {
			*systemIntValPointers[i] = value;
			return true;
		}
	}
	//debugLOG("Setting Not Found by name! Name: " + name + " ->INT_TYPE");
	return false;

};

bool SettingsOBJ::setSingleSettingsbyName(std::string name, std::string value) {
	for (size_t i = 0; i < TEXT_VAR_NUM; i++) {
		if (textVariableNames[i] == name) {
			*textValPointers[i] = value;
			return true;
		}
	}
	//debugLOG("Setting Not Found by name! Name: " + name + " ->STRING_TYPE");
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



bool SettingsOBJ::resetSettingsFiles(int file  = 0) { // DATAFILE,STYLEFILE,BOATH
	//debugLOG("resetingSettings");

	// add folder cretion

	mkdir((getenv("APPDATA") + string("\\NoteCali")).c_str()); // try create folder / if folder exist nothimng hapends

	std::string defaultLightTheme = "showLineNumbers = false\ntransparencity = 100.000000\nfontSize = 18.000000\nfontPadding = 2.000000\nbackgroudColor = #d9d9d9\ndividerLineColor = #10819A\nfontColor = #1F2937\nsolutionFontColor = #0E2235\nclickColor = #2F71AF\nhowerColor = #1C456B\n";
	if (file == 0 || file == DATAFILE) {
		std::string defaultDarkTheme = "showLineNumbers = false\ntransparencity = 100.000000\nbackgroudColor = #262929\ndividerLineColor = #6d8a8a\nfontColor = #919191\nsolutionFontColor = #587575\nclickColor = #2F71AF\nhowerColor = #1C456B\nbarIconColor = #ffffff\ntitleColor = #6f7575\n";

		string appDataPath = getenv("APPDATA") + string("\\NoteCali\\appData.ini");
		std::ofstream appDataFile(appDataPath.c_str());
		if (appDataFile.fail()) {
			appDataFile.close();
			debugLOG("cant crete data file ! MATER ERROR = CLOSING");
			debugERROR(L"Cant find file at" + StrToWstr(appDataPath) + L". And cant file create.\n(You can try crete thys file by hand)");//only one handle
			exit(1);
		}

		appDataFile << "[settings]\nstylescheme = 0\nmathType = 1\nroundToDec = 3\nnumberGrouping = 0\nshowAppName = true\nhighliteERR = true\nhighliteVAR = true\nhighliteSUPER = true\nclickToCopy = true\nshowLineEnd = false\nisAllLinesSuperlines = false\ncountingOnLineEnd = false\nallowLineJump = true\nuseRadians = false\nuseMetrics = false\nignoreHightDiference = true\ncorectParenthesis = false\nuseSientific = false\nshowErrText = false\nfontSize = 18.000000\nfontPadding = 10.000000\n\n[defaultDarkTheme]\n" + defaultLightTheme + "\n[defaultLightTheme]\n" + defaultDarkTheme;
		appDataFile.close();
	}
	if (file == 0 || file == STYLEFILE) {

		string customthemePath = getenv("APPDATA") + string("\\NoteCali\\customTheme.ntheme");
		std::ofstream CustomThemeFile(customthemePath.c_str());
		if (CustomThemeFile.fail()) {
			CustomThemeFile.close();
			debugLOG("cant crete theme file ! MATER ERROR = CLOSING");
			debugERROR(L"Cant find file at" + StrToWstr(customthemePath) + L". And cant file create.\n(You can try crete thys file by hand)");//only one handle

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

	string appDataPath = getenv("APPDATA") + string("\\NoteCali\\appData.ini");

	SI_Error rc = ini.LoadFile(appDataPath.c_str()); // file for system setings settings 

	if (rc < 0) {
		debugLOG("failed to open file. Reseting Settings!");
		resetSettingsFiles(DATAFILE);
		SI_Error rc = ini.LoadFile(appDataPath.c_str());					//try load data second time / default data expected 
		if (rc < 0) { 
			debugLOG("master error on opening file");  
			return SETINGS_FILE_NOT_FOUND; 
		} // probably masterError
	};

	for (size_t i = 0; i < INT_VAR_NUM; i++) {	// load
		if (systemIntValue[i]) {
			*systemIntValPointers[i] = std::stoi(ini.GetValue("settings", systemIntVariableNames[i].c_str(), std::to_string(systemIntValDefault[i]).c_str()));
		}
	}
	for (size_t i = 0; i < BOOL_VAR_NUM; i++) {	// load system bools non apereance 
		if (systemBoolValue[i]) {
			*boolValPointers[i] = ini.GetBoolValue("settings", boolVariableNames[i].c_str(), boolValDefault[i]);
		}
	}
	for (size_t i = 0; i < DOUBLE_VAR_NUM; i++) {	// load system doubles non apereance 
		if (systemDoubleValue[i]) {
			*doubleValPointers[i] = ini.GetDoubleValue("settings", doubleVariableNames[i].c_str(), doubleValDefault[i]);
		}
	}
	return 0;
};



int SettingsOBJ::loadSettings(){

	loadSystemSettings();

	CSimpleIniA ini;

	std::string styleDestinatio;

	if (stylescheme == CUSTOM) {
		string Path = getenv("APPDATA") + string("\\NoteCali\\customTheme.ntheme");
		SI_Error rc = ini.LoadFile(Path.c_str()); // file for custom theme 
		debugLOG(rc);

		if (rc < 0) {
			debugLOG("failed to open file");
			resetSettingsFiles(STYLEFILE); // probìhne ale neprobìhne nvm co to je // nic nevypíše a nedá return ale soubor vytvorí. // ????
			SI_Error rc = ini.LoadFile(Path.c_str());
			if (rc < 0) { debugLOG("master error on opening file");  return SETINGS_FILE_NOT_FOUND; } // probably masterError
		};
		styleDestinatio = "theme";
	}
	else {
		string appDataPath = getenv("APPDATA") + string("\\NoteCali\\appData.ini");

		SI_Error rc = ini.LoadFile(appDataPath.c_str()); // file for custom theme 
		if (rc < 0) {
			debugLOG("failed to open file");
			resetSettingsFiles(STYLEFILE); // probìhne ale neprobìhne nvm co to je // nic nevypíše a nedá return ale soubor vytvorí. // ????
			SI_Error rc = ini.LoadFile(appDataPath.c_str());
			if (rc < 0) { debugLOG("master error on opening file");  return SETINGS_FILE_NOT_FOUND; } // probably masterError
		};
		styleDestinatio = is_light_theme() ? "defaultLightTheme" : "defaultDarkTheme";
		if (stylescheme > AUTO) { // is forsed
			styleDestinatio = (stylescheme == LIGHT) ? "defaultDarkTheme" : "defaultLightTheme";
		}
	}

	// add forsedc 
	//debugLOG("loading from : " + styleDestinatio);
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

	//debugLOG(" -:SavingSystemSettings");
	string appDataPath = getenv("APPDATA") + string("\\NoteCali\\appData.ini");

	SI_Error rc = ini.LoadFile(appDataPath.c_str());
	for (size_t i = 0; i < INT_VAR_NUM; i++) {
		if (systemIntValue[i]) {
			ini.SetValue("settings", systemIntVariableNames[i].c_str(), std::to_string(*systemIntValPointers[i]).c_str());
			#if PRINT_SETINGS_SETT
				debugLOG("  : " + systemIntVariableNames[i] + " - " + to_string(*systemIntValPointers[i]));
			#endif	
		}
	}
	for (size_t i = 0; i < BOOL_VAR_NUM; i++) {
		if (systemBoolValue[i]) {
			ini.SetBoolValue("settings", boolVariableNames[i].c_str(), *boolValPointers[i]);
			#if PRINT_SETINGS_SETT
				debugLOG("  : " + boolVariableNames[i] + " - " + to_string(*boolValPointers[i]));
			#endif
		}
	}
	for (size_t i = 0; i < DOUBLE_VAR_NUM; i++) {
		if (systemDoubleValue[i]) {
			ini.SetDoubleValue("settings", doubleVariableNames[i].c_str(), *doubleValPointers[i]);
			#if PRINT_SETINGS_SETT
				debugLOG("  : " + doubleVariableNames[i] + " - " + to_string(*doubleValPointers[i]));
			#endif	
		}
	}
	for (size_t i = 0; i < TEXT_VAR_NUM; i++) {
		if (systemTextValue[i]) {
			ini.SetValue("theme", textVariableNames[i].c_str(), textValPointers[i]->c_str());
			#if PRINT_SETINGS_SETT
				debugLOG("  : " + textVariableNames[i] + " - " + *textValPointers[i]);
			#endif	
		}
	}

	ini.SaveFile(appDataPath.c_str());

	return 0;
}


bool SettingsOBJ::SaveThemeSettings() {
	CSimpleIniA ini;
	ini.SetUnicode();

	//debugLOG(" -:SavingThemeSettings");
	if (stylescheme == CUSTOM) { // only custom theme can be saved // saves all settings
		for (size_t i = 0; i < INT_VAR_NUM; i++) {
			if (!systemIntValue[i]) {
				ini.SetValue("settings", systemIntVariableNames[i].c_str(), std::to_string(*systemIntValPointers[i]).c_str());
				#if PRINT_SETINGS_SETT
					debugLOG("  : " + systemIntVariableNames[i] + " - " + to_string(*systemIntValPointers[i]));
				#endif	
			}
		}
		for (size_t i = 0; i < BOOL_VAR_NUM; i++) {
			if (!systemBoolValue[i]) {	// IF VARIABLE IS THEME 
				ini.SetBoolValue("theme", boolVariableNames[i].c_str(), *boolValPointers[i]);
				#if PRINT_SETINGS_SETT
					debugLOG("  : " + boolVariableNames[i] + " - " + to_string(*boolValPointers[i]));
				#endif	
			}
		}
		for (size_t i = 0; i < DOUBLE_VAR_NUM; i++) {
			if (!systemDoubleValue[i]) {
				ini.SetDoubleValue("theme", doubleVariableNames[i].c_str(), *doubleValPointers[i]);
				#if PRINT_SETINGS_SETT
					debugLOG("  : " + doubleVariableNames[i] + " - " + to_string(*doubleValPointers[i]));
				#endif	
			}
		}
		for (size_t i = 0; i < TEXT_VAR_NUM; i++) {
			if (!systemTextValue[i]) {
				ini.SetValue("theme", textVariableNames[i].c_str(), textValPointers[i]->c_str());
				#if PRINT_SETINGS_SETT
					debugLOG("  : " + textVariableNames[i] + " - " + *textValPointers[i]);
				#endif	
			}
		}
		string Path = getenv("APPDATA") + string("\\NoteCali\\customTheme.ntheme");
		ini.SaveFile(Path.c_str());
		//debugLOG("saving CustomTheme");
	}
	else {
		debugLOG("cantSaveNonCustomSettings");
	}

	return 0;
}

bool SettingsOBJ::saveSettings(){
	SaveSystemSettings();
	if (stylescheme == CUSTOM) { // aby se nevypisovala chybová hláška
		SaveThemeSettings();
	}
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
		debugLOG("problem vith loading light thme!"); return 0;
	}

	// convert bytes written to our buffer to an int, assuming little-endian
	auto i = int(buffer[3] << 24 | buffer[2] << 16 | buffer[1] << 8 | buffer[0]);

	return i == 1;
}