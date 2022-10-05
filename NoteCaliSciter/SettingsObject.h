#include "library/ini.h" // https://github.com/pulzed/mINI

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

}settings;

bool Settings::loadSettings(){
	ini::File file = ini::open("settings.ini");

	file.add_section("appSettings");
	file["appSettings"]["is_generic_key"] = "true";

	file.write("settings.ini");

};

bool Settings::saveSettings(){

};