class Controler; // idk haw to run .h and .cpp type of structure
class CalculatrWin; // idk haw to run .h and .cpp type of structure
class SettingsWin; // idk haw to run .h and .cpp type of structure


class Controler {
	//SettingsOBJ settings;
	sciter::om::hasset<CalculatrWin> calculatorWin;// = new CalculatrWin();
	sciter::om::hasset<SettingsWin> settingsWin;

	int CalculateValidPositionX();

	int CalculateValidPositionY();


public:
	
	int toggleSettings();

	int doCommand(std::wstring cmd);

	int start();




};


#include "resources.cpp" // resources packaged into binary blob.
#include "settWin.h"
#include "calcWin.h"

int Controler::doCommand(std::wstring cmd) {
	std::wstring func;
	std::wstring value;

	if (cmd.substr(0,7) == L"setset ") {
		int i = 7;
		for (1; i < cmd.length(); i++){
			if (cmd.at(i) == ' ' || cmd.at(i) == '\n') { i++; break; }
			func += cmd.at(i);
			debugLOG(func);
		}
		for (1; i < cmd.length(); i++) {
			if (cmd.at(i) == ' ' || cmd.at(i) == '\n') { break; }
			value += cmd.at(i);
		}
		const std::string s(func.begin(), func.end());
		settings.setSetting(s,value);
		debugLOG(func + L" -o- " + value);
	}

	if (cmd.substr(0, 7) == L"saveset") { settings.saveSettings(); }

	if (cmd.substr(0, 7) == L"applset") { calculatorWin->updateStyles(); }
	
	debugLOG("s");
	return 1;
};

int Controler::start() {
	settings.loadSettings();

	sciter::archive::instance().open(aux::elements_of(resources));
	calculatorWin = new CalculatrWin(this);
	calculatorWin->load(L"this://app/calculator.htm");
	calculatorWin->expand();
	calculatorWin->updateSettings();
	calculatorWin->updateStyles();

	return 1;
};
int Controler::toggleSettings() {
	if (settingsWin != nullptr && settingsWin->is_valid()) {
		settingsWin->close();
	}
	else {
		settingsWin = new SettingsWin(this);
		settingsWin->load(L"this://app/settings.htm");
		SetWindowPos(settingsWin->get_hwnd(), 0, CalculateValidPositionX(), CalculateValidPositionY(), SETTINGS_WIN_WIDTH, SETTINGS_WIN_HEIGHT, SW_POPUP | SW_ENABLE_DEBUG);
		settingsWin->expand();
		//settingsWin->updateSettings();
	}
	return 0;
}


int Controler::CalculateValidPositionX() {
	RECT rect;
	GetWindowRect(calculatorWin->get_hwnd(), &rect);
	bool isLeftSideBigger = rect.left > SETTINGS_WIN_WIDTH + WIN_MARGIN;
	bool winFits = rect.left > SETTINGS_WIN_WIDTH + WIN_MARGIN || MONITOR_WIDTH - rect.right > SETTINGS_WIN_WIDTH + WIN_MARGIN;
	if (winFits) {
		return (rect.left + rect.right - SETTINGS_WIN_WIDTH) / 2 + ((rect.right - rect.left + SETTINGS_WIN_WIDTH) / 2 + WIN_MARGIN) * (isLeftSideBigger ? -1 : 1);
	}
	else {
		return (MONITOR_WIDTH - SETTINGS_WIN_WIDTH) / 2;
	}
}


int Controler::CalculateValidPositionY() {
	RECT rect;
	GetWindowRect(calculatorWin->get_hwnd(), &rect);
	int position = (rect.top + rect.bottom) / 2 - SETTINGS_WIN_HEIGHT / 2;
	if (position < 0) { position = 0; }
	if (position > MONITOR_HEIGHT - SETTINGS_WIN_HEIGHT) { position = MONITOR_HEIGHT - SETTINGS_WIN_HEIGHT; }
	return position;
}