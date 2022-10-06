#include "library/SimpleIni.h" // https://github.com/pulzed/mINI
#include <fstream>


#define SHOW_APP_NAME_DEFAULT true
#define HIGHLITE_ERR_DEFAULT true
#define HIGHLITE_VAR_DEFAULT true
#define CLICK_TO_COPY_DEFAULT true

#if DEBUG
	#define SHOW_LINE_END_DEFAULT true
#else
	#define SHOW_LINE_END_DEFAULT false
#endif

#define FONT_SIZE_DEFAULT 18
#define FONT_PADDING_DEFAULT 2



//exit codes//
#define SETINGS_FILE_NOT_FOUND 2
#define SETINGS_LOADED_SCUESFUL 0


//exit codes END//




class Settings {

public: 
	bool showAppName = SHOW_APP_NAME_DEFAULT;
	bool highliteERR = HIGHLITE_ERR_DEFAULT;
	bool highliteVAR = HIGHLITE_VAR_DEFAULT;
	bool clickToCopy = CLICK_TO_COPY_DEFAULT;
	bool showLineEnd = SHOW_LINE_END_DEFAULT;
	int fontSize = FONT_SIZE_DEFAULT;
	int fontPadding = FONT_PADDING_DEFAULT;
	 
	 
	bool ishighlitingOn() {
		return highliteERR || highliteVAR;
	}
	

	std::string backgroudColor = "";
	std::string dividerLineColor = "";

	int loadSettings();

	bool saveSettings();

	bool resetSettingsToDefault();


}settings;


bool Settings::resetSettingsToDefault() {
	CSimpleIniA ini;
	ini.SetUnicode();

	ini.SetBoolValue("settings", "showAppName", SHOW_APP_NAME_DEFAULT);
	ini.SetBoolValue("settings", "highliteERR", HIGHLITE_ERR_DEFAULT);
	ini.SetBoolValue("settings", "highliteVAR", HIGHLITE_VAR_DEFAULT);
	ini.SetBoolValue("settings", "clickToCopy", CLICK_TO_COPY_DEFAULT);
	ini.SetBoolValue("settings", "showLineEnd", SHOW_LINE_END_DEFAULT);

	ini.SetDoubleValue("settings","fontSize", FONT_SIZE_DEFAULT);
	ini.SetDoubleValue("settings", "fontPadding", FONT_PADDING_DEFAULT);

	ini.SaveFile("appData.ini");

	return 0;
};

bool is_file_exist(const char* fileName)
{
	std::ifstream infile(fileName);
	return infile.good();
}

int Settings::loadSettings(){
	CSimpleIniA ini;
	ini.SetUnicode();

	SI_Error rc = ini.LoadFile("appData.ini");
	debugLOG(rc);


	if (rc < 0) {
		debugLOG("failed to open file");
		resetSettingsToDefault(); // probìhne ale neprobìhne nvm co to je // nic nevypíše a nedá return ale soubor vytvorí. // ????
		return SETINGS_FILE_NOT_FOUND;
	};

	bool showAppName = ini.GetBoolValue("settings", "showAppName", SHOW_APP_NAME_DEFAULT);
	bool highliteERR = ini.GetBoolValue("settings", "highliteERR", HIGHLITE_ERR_DEFAULT);
	bool highliteVAR = ini.GetBoolValue("settings", "highliteVAR", HIGHLITE_VAR_DEFAULT);
	bool clickToCopy = ini.GetBoolValue("settings", "clickToCopy", CLICK_TO_COPY_DEFAULT);
	bool showLineEnd = ini.GetBoolValue("settings", "showLineEnd", SHOW_LINE_END_DEFAULT);
	int fontSize = ini.GetDoubleValue("settings", "fontSize", FONT_SIZE_DEFAULT);
	int fontPadding = ini.GetDoubleValue("settings", "fontPadding", FONT_PADDING_DEFAULT);;
	

	return 0;
};

bool Settings::saveSettings(){
	CSimpleIniA ini;
	ini.SetUnicode();

	ini.SetBoolValue("settings", "showAppName", showAppName);
	ini.SetBoolValue("settings", "highliteERR", highliteERR);
	ini.SetBoolValue("settings", "highliteVAR", highliteVAR);
	ini.SetBoolValue("settings", "clickToCopy", clickToCopy);
	ini.SetBoolValue("settings", "showLineEnd", showLineEnd);

	ini.SetDoubleValue("settings", "fontSize", fontSize);
	ini.SetDoubleValue("settings", "fontPadding", fontPadding);

	ini.SaveFile("appData.ini");

	return 0;
};