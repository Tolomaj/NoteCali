#include <Windows.h>
#include <string>
#include <sstream> 

#pragma once


#define BLACK 0
#define DARKBLUE FOREGROUND_BLUE
#define DARKGREEN FOREGROUND_GREEN
#define DARKCYAN FOREGROUND_GREEN | FOREGROUND_BLUE
#define DARKRED FOREGROUND_RED
#define DARKMAGENTA FOREGROUND_RED | FOREGROUND_BLUE
#define DARKYELLOW FOREGROUND_RED | FOREGROUND_GREEN
#define DARKGRAY FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE
#define GRAY FOREGROUND_INTENSITY
#define BLUE FOREGROUND_INTENSITY | FOREGROUND_BLUE
#define GREEN FOREGROUND_INTENSITY | FOREGROUND_GREEN
#define CYAN FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_BLUE
#define RED FOREGROUND_INTENSITY | FOREGROUND_RED
#define MAGENTA FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_BLUE
#define YELLOW FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN
#define WHITE FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE


//add print error (opens window that show error hlasku )

void debugERROR(std::wstring a) { 
	MessageBox(NULL, a.c_str(), L"Fatal Error", MB_ICONWARNING | MB_OK);
}

std::string sciterStrToStr(sciter::string s){
	return std::string(s.begin(), s.end());
}

std::wstring sciterStrToWStr(sciter::string s){
	return std::wstring(s.begin(), s.end());
}

sciter::string WStrToSciterStr(std::wstring s) {
	return sciter::string(s.begin(), s.end());
}

std::string WstrToStr(std::wstring s) {
	return std::string(s.begin(), s.end());
}
std::wstring StrToWstr(std::string s) {
	return std::wstring(s.begin(), s.end());
}

std::string intToHEXstr(int input){
	std::ostringstream ss;
	ss << std::hex << input;
	std::string result = ss.str();

	return result;
}


std::wstring dtos(double val) { // double to string without useful nulls // not mine
	std::wstring str = std::to_wstring(val);
	str.erase(str.find_last_not_of('0') + 1, std::string::npos);
	str.erase(str.find_last_not_of('.') + 1, std::string::npos);
	return str;
}




std::string ConvertToString(DWORD value) { 
	std::stringstream ss;    
	ss << value;    
	return ss.str(); 
}

#if DEBUG 

void debugCLEAR() {
	system("cls");
}

void debugCOLOR(WORD color) {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void debugLOG(std::string s, bool onNewLine = true) {
	if (onNewLine) { s = "\n" + s; } 
	std::wstring stemp = std::wstring(s.begin(), s.end());
	LPCWSTR sw = stemp.c_str();
	DWORD dwSize = 0;
	WriteConsole(GetStdHandle(STD_OUTPUT_HANDLE), sw, lstrlen(sw), &dwSize, NULL);
}

void debugLOG(sciter::string s, bool onNewLine = true) {
	if (onNewLine) {
		WriteConsole(GetStdHandle(STD_OUTPUT_HANDLE), "\n", 1, 0, NULL);
	}
	std::wstring stemp = std::wstring(s.begin(), s.end());
	LPCWSTR sw = stemp.c_str();
	DWORD dwSize = 0;
	WriteConsole(GetStdHandle(STD_OUTPUT_HANDLE), sw, lstrlen(sw), &dwSize, NULL);

	
} 
 
void debugLOG(int num, bool onNewLine = true) {
	std::string s = std::to_string(num);
	if (onNewLine) { s = "\n" + s; }
	std::wstring stemp = std::wstring(s.begin(), s.end());
	LPCWSTR sw = stemp.c_str();
	DWORD dwSize = 0;
	WriteConsole(GetStdHandle(STD_OUTPUT_HANDLE), sw, lstrlen(sw), &dwSize, NULL);
}

void debugLOG(float num, bool onNewLine = true) {
	std::string s = std::to_string(num);
	if (onNewLine) { s = "\n" + s; }
	std::wstring stemp = std::wstring(s.begin(), s.end());
	LPCWSTR sw = stemp.c_str();
	DWORD dwSize = 0;
	WriteConsole(GetStdHandle(STD_OUTPUT_HANDLE), sw, lstrlen(sw), &dwSize, NULL);
}  

void debugLOG(double num, bool onNewLine = true) {
	std::string s = std::to_string(num);
	if (onNewLine) { s = "\n" + s; }
	std::wstring stemp = std::wstring(s.begin(), s.end());
	LPCWSTR sw = stemp.c_str();
	DWORD dwSize = 0;
	WriteConsole(GetStdHandle(STD_OUTPUT_HANDLE), sw, lstrlen(sw), &dwSize, NULL);
}

#else

// když není zaplý debug režim tak se nic nevypisuje
#define debugLOG(...)
#define debugCOLOR(...)
#define debugCLEAR()
#endif



void startDebugConsole() {
	AllocConsole();
	debugLOG("Starting console..");
}

void printElement(sciter::dom::element target) {
	sciter::string id = target.get_attribute("id");
	if (id.size() == 0) { debugLOG("noID"); }
	else {
		debugLOG(target.get_value().to_string());
		debugLOG(" - ", false);
	}
	debugLOG(id);
	debugLOG(" - ", false);

	debugLOG(target.get_element_type(), false);
	debugLOG(" - ", false);
	debugLOG((int)target.get_attribute_count(), false);
	debugLOG(" : ", false);
	for (size_t i = 0; i < target.get_attribute_count(); i++) {
		debugLOG(target.get_attribute_name(i), false);
		debugLOG(", ", false);
	}
}

std::string toLower(std::string text) {
	for (size_t i = 0; i < text.length(); i++) {
		text.at(i) = towlower(text.at(i));
	}
	return text;
}

bool stringToBool(std::string value) { // true false 1 0 on off 
	value = toLower(value);
	if (value == "1" || value == "on" || value == "true") {
		return true;
	}
	else {
		return false;
	}
}

void toClipboard(HWND hwnd, const std::string& s) {
	OpenClipboard(hwnd);
	EmptyClipboard();
	HGLOBAL hg = GlobalAlloc(GMEM_MOVEABLE, s.size() + 1);
	if (!hg) {
		CloseClipboard();
		return;
	}
	memcpy(GlobalLock(hg), s.c_str(), s.size() + 1);
	GlobalUnlock(hg);
	SetClipboardData(CF_TEXT, hg);
	CloseClipboard();
	GlobalFree(hg);
}