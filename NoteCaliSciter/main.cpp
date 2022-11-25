#define WIN_VERSION true
#define DEBUG true



#include <sciter-x.h>
#include <sciter-x-window.hpp>
#include "sciter-win-main.cpp"

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

    SciterSetOption(nullptr, SCITER_SET_DEBUG_MODE, true);
    SciterSetOption(NULL, SCITER_SET_SCRIPT_RUNTIME_FEATURES, ALLOW_FILE_IO | ALLOW_SOCKET_IO | ALLOW_EVAL | ALLOW_SYSINFO);

    startDebugConsole();

    Controler controler;

    controler.start();

    /*LPCWSTR stylePath = is_light_theme() ? L"lightTheme.css" : L"darkTheme.css";
    aux::bytes res_data = sciter::archive::instance().get(stylePath);
    SciterSetMasterCSS(res_data.start, res_data.length);*/
    

    return run();
}