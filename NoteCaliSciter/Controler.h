#include "SettingsObject.h"
#include "mLines.h"

#include "calcualtionAlg.h"
#include "LineSeparator.h"

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

	int doCommand(std::wstring cmd);

	int doCommandLine(mline * cmdLine);

	int start();

	int procesChangedInput(std::wstring);

};


#include "resources.cpp" // resources packaged into binary blob.
#include "settWin.h"
#include "calcWin.h"

int Controler::doCommand(std::wstring cmd) {
	std::wstring func;
	std::wstring value;

	debugLOG(cmd);

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
		const std::string o(value.begin(), value.end());
		settings.setSetting(s,o);
		debugLOG(func + L" -o- " + value);
	}

	if (cmd.substr(0, 7) == L"saveset") { settings.saveSettings(); }

	if (cmd.substr(0, 7) == L"applset") { calculatorWin->updateStyles(); }

	if (cmd.substr(0, 6) == L"test 1") { calculatorWin->setText(L"Hello World"); }
	
	
	return 1;
};

#define NOT_CMD 0
#define LINE_NULLING_CMD 1
#define LINE_NOT_NULLING_CMD 2
#define LINE_NEEDS_REPROCESS 5
#define LINE_NOT_NULLING_NEEDS_REPROCESS 6
#define LINE_WITH_RESPONSE 8

#define LINE_ENDING_CALCULATION_PROCESS 25
//#define LINE_CALC_PROCES_STOP_AND_NULLING 26
 
int Controler::doCommandLine(mline * cmdLine) {
	debugLOG("cmdTests");
	cmdLine->isComandDone = true;


	if (cmdLine->lineModifier == L"setset" && cmdLine->isEnded == true) {
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
			cmdLine->lineModifier = L"";
			cmdLine->line = L"setingsSotFound!";
			return LINE_WITH_RESPONSE;
		}
		
	} else if (cmdLine->lineModifier == L"saveset" ) { settings.saveSettings(); }

	else if (cmdLine->lineModifier == L"applset") { calculatorWin->updateStyles(); }

	else if (cmdLine->lineModifier == L"test" && cmdLine->line == L"1") {
		cmdLine->lineModifier = L"";
		cmdLine->line = L"Helo World!";
		return LINE_WITH_RESPONSE;
	}
	else if (cmdLine->lineModifier == L"test" && cmdLine->line == L"2") {
		cmdLine->solution = L"solus";
		return LINE_NOT_NULLING_CMD;
	}
	else if (cmdLine->lineModifier == L"clear") { calculatorWin->setText(L""); return LINE_ENDING_CALCULATION_PROCESS; }

	else { cmdLine->isComandDone = false; return NOT_CMD; } // if not any comad works
	return LINE_NULLING_CMD;
};


int Controler::procesChangedInput(std::wstring dta) {
	lineSeparator.procesInput(&dta);
	bool refreshAfterCmd = false;
	for (size_t i = 0; i < lineSeparator.lines.size(); i++) {
		if (lineSeparator.lines.at(i).lineModifier != L"") {
			int linePostOperationID = doCommandLine(&lineSeparator.lines.at(i));
			if (linePostOperationID == LINE_NULLING_CMD) {
				lineSeparator.lines.erase(lineSeparator.lines.begin() + i);
				i--;
				refreshAfterCmd = true;
			}
			else if (linePostOperationID == LINE_WITH_RESPONSE) {  refreshAfterCmd = true; }
			else if (linePostOperationID == LINE_ENDING_CALCULATION_PROCESS) { return 0; }
		}
	}
	if (refreshAfterCmd) {
		calculatorWin->setText(compositeLines(&lineSeparator.lines));
	}
	
	mathSolver.solve(&lineSeparator.lines);

	calculatorWin->publish(lineSeparator.lines);

	return 0;
};

int Controler::start() {
	settings.loadSettings();

	sciter::archive::instance().open(aux::elements_of(resources));
	calculatorWin = new CalculatrWin(this,&lineSeparator);
	calculatorWin->load(L"this://app/calculator.htm");
	calculatorWin->expand();
	calculatorWin->updateStyles();

	/* load variables */
	settings.globalVariables.push_back(Variable(L"pi", 3.14159265359));
	settings.globalVariables.push_back(Variable(L"PI", 3.14159265359));

	/* load variables */

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