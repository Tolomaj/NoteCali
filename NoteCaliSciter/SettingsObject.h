#include "library/SimpleIni.h" // https://github.com/pulzed/mINI

class Settings {

public: 
	bool showAppName = true;
	bool highliteERR = true;
	bool highliteVAR = true;
	bool showLineEnd = true;
	int fontSize = 18;
	int fontPadding = 2;
	bool clickToCopy = true;
	 
	 
	bool ishighlitingOn() {
		return highliteERR || highliteVAR;
	}
	

	std::string backgroudColor = "";
	std::string dividerLineColor = "";

	bool loadSettings();

	bool saveSettings();

	bool resetSettingsToDefault();

}settings;


bool Settings::resetSettingsToDefault() {
	CSimpleIniA ini;
	ini.SetUnicode();

	ini.SetValue("appSetings", "key", "true");
	ini.SetBoolValue("settings", "showAppName", true);

	ini.SaveFile("appData.ini");
	//strlen();



	return 0;
};

bool Settings::loadSettings(){
	CSimpleIniA ini;
	ini.SetUnicode();

	SI_Error rc = ini.LoadFile("appData.ini");
	if (rc < 0) { 
		debugLOG("failed to open");
		resetSettingsToDefault();
	};

	
	ini.SetValue("appSetings", "key", "true");
	ini.SetBoolValue("settings", "showAppName", true);

	const char* pv = ini.GetValue("section", "key", "default");


	debugLOG(std::string(pv));
	ini.SaveFile("appData.ini");
	//strlen();

	

	return 0;
};

bool Settings::saveSettings(){
	return 0;
};