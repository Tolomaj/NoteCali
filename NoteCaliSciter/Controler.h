#include "ObjectDefinitions.h"
#include "SettingsObject.h"
//#include "mLines.h"

#include "variableLoader.h"
#include "calcualtionAlg.h"
#include "LineSeparator.h"

#include "netUpdater.h"

#define NOT_CMD 0
#define LINE_NULLING_CMD 1
#define LINE_NOT_NULLING_CMD 2
#define LINE_NEEDS_REPROCESS 5
#define LINE_NOT_NULLING_NEEDS_REPROCESS 6
#define LINE_WITH_RESPONSE 8

#define LINE_ENDING_CALCULATION_PROCESS 25
//#define LINE_CALC_PROCES_STOP_AND_NULLING 26

#define RELOAD_FROM_FILE true
#define DONT_RELOAD_ON_FILE false
#define SAVE_SYTEM_ONLY_FILE 2
#define SAVE_FILE 1
#define DONT_SAVE_FILE 0

#define RELOAD_ALL 3
#define RELOAD_SETTINGS 1
#define RELOAD_CALCULATOR 0

class Controler; // idk haw to run .h and .cpp type of structure
class CalculatrWin; // idk haw to run .h and .cpp type of structure
class SettingsWin; // idk haw to run .h and .cpp type of structure

class Controler {
	//SettingsOBJ settings;
	sciter::om::hasset<CalculatrWin> calculatorWin;// = new CalculatrWin();
	sciter::om::hasset<SettingsWin> settingsWin;
	LineRegister lineSeparator;
	MatematicalSolver mathSolver;

	int CalculateValidPositionX();

	int CalculateValidPositionY();

public:
	
	int toggleSettings();

	int doCommandLine(mline * cmdLine);

	void reloadVariables();

	int start();

	int procesChangedInput(std::wstring);

	void processSettingsChange(bool reLoadFromFile = DONT_RELOAD_ON_FILE, int SaveFile = SAVE_FILE); //reload data from config files. is needed if is chaget Dark,Auto,Custom

	void reloadWIN(int win); //reload data from config files. is needed if is chaget Dark,Auto,Custom


};


#include "resources.cpp" // resources packaged into binary blob.
#include "settWin.h"
#include "calcWin.h"

void Controler::reloadVariables() {
	variableTable.loadVariables(); // load varables to table
	mathSolver.loadVariablesFromTable(); // loaded into table
	calculatorWin->reCalculateInput();
}

int Controler::start() {
	debugLOG("Starting Program..");
	settings.loadSettings();
	variableTable.loadVariables();

	sciter::archive::instance().open(aux::elements_of(resources));
	calculatorWin = new CalculatrWin(this, &lineSeparator);
	calculatorWin->load(L"this://app/calculator.htm");
	calculatorWin->expand();
	calculatorWin->updateStyles();

	mathSolver.begin();

	if (settings.loadVersionStatusOn == ON_CALCULATOR_OPEN) {
		settings.isActual = checkForUpdate();
		if (settings.isActual == IS_NOT_ACTUAL) {
			calculatorWin->showNotification(L"ON WEB IS NEW VERSION 🥳", COLOR_WARNING);
		}
	}

	debugLOG("Program Sucesfuly Started.");
	debugLOG("");
	return 1;
};


int Controler::doCommandLine(mline * cmdLine) { // find and execute system comand from text  // if comand is not recognized is ignered becouse can modify line calculation process
	cmdLine->isComandDone = true;

	if (cmdLine->command == L"clear" ) { calculatorWin->setText(L""); return LINE_ENDING_CALCULATION_PROCESS; }
	else if (cmdLine->command == L"hello") { cmdLine->command = L"";  cmdLine->line = L"Hi!";  return LINE_WITH_RESPONSE; }
#if DEBUG
	else if (cmdLine->command == L"setset" && cmdLine->isEnded == true) {
		int i = 0;
		wstring cmd = cmdLine->line;
		wstring func = L"";
		wstring value = L"";
		for (1; i < cmd.length(); i++) {
			if (cmd.at(i) == ' ' || cmd.at(i) == '\n') { i++; break; }
			func += cmd.at(i);
		}
		for (1; i < cmd.length(); i++) {
			if (cmd.at(i) == ' ' || cmd.at(i) == '\n') { break; }
			value += cmd.at(i);
		}
		const std::string s(func.begin(), func.end());
		const std::string o(value.begin(), value.end());

		if( !settings.setSetting(s, o) ){
			cmdLine->command = L"";
			cmdLine->line = L"setingsSotFound!";
			return LINE_WITH_RESPONSE;
		}
	}
	else if (cmdLine->command == L"saveset" )                      { settings.saveSettings();       }
	else if (cmdLine->command == L"applset")                       { calculatorWin->updateStyles(); }
	else if (cmdLine->command == L"test" && cmdLine->line == L"2") { cmdLine->solution = L"solus";                             return LINE_NOT_NULLING_CMD; }
#endif
	else { cmdLine->isComandDone = false; return NOT_CMD; } // if not any comad works
	return LINE_NULLING_CMD;
};


int Controler::procesChangedInput(std::wstring dta) {
	debugLOG("\n>> Starting Processing Text Input <<");
	lineSeparator.procesInput(&dta);
	bool refreshAfterCmd = false;
	for (size_t i = 0; i < lineSeparator.lines.size(); i++) {
		if (lineSeparator.lines.at(i).command != L"") { // když obsahuje nejaký příkaz zavolá doCommand aby ho popřípadě zpustil
			int linePostOperationID = doCommandLine(&lineSeparator.lines.at(i));
			if (linePostOperationID == LINE_NULLING_CMD) { // remove comand line and must refresh text area
				lineSeparator.lines.erase(lineSeparator.lines.begin() + i);
				i--;
				refreshAfterCmd = true;
			}
			else if (linePostOperationID == LINE_WITH_RESPONSE) {  refreshAfterCmd = true; } // text area must be rerfreshed
			else if (linePostOperationID == LINE_ENDING_CALCULATION_PROCESS) { return 0; } // comand ends text solving
		}
	}
	if (refreshAfterCmd) {
		calculatorWin->setText(compositeLines(&lineSeparator.lines));
	}
	
	mathSolver.solve(&lineSeparator.lines);

	calculatorWin->publish(lineSeparator.lines);
	debugLOG(">> Text input Procesed <<\n");
	return 0;
};


int Controler::toggleSettings() {
	if (settingsWin != nullptr && settingsWin->is_valid()) {
		debugLOG("Closing Setting Window.");
		settingsWin->close();
	} else {
		debugLOG("Opening Setting Window.");
		settingsWin = new SettingsWin(this);
		settingsWin->load(L"this://app/settings.htm");
		SetWindowPos(settingsWin->get_hwnd(), 0, CalculateValidPositionX(), CalculateValidPositionY(), SETTINGS_WIN_WIDTH, SETTINGS_WIN_HEIGHT, SW_POPUP);
		settingsWin->expand();
		//settingsWin->updateStyles();

		if (settings.loadVersionStatusOn == ON_SETTINGS_OPEN) {
			debugLOG("strtactual*******************************************");
			settings.isActual = checkForUpdate();
			debugLOG("strtactual ende -----------------------------------");
		}

		settingsWin->loadSettingsInWindow();
		debugLOG("Setting Window Scuesfuly loaded.");
	}
	return 0;
}


int Controler::CalculateValidPositionX() { // vypočítá vaidní pozici pro okno nastavení v ose X
	RECT rect;
	GetWindowRect(calculatorWin->get_hwnd(), &rect);
	bool isLeftSideBigger = rect.left > SETTINGS_WIN_WIDTH + WIN_MARGIN;
	bool winFits = rect.left > SETTINGS_WIN_WIDTH + WIN_MARGIN || MONITOR_WIDTH - rect.right > SETTINGS_WIN_WIDTH + WIN_MARGIN;
	if (winFits) {
		return (rect.left + rect.right - SETTINGS_WIN_WIDTH) / 2 + ((rect.right - rect.left + SETTINGS_WIN_WIDTH) / 2 + WIN_MARGIN) * (isLeftSideBigger ? -1 : 1);
	} else {
		return (MONITOR_WIDTH - SETTINGS_WIN_WIDTH) / 2;
	}
}


int Controler::CalculateValidPositionY() { // vypočítá vaidní pozici pro okno nastavení v ose Y
	RECT rect;
	GetWindowRect(calculatorWin->get_hwnd(), &rect);
	int position = (rect.top + rect.bottom) / 2 - SETTINGS_WIN_HEIGHT / 2;
	if (position < 0) { position = 0; }
	if (position > MONITOR_HEIGHT - SETTINGS_WIN_HEIGHT) { position = MONITOR_HEIGHT - SETTINGS_WIN_HEIGHT; }
	return position;
}


void Controler::processSettingsChange(bool reLoadFromFile , int SaveFile) { //reload data from config files. is needed if is chaget Dark,Auto,Custom // and reload styles
	debugLOG("Procesing Settings Change..");
	if (SaveFile == SAVE_FILE) {
		settings.saveSettings();
	}else if (SaveFile == SAVE_SYTEM_ONLY_FILE) {
		settings.SaveSystemSettings();
	}
	if (reLoadFromFile) {
		settings.loadSettings();
	}

	calculatorWin->updateStyles();
	settingsWin->updateStyles();
}
