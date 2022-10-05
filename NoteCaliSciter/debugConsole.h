#include <Windows.h>
#include <string>
#include <sstream> 

std::string sciterStrToStr(sciter::string s){
	return std::string(s.begin(), s.end());
}

std::wstring sciterStrToWStr(sciter::string s){
	return std::wstring(s.begin(), s.end());
}

std::string WstrToStr(std::wstring s) {
	return std::string(s.begin(), s.end());
}

bool is_light_theme() {
	// based on https://stackoverflow.com/questions/51334674/how-to-detect-windows-10-light-dark-mode-in-win32-application

	// The value is expected to be a REG_DWORD, which is a signed 32-bit little-endian
	auto buffer = std::vector<char>(4);
	auto cbData = static_cast<DWORD>(buffer.size() * sizeof(char));
	auto res = RegGetValueW(
		HKEY_CURRENT_USER,
		L"Software\\Microsoft\\Windows\\CurrentVersion\\Themes\\Personalize",
		L"AppsUseLightTheme",
		RRF_RT_REG_DWORD, // expected value type
		nullptr,
		buffer.data(),
		&cbData);

	if (res != ERROR_SUCCESS) {
		throw std::runtime_error("Error: error_code=" + std::to_string(res));
	}

	// convert bytes written to our buffer to an int, assuming little-endian
	auto i = int(buffer[3] << 24 |
		buffer[2] << 16 |
		buffer[1] << 8 |
		buffer[0]);

	return i == 1;
}



std::string ConvertToString(DWORD value) { 
	std::stringstream ss;    
	ss << value;    
	return ss.str(); 
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
	for (int i = 0; i < target.get_attribute_count(); i++) {
		debugLOG(target.get_attribute_name(i), false);
		debugLOG(", ", false);
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