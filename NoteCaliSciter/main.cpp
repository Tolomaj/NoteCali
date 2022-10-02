#define WIN_VERSION true

#include <sciter-x.h>
#include <sciter-x-window.hpp>
#include "sciter-win-main.cpp"
#include "resources.cpp" // resources packaged into binary blob.

#if WIN_VERSION
#include <Uxtheme.h>
#include "debugConsole.h"
#endif // WIN_VERSION

#include <iostream>

#include "SettingsObject.h"
#include "calcualtionAlg.h"

#define SETTINGS_WIN_WIDTH 650
#define SETTINGS_WIN_HEIGHT 560

#define CALC_WIN_WIDTH 350
#define CALC_WIN_HEIGHT 450

#define MONITOR_WIDTH GetSystemMetrics(SM_CXFULLSCREEN)
#define MONITOR_HEIGHT GetSystemMetrics(SM_CYFULLSCREEN)

#define WIN_MARGIN 10


#include "settWin.h"
sciter::om::hasset<SettingsWin> settingsWin;
#include "calcWin.h"



int uimain(std::function<int()> run) {
    
    sciter::archive::instance().open(aux::elements_of(resources)); // bind resources[] (defined in "resources.cpp") with the archive

    sciter::om::hasset<CalculatrWin> pwin = new CalculatrWin();
    /*LPCWSTR stylePath = is_light_theme() ? L"lightTheme.css" : L"darkTheme.css";
    aux::bytes res_data = sciter::archive::instance().get(stylePath);
    SciterSetMasterCSS(res_data.start, res_data.length);*/
    
    startDebugConsole();
    debugLOG((int)is_light_theme());
    // note: this:://app URL is dedicated to the sciter::archive content associated with the application
    pwin->load(L"this://app/calculator.htm");
    

    //or use this to load UI from  
    //  pwin->load( WSTR("file:///home/andrew/Desktop/Project/res/main.htm") );

    pwin->expand();
    pwin->updateSettings();

    return run();
}