#include "library/SimpleIni.h" // https://github.com/pulzed/mINI
#include <fstream>


#define SHOW_APP_NAME_DEFAULT true
#define HIGHLITE_ERR_DEFAULT true
#define HIGHLITE_VAR_DEFAULT true
#define HIGHLITE_SUPERLINE_DEFAULT true
#define CLICK_TO_COPY_DEFAULT true
#define SHOW_LINE_NUMBERS_DEFAULT false
#define ALL_SUPERLINE_DEFAUTL false

#define BACKGROUND_COLOR_DEFAULT "#d9d9d9"
#define DIVIDER_LINE_COLOR_DEFAULT "#10819A"
#define FONT_COLOR_DEFAULT "#1F2937"
#define SOLUTION_FONT_COLOR_DEFAULT "#0E2235"
#define CLICK_COLOR_DEFAULT "#2F71AF"
#define HOWER_COLOR_DEFAULT "#1C456B"


#if DEBUG
	#define SHOW_LINE_END_DEFAULT true
#else
	#define SHOW_LINE_END_DEFAULT false
#endif

#define TRANSPARENT_MODE_DEFAULT 100
#define FONT_SIZE_DEFAULT 18
#define FONT_PADDING_DEFAULT 2



//exit codes//
#define SETINGS_FILE_NOT_FOUND 2
#define SETINGS_LOADED_SCUESFUL 0


//exit codes END//


#define CUSTOM 3
#define DARK 2
#define LIGHT 1
#define AUTO 0

#define TYPE_TEXTMATH 0
#define TYPE_TREEMATH 1


class SettingsOBJ {
public:  
	int stylescheme = CUSTOM;
	int mathType = TYPE_TREEMATH;
	
	bool showAppName = SHOW_APP_NAME_DEFAULT; 
	bool highliteERR = HIGHLITE_ERR_DEFAULT;
	bool highliteVAR = HIGHLITE_VAR_DEFAULT;
	bool highliteSUPER = HIGHLITE_SUPERLINE_DEFAULT;
	bool clickToCopy = CLICK_TO_COPY_DEFAULT;
	bool showLineEnd = SHOW_LINE_END_DEFAULT;
	bool showLineNumbers = SHOW_LINE_NUMBERS_DEFAULT;
	bool isAllLinesSuperlines = ALL_SUPERLINE_DEFAUTL;
	bool countingOnLineEnd = false;

private:
	#define BOOL_VAR_NUM 9 // idk jak jinak to dìlat ve foru
	bool * boolValPointers[BOOL_VAR_NUM] =        { &showAppName        , &highliteERR       , &highliteVAR       , &highliteSUPER             , &clickToCopy          , &showLineEnd          , &showLineNumbers          , &isAllLinesSuperlines , &countingOnLineEnd  };
	std::string boolVariableNames[BOOL_VAR_NUM] = { "showAppName"       , "highliteERR"      , "highliteVAR"      , "highliteSUPER"            , "clickToCopy"         , "showLineEnd"         , "showLineNumbers"         , "isAllLinesSuperlines", "countingOnLineEnd" };
	bool boolValDefault[BOOL_VAR_NUM] =           {SHOW_APP_NAME_DEFAULT,HIGHLITE_ERR_DEFAULT,HIGHLITE_VAR_DEFAULT, HIGHLITE_SUPERLINE_DEFAULT , CLICK_TO_COPY_DEFAULT , SHOW_LINE_END_DEFAULT , SHOW_LINE_NUMBERS_DEFAULT , ALL_SUPERLINE_DEFAUTL , false               };

	#define SYSTEM_INT_VAR_NUM 2 // idk jak jinak to dìlat ve foru
	int* systemIntValPointers[SYSTEM_INT_VAR_NUM] =          { &stylescheme , &mathType };
	std::string systemIntVariableNames[SYSTEM_INT_VAR_NUM] = { "stylescheme", "mathType"     };
	int systemIntValDefault[SYSTEM_INT_VAR_NUM] =            { AUTO         , TYPE_TREEMATH };



public:
	double transparencity = TRANSPARENT_MODE_DEFAULT;
	double fontSize = FONT_SIZE_DEFAULT;
	double fontPadding = FONT_PADDING_DEFAULT;

private:
	#define DOUBLE_VAR_NUM 3 // idk jak jinak to dìlat ve foru
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
private:

	/*
		std::string backgroudColor = ;
	*/

	#define TEXT_VAR_NUM 6 // idk jak jinak to dìlat ve foru
	std::string * textValPointers[TEXT_VAR_NUM] = { &backgroudColor          , &dividerLineColor, &fontColor, &solutionFontColor, &clickColor, &howerColor };
	std::string textVariableNames[TEXT_VAR_NUM] = { "backgroudColor"         , "dividerLineColor", "fontColor", "solutionFontColor", "clickColor", "howerColor"};
	std::string textValDefault[TEXT_VAR_NUM] =    { BACKGROUND_COLOR_DEFAULT , DIVIDER_LINE_COLOR_DEFAULT ,FONT_COLOR_DEFAULT,SOLUTION_FONT_COLOR_DEFAULT,CLICK_COLOR_DEFAULT,HOWER_COLOR_DEFAULT };




public:



	bool ishighlitingOn() {
		return highliteERR || highliteVAR;
	}
	



	int loadSettings();

	bool saveSettings();

	bool resetSettingsToDefault();

	bool resetSettingsFiles(int file);

	bool setSingleSettingsbyName(std::string name, bool value);
	bool setSingleSettingsbyName(std::string name, double value);
	bool setSingleSettingsbyName(std::string name, int value);
	bool setSingleSettingsbyName(std::string name, std::string value);

	bool setSetting(std::string name, std::string value);


	bool is_light_theme();

}settings;




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
	for (size_t i = 0; i < SYSTEM_INT_VAR_NUM; i++) {
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
	for (size_t i = 0; i < SYSTEM_INT_VAR_NUM; i++) {
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
	for (size_t i = 0; i < SYSTEM_INT_VAR_NUM; i++) {
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
	
	std::string defaultLightTheme = "showAppName = true\nhighliteERR = true\nhighliteVAR = true\nhighliteSUPER = true\nclickToCopy = true\nshowLineEnd = true\nshowLineNumbers = false\nisAllLinesSuperlines = false\ncountingOnLineEnd = false\ntransparencity = 100.000000\nfontSize = 18.000000\nfontPadding = 2.000000\n";
	if (file == 0 || file == DATAFILE) {
		std::string defaultDarkTheme = "showAppName = true\nhighliteERR = true\nhighliteVAR = true\nhighliteSUPER = true\nclickToCopy = true\nshowLineEnd = true\nshowLineNumbers = false\nisAllLinesSuperlines = false\ncountingOnLineEnd = false\ntransparencity = 100.000000\nfontSize = 18.000000\nfontPadding = 2.000000\n";
		std::ofstream appDataFile("appData.ini");
		if (appDataFile.fail()) {
			appDataFile.close();
			debugLOG("cant crete data file ! MATER ERROR = CLOSING");
			exit(1);
		}

		appDataFile << "[settings]\nstylescheme = 0\nmathType = 1\n\n[defaultDarkTheme]\n" + defaultLightTheme + "\n[defaultLightTheme]\n" + defaultDarkTheme;
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

bool is_file_exist(const char* fileName)
{
	std::ifstream infile(fileName);
	return infile.good();
}

int SettingsOBJ::loadSettings(){
	CSimpleIniA ini;
	ini.SetUnicode();

	SI_Error rc = ini.LoadFile("appData.ini");
	debugLOG(rc);

	if (rc < 0) {
		debugLOG("failed to open file. Reseting Settings!");
		resetSettingsFiles(DATAFILE);
		SI_Error rc = ini.LoadFile("appData.ini");
		if (rc < 0) { debugLOG("master error on opening file");  return SETINGS_FILE_NOT_FOUND ; } // probably masterError
	};

	for (size_t i = 0; i < SYSTEM_INT_VAR_NUM; i++) {
		*systemIntValPointers[i] = std::stoi(ini.GetValue("settings", systemIntVariableNames[i].c_str(), std::to_string(systemIntValDefault[i]).c_str()));
		debugLOG(*systemIntValPointers[i]);
	}

	std::string styleDestinatio;

	if (stylescheme == CUSTOM) {
		rc = ini.LoadFile("customTheme.ntheme");
		debugLOG(rc);

		if (rc < 0) {
			debugLOG("failed to open file");
			resetSettingsFiles(STYLEFILE); // probìhne ale neprobìhne nvm co to je // nic nevypíše a nedá return ale soubor vytvorí. // ????
			SI_Error rc = ini.LoadFile("appData.ini");
			if (rc < 0) { debugLOG("master error on opening file");  return SETINGS_FILE_NOT_FOUND; } // probably masterError
		};
		styleDestinatio = "theme";
	}
	else {
		styleDestinatio = is_light_theme() ? "defaultDarkTheme" : "defaultLightTheme";
	}

	for (size_t i = 0; i < BOOL_VAR_NUM; i++) {
		*boolValPointers[i] = ini.GetBoolValue(styleDestinatio.c_str(), boolVariableNames[i].c_str(), boolValDefault[i]);
	}
	for (size_t i = 0; i < DOUBLE_VAR_NUM; i++) {
		*doubleValPointers[i] = ini.GetDoubleValue(styleDestinatio.c_str(), doubleVariableNames[i].c_str(), doubleValDefault[i]);
	}
	for (size_t i = 0; i < TEXT_VAR_NUM; i++) {
		*textValPointers[i] = ini.GetValue(styleDestinatio.c_str(), textVariableNames[i].c_str(), textValDefault[i].c_str());
	}

	return 0;
};

bool SettingsOBJ::saveSettings(){
	CSimpleIniA ini;
	ini.SetUnicode();


	for (size_t i = 0; i < SYSTEM_INT_VAR_NUM; i++) {
		ini.SetValue("settings", systemIntVariableNames[i].c_str(), std::to_string(*systemIntValPointers[i]).c_str());
	}
	ini.SaveFile("appData.ini");

	CSimpleIniA ini2;
	ini2.SetUnicode();

	if (stylescheme == CUSTOM) { // only custom theme can be saved
		for (size_t i = 0; i < BOOL_VAR_NUM; i++) {
			ini2.SetBoolValue("theme", boolVariableNames[i].c_str(), *boolValPointers[i]);
		}
		for (size_t i = 0; i < DOUBLE_VAR_NUM; i++) {
			ini2.SetDoubleValue("theme", doubleVariableNames[i].c_str(), *doubleValPointers[i]);
		}
		ini2.SaveFile("customTheme.ntheme");
		debugLOG("ssetings");
	}
	
	//add new values
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