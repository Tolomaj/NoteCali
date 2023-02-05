#define WIN_VERSION true
#define DEBUG false

#include <sciter-x.h>
#include <sciter-x-window.hpp>
#include "sciter-win-main.cpp"
#include "ttmath/ttmath.h"
#include <chrono>

#include <direct.h>

#if WIN_VERSION
#include <Uxtheme.h>
#include "debugConsole.h"
#endif // WIN_VERSION

#include <iostream>

#define SETTINGS_WIN_WIDTH 650
#define SETTINGS_WIN_HEIGHT 560

#define CALC_WIN_WIDTH 350
#define CALC_WIN_HEIGHT 450

#define MONITOR_WIDTH GetSystemMetrics(SM_CXFULLSCREEN)
#define MONITOR_HEIGHT GetSystemMetrics(SM_CYFULLSCREEN)

#define WIN_MARGIN 10

using namespace std;
#include "Controler.h"



int uimain(std::function<int()> run) {

#if DEBUG
    SciterSetOption(nullptr, SCITER_SET_DEBUG_MODE, true);
    SciterSetOption(NULL, SCITER_SET_SCRIPT_RUNTIME_FEATURES, ALLOW_FILE_IO | ALLOW_SOCKET_IO | ALLOW_EVAL | ALLOW_SYSINFO);
    startDebugConsole();
#endif

    /*
        string s = getenv("APPDATA") + string("\\NoteCali") + string("\\ini.ini");
        getenv("APPDATA") + string("\\NoteCali") + string("\\ini.ini");
        mkdir((getenv("APPDATA") + string("\\NoteCali")).c_str());
    */

    Controler controler;

    controler.start();

    return run();
}