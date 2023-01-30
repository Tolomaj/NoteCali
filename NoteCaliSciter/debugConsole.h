#include <Windows.h>
#include <string>
#include <sstream> 

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

void debugLOG(std::string s, bool onNewLine = true) {}

void debugLOG(sciter::string s, bool onNewLine = true) {}

void debugLOG(int num, bool onNewLine = true) {}

void debugLOG(float num, bool onNewLine = true) {}

void debugLOG(double num, bool onNewLine = true) {}

#endif



void startDebugConsole() {
	AllocConsole();
	debugLOG("Starting console..");
	debugLOG(12);
	debugLOG(12,false);
	debugLOG((float)6.9);
	debugLOG(4.2);
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