/* eventslist ;
BUTTON_CLICK = 0,              // click on button
BUTTON_PRESS = 1,              // mouse down or key down in button
BUTTON_STATE_CHANGED = 2,      // checkbox/radio/slider changed its state/value
EDIT_VALUE_CHANGING = 3,       // before text change
EDIT_VALUE_CHANGED = 4,        // after text change
SELECT_SELECTION_CHANGED = 5,  // selection in <select> changed
SELECT_STATE_CHANGED = 6,      // node in select expanded/collapsed, heTarget is the node
POPUP_REQUEST = 7,           // request to show popup just received,                                //     here DOM of popup element can be modifed.
POPUP_READY = 8,           // popup element has been measured and ready to be shown on screen,
POPUP_DISMISSED = 9,           // popup element is closed,
MENU_ITEM_ACTIVE = 0xA,        // menu item activated by mouse hover or by keyboard,
MENU_ITEM_CLICK = 0xB,         // menu item click,
CONTEXT_MENU_REQUEST = 0x10,   // "right-click", BEHAVIOR_EVENT_PARAMS::he is current popup menu HELEMENT being processed or NULL.
VISIUAL_STATUS_CHANGED = 0x11, // broadcast notification, sent to all elements of some container being shown or hidden
DISABLED_STATUS_CHANGED = 0x12,// broadcast notification, sent to all elements of some container that got new value of :disabled state
POPUP_DISMISSING = 0x13,       // popup is about to be closed
CONTENT_CHANGED = 0x15,        // content has been changed, is posted to the element that gets content changed,  reason is combination of CONTENT_CHANGE_BITS.
CLICK = 0x16,                  // generic click
CHANGE = 0x17,                 // generic change
HYPERLINK_CLICK = 0x80,        // hyperlink click
ELEMENT_COLLAPSED = 0x90,      // element was collapsed, so far only behavior:tabs is sending these two to the panels
ELEMENT_EXPANDED,              // element was expanded,
ACTIVATE_CHILD,                // activate (select) child,
INIT_DATA_VIEW,                // request to virtual grid to initialize its view
ROWS_DATA_REQUEST,             // request from virtual grid to data source behavior to fill data in the table
UI_STATE_CHANGED,              // ui state changed, observers shall update their visual states.
FORM_SUBMIT,                   // behavior:form detected submission event. BEHAVIOR_EVENT_PARAMS::data field contains data to be posted.
FORM_RESET,                    // behavior:form detected reset event (from button type=reset). BEHAVIOR_EVENT_PARAMS::data field contains data to be reset.
DOCUMENT_COMPLETE,             // document in behavior:frame or root document is complete.
HISTORY_PUSH,                  // requests to behavior:history (commands)
HISTORY_DROP,
HISTORY_PRIOR,
HISTORY_NEXT,
HISTORY_STATE_CHANGED,         // behavior:history notification - history stack has changed
CLOSE_POPUP,                   // close popup request,
REQUEST_TOOLTIP,               // request tooltip, evt.source <- is the tooltip element.
ANIMATION = 0xA0,      // animation started (reason=1) or ended(reason=0) on the element.
DOCUMENT_CREATED = 0xC0,      // document created, script namespace initialized. target -> the document
DOCUMENT_CLOSE_REQUEST = 0xC1, // document is about to be closed, to cancel closing do: evt.data = sciter::value("cancel");
DOCUMENT_CLOSE = 0xC2,      // last notification before document removal from the DOM
DOCUMENT_READY = 0xC3,      // document has got DOM structure, styles and behaviors of DOM elements. Script loading run is complete at this moment. 
VIDEO_INITIALIZED = 0xD1,      // <video> "ready" notification   
VIDEO_STARTED = 0xD2,      // <video> playback started notification   
VIDEO_STOPPED = 0xD3,      // <video> playback stoped/paused notification   
VIDEO_BIND_RQ = 0xD4,      // <video> request for frame source binding, 
PAGINATION_STARTS = 0xE0,     // behavior:pager starts pagination
PAGINATION_PAGE = 0xE1,     // behavior:pager paginated page no, reason -> page no
PAGINATION_ENDS = 0xE2,     // behavior:pager end pagination, reason -> total pages
FIRST_APPLICATION_EVENT_CODE = 0x100
*/  
// events list 

using namespace sciter::dom;


class SettingsWin : public sciter::window {
private:
    Controler* controler;

public:
    SettingsWin(Controler* controler) : window(SW_POPUP, { 0, 0 , 600, 500 }) {
        this->controler = controler;
    }

    SOM_PASSPORT_BEGIN(SettingsWin)
        SOM_FUNCS(SOM_FUNC(setSet)
            , SOM_FUNC(openFile)
            , SOM_FUNC(setSwitch)
            , SOM_FUNC(setNumInput)
            , SOM_FUNC(reloadVariables)
            , SOM_FUNC(saveVariables)
            , SOM_FUNC(chckVer))
        SOM_PASSPORT_END

    void setSet(sciter::string variable, sciter::string value, sciter::string complete);

    void saveVariables(sciter::string variableList);

    void setSwitch(sciter::string variable, sciter::string value);

    void setNumInput(sciter::string variable, int value);

    void updateVersionBanner(wstring backgroundColor, wstring BorderColor, wstring text, bool nConect, bool nActual);

    void showVerisonStartus(int versionStatus);

    void chckVer();

    void openFile(sciter::string file);

    void reloadVariables();

    void loadSettingsInWindow();

    void updateStyles() {}

    void handle_size(HELEMENT he);

    void numberInputEvent(sciter::dom::element target);

    virtual bool handle_event(HELEMENT, BEHAVIOR_EVENT_PARAMS& params) { 
        sciter::dom::element target = params.heTarget;
        sciter::string elementId = target.get_attribute("id");
        string eID = sciterStrToStr(elementId);

        string stro = "_";

        if (target.is_valid()) { stro = std::to_string(target.get_value().get(0)); } // debug only
        if (target.is_valid()) { stro = std::to_string(target.get_value().get(0)); } // debug only
       
        //bugLOG("something with:" + std::to_string(params.cmd) + " - " + WstrToStr(params.name) + " - " + std::to_string(params.reason) + " - " + stro + " - " + sciterStrToStr(elementId));

       
        switch (params.cmd) {
            case EDIT_VALUE_CHANGED:
                debugLOG("something with:" + std::to_string(params.cmd) + " - " + WstrToStr(params.name) + " - " + std::to_string(params.reason) + " - " + stro + " - " + sciterStrToStr(elementId));

                if (eID.length() > 3 && eID.substr(eID.length() - 3,3) == "INP") { // když se zmìní (jmeno)+"INP" tak mìní
                    setNumInput(elementId, target.get_value().get(0));
                }
               
                
                break;

                break;
            case BUTTON_CLICK:
                debugLOG("Button/Switch Click");
               /* if (target.test("switch.inp")) {
                    string name = sciterStrToStr(target.get_attribute("id")); // vezme Id elementu. to je název settings + "SW"
                    settings.setSingleSettingsbyName(name.substr(0, name.size() - 2), (bool)target.get_value().get(0)); // odstraní z id "SW" a nastaví hodnotou
                    controler->processSettingsChange();
                    return true; // handled
                }*/

                if (target.test("button.categoryButton")) { // nastav kategorii / auto dark light custom
                    settings.stylescheme = _wtoi(target.get_attribute("value").c_str());      // nastavi categorii  
                    controler->processSettingsChange(RELOAD_FROM_FILE, SAVE_SYTEM_ONLY_FILE); // naète nastavení pro kategorii
                    loadSettingsInWindow(); // naète styli pro kategorii z aktuálnho nastravení stylù
                    return true; // handled
                }
                if (target.test("button.updateButton")) { // nastav kategorii / auto dark light custom
                    settings.loadVersionStatusOn = _wtoi(target.get_attribute("value").c_str());      // nastavi categorii 
                    settings.saveSettings();
                    if (settings.loadVersionStatusOn != ON_NEVER) {
                        chckVer(); // zkontrolovat verzi
                    }
                    return true; // handled
                }
                if (target.test("button#closeB")) { // nastav kategorii / auto dark light custom
                    close();
                    return true; // handled
                }
        }
        return false;
    }


};


void SettingsWin::saveVariables(sciter::string variableList) {
    debugLOG(L"variable savibng: " + variableList);
    variableTable.saveRawComposite("[constants]\n" + sciterStrToStr(variableList)); // template: [constants]\nzero = 0\none = 1\nsqrttwo = 1.4142\n
    controler->reloadVariables();
    reloadVariables();
}

void SettingsWin::setSwitch(sciter::string variable, sciter::string value) {
    debugLOG("varus: " + sciterStrToStr(variable) + " -- " + to_string((int)(value == L"true")));
    string name = sciterStrToStr(variable); // vezme Id elementu. to je název settings + "SW"
    settings.setSingleSettingsbyName(name.substr(0, name.size() - 2), (bool)(value == L"true")); // odstraní z id "SW" a nastaví hodnotou
    controler->processSettingsChange();
    //return true; // handled
}

void SettingsWin::setNumInput(sciter::string variable, int value) {
    string name = sciterStrToStr(variable); // vezme Id elementu. to je název settings + "INP"
    debugLOG("intus: " + name.substr(0, name.size() - 3) + " -- " + to_string(value));
    settings.setSingleSettingsbyName(name.substr(0, name.size() - 3), value); // odstraní z id "SW" a nastaví hodnotou
    controler->processSettingsChange();
    //return true; // handled
}



void SettingsWin::updateVersionBanner(wstring backgroundColor, wstring BorderColor, wstring text, bool nConect, bool nActual) {
    sciter::dom::element root = sciter::dom::element::root_element(get_hwnd()); //get root element
    ((element)root.get_element_by_id("uptudateBanner")).set_style_attribute("background-color", backgroundColor.c_str()); // set background volor of version baner
    ((element)root.get_element_by_id("uptudateBanner")).set_style_attribute("border", (wstring(L"2px solid") + BorderColor).c_str()); // change version baner border color
    ((element)root.get_element_by_id("actualizationMesage")).set_text(text.c_str()); // set version tenxt

    ((element)root.get_element_by_id("nonConectedBanner")).set_style_attribute("display", nConect ? L"block" : L"none"); // show/hide not conected baner
    ((element)root.get_element_by_id("nonActualBanner")).set_style_attribute("display", nActual ? L"block" : L"none"); // show/hide not actual banner
}

void SettingsWin::showVerisonStartus(int versionStatus) {
    sciter::dom::element root = sciter::dom::element::root_element(get_hwnd());

    bool isUpdatesON = (settings.loadVersionStatusOn != ON_NEVER);

    if (versionStatus == IS_ACTUAL) {
        updateVersionBanner(L"#92c680", L"#b4c9ad", L"APP up to date", false, false);
    }
    else if (versionStatus == IS_NOT_ACTUAL) {
        updateVersionBanner(L"#942f3b", L"#691d26", L"APP out dated", false, isUpdatesON);
    }
    else {
        updateVersionBanner(L"#e09136", L"#ffb13d", L"APP status unknow", isUpdatesON, false);
    }
}

void SettingsWin::chckVer() {
    settings.isActual = checkForUpdate();
    showVerisonStartus(settings.isActual);
}

void SettingsWin::openFile(sciter::string file) {
    debugLOG(L"OpeningFile: ");
    debugLOG(L"OpeningFile: " + file);
    string s = (getenv("APPDATA") + string("\\NoteCali\\"));
    wstring ws = wstring(s.begin(), s.end()) + file;
    debugLOG(ws);
    ShellExecute(NULL, L"open", (wstring(s.begin(), s.end()) + file).c_str(), NULL, NULL, SW_SHOWNORMAL);
}

void SettingsWin::reloadVariables() {
    debugLOG("ReloadingVariables");
    variableTable.loadVariables();
    string arg;
    for (size_t i = 0; i < variableTable.table.size(); i++) {
        arg = arg + variableTable.table.at(i).varName + ";" + variableTable.table.at(i).number + ";";
    }
    arg.pop_back();

    this->call_function("loadVariables", arg);
}

void SettingsWin::handle_size(HELEMENT he) {
    debugLOG(" Handling Size");
    RECT rect;
    GetWindowRect(get_hwnd(), &rect);
    std::string si = "sizeChanged('" + to_string(rect.right - rect.left) + "','" + to_string(rect.bottom - rect.top) + "');";
    //eval(aux::chars(si.c_str(), si.length()));
    //this->call_function("sizeChanged", rect.right - rect.left,rect.bottom - rect.top);
    //set_attribute(const char* name, const WCHAR * value)
    sciter::dom::element root = sciter::dom::element::root_element(get_hwnd());
    if (rect.right - rect.left < 500) {
        ((element)root.get_element_by_id("body")).set_attribute("size", L"narrow");
    } else {
        ((element)root.get_element_by_id("body")).set_attribute("size", L"wide");
    }

}

void SettingsWin::numberInputEvent(sciter::dom::element target) {
    string setName = sciterStrToStr(target.get_attribute("id")); // get setting name atributte from setting hl element
    setName = setName.substr(0, setName.size() - 3);
    if (!settings.setSingleSettingsbyName(setName, (double)target.get_value().get(0.0f))) {
        debugLOG("triing INT");
        settings.setSingleSettingsbyName(setName, (int)round(target.get_value().get(0.0f)));
    };

    controler->processSettingsChange(); // refresh styles
    debugLOG("setSet: " + setName + " - " + to_string((double)target.get_value().get(0.0f)));
}

void SettingsWin::setSet(sciter::string variable, sciter::string value, sciter::string complete) {
    string var = sciterStrToStr(variable);
    var = var.substr(0, var.size() - 3);
    string val = sciterStrToStr(value);

    debugLOG(" Seting seting : " + var + " to value : " + val + " resfresh is: " + sciterStrToStr(complete));

    settings.setSingleSettingsbyName(var, val);

    if (complete == L"t") {
        controler->processSettingsChange(DONT_RELOAD_ON_FILE, SAVE_FILE); // save settings into file
    }
    else {
        controler->processSettingsChange(DONT_RELOAD_ON_FILE, DONT_SAVE_FILE); // dont save into file only apply to styles
    }
}


void SettingsWin::loadSettingsInWindow() {
    // zkusit naèítání pomocí exec() jestli nebude rychlejší TODO 
    debugLOG("Starting loading to window.");
    sciter::dom::element root = sciter::dom::element::root_element(get_hwnd());

    showVerisonStartus(settings.isActual); 
    debugLOG("Loading 1/5");
    call_function("SetOpenedCategory", settings.stylescheme);
    call_function("SetUpdateOPT", settings.loadVersionStatusOn);
    debugLOG("Loading 2/5");
    ((element)root.get_element_by_id("showAppNameSW")).set_value(sciter::value(settings.showAppName));
    ((element)root.get_element_by_id("fontSizeINP")).set_value(sciter::value(settings.fontSize));
    //((element)root.get_element_by_id("fontPaddingINP")).set_value(sciter::value(settings.fontPadding)); //not done 
    //((element)root.get_element_by_id("transparencityINP")).set_value(sciter::value(settings.transparencity)); //not done 

    ((element)root.get_element_by_id("backgroudColorCLR")).set_style_attribute("background-color", StrToWstr(settings.backgroudColor).c_str());
    ((element)root.get_element_by_id("dividerLineColorCLR")).set_style_attribute("background-color", StrToWstr(settings.dividerLineColor).c_str());

    ((element)root.get_element_by_id("fontColorCLR")).set_style_attribute("background-color", StrToWstr(settings.fontColor).c_str());
    ((element)root.get_element_by_id("titleColorCLR")).set_style_attribute("background-color", StrToWstr(settings.titleColor).c_str());
    ((element)root.get_element_by_id("solutionFontColorCLR")).set_style_attribute("background-color", StrToWstr(settings.solutionFontColor).c_str());
    debugLOG("Loading 3/5");
   // ((element)root.get_element_by_id("countingOnLineEndSW")).set_value(sciter::value(settings.countingOnLineEnd)); // not done
   // ((element)root.get_element_by_id("isAllLinesSuperlinesSW")).set_value(sciter::value(settings.isAllLinesSuperlines));
   // ((element)root.get_element_by_id("showLineNumbersSW")).set_value(sciter::value(settings.showLineNumbers));
    //((element)root.get_element_by_id("clickToCopySW")).set_value(sciter::value(settings.clickToCopy));
    ((element)root.get_element_by_id("useRadiansSW")).set_value(sciter::value(settings.useRadians));
    ((element)root.get_element_by_id("useMetricsSW")).set_value(sciter::value(settings.useMetrics));
    ((element)root.get_element_by_id("ignoreHightDiferenceSW")).set_value(sciter::value(settings.ignoreHightDiference));
    ((element)root.get_element_by_id("allowLineJumpSW")).set_value(sciter::value(settings.allowLineJump));
    ((element)root.get_element_by_id("corectParenthesisSW")).set_value(sciter::value(settings.corectParenthesis));
    ((element)root.get_element_by_id("useSientificSW")).set_value(sciter::value(settings.useSientific));
    ((element)root.get_element_by_id("useLineModifiersSW")).set_value(sciter::value(settings.useLineModifiers));
    ((element)root.get_element_by_id("showErrTextSW")).set_value(sciter::value(settings.showErrText));
    ((element)root.get_element_by_id("useNoroundPointersSW")).set_value(sciter::value(settings.useNoroundPointers));

    ((element)root.get_element_by_id("snipetAlwaisVisibleSW")).set_value(sciter::value(settings.snipetAlwaisVisible));
    ((element)root.get_element_by_id("showSnipetSW")).set_value(sciter::value(settings.showSnipet));
    
    ((element)root.get_element_by_id("numberGroupingINP")).set_value(sciter::value(settings.numberGrouping));
    ((element)root.get_element_by_id("roundToDecINP")).set_value(sciter::value(settings.roundToDec));
    ((element)root.get_element_by_id("dividerLinePosINP")).set_value(sciter::value(settings.dividerLinePos));
    debugLOG("Loading 4/5");

    string arg;
    for (size_t i = 0; i < variableTable.table.size(); i++){
        arg = arg + variableTable.table.at(i).varName + ";" + variableTable.table.at(i).number + ";";
    }
    arg.pop_back();
    debugLOG("Loading 5/5");
    
    this->call_function("loadVariables",arg);
    debugLOG("Loading End.");

    // add loading more settings     ///TODO
}