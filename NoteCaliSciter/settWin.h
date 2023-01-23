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
    SettingsWin(Controler* controler) : window(SW_POPUP | SW_ENABLE_DEBUG | SW_HIDE, { 0, 0 , 600, 500 }) {
        this->controler = controler;
    }

    SOM_PASSPORT_BEGIN(SettingsWin)
        SOM_FUNCS( SOM_FUNC(setSet) )
        SOM_PASSPORT_END

    void setSet(sciter::string variable, sciter::string value, sciter::string complete);

    void loadSettingsInWindow();

    void updateStyles() { }

    void numberInputEvent(sciter::dom::element target);


    virtual bool handle_event(HELEMENT, BEHAVIOR_EVENT_PARAMS& params) { 
        sciter::dom::element target = params.heTarget;
        sciter::string elementId = target.get_attribute("id");

        string stro = "_";

        if (target.is_valid()) { stro = std::to_string(target.get_value().get(0)); } // debug only
        debugLOG("something with:" + std::to_string(params.cmd )  + " - " + WstrToStr(params.name) + " - " + std::to_string(params.reason) + " - " + stro  + " - " + sciterStrToStr(elementId));

        switch (params.cmd) {
            case 23: debugLOG("NumberEventInput");  numberInputEvent(target);  break;
                    
            case 161: // není v dokumentaci, odpozorováno. zavoláno dvakrát jednou reson je 1 podrzhé 0
                if (target.test("switch.inp") && params.reason == 0) { // switch / bool event
                    string name = sciterStrToStr(target.get_attribute("id")); // vezme Id elementu. to je název settings + "SW"
                    settings.setSingleSettingsbyName(name.substr(0, name.size() - 2), (bool)target.get_value().get(0)); // odstraní z id "SW" a nastaví hodnotou
                    controler->processSettingsChange();
                    return true; // handled
                }

                if (target.test("input.inp") && params.reason == 0) {
                    debugLOG(L"##hvent catch with 161:" + std::to_wstring(params.cmd) + L" - " + std::to_wstring(target.get_value().get(0)) + L" -daps1: " + std::to_wstring(params.reason));
                    return true; // handled
                }

                if (target.test("caption") && params.reason == 0) {  
                    numberInputEvent(target.parent()); 
                    debugLOG("mocenos");
                    return true; 
                }

                break;
            case BUTTON_CLICK:
                if (target.test("button.categoryButton")) { // nastav kategorii / auto dark light custom
                    settings.stylescheme = _wtoi(target.get_attribute("value").c_str());      // nastavi categorii  
                    controler->processSettingsChange(RELOAD_FROM_FILE, SAVE_SYTEM_ONLY_FILE); // naète nastavení pro kategorii
                    loadSettingsInWindow(); // naète styli pro kategorii z aktuálnho nastravení stylù
                    return true; // handled
                }
        }
        return false;
    }


};




void SettingsWin::numberInputEvent(sciter::dom::element target) {
    string setName = sciterStrToStr(target.get_attribute("id")); // get setting name atributte from setting hl element
    setName = setName.substr(0, setName.size() - 3);
    settings.setSingleSettingsbyName(setName, (double)target.get_value().get(0.0f));
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
    call_function("SetOpenedCategory", settings.stylescheme);
    sciter::dom::element root = sciter::dom::element::root_element(get_hwnd());
    ((element)root.get_element_by_id("showAppNameSW")).set_value(sciter::value(settings.showAppName));
    ((element)root.get_element_by_id("fontSizeINP")).set_value(sciter::value(settings.fontSize));
    ((element)root.get_element_by_id("fontPaddingINP")).set_value(sciter::value(settings.fontPadding));
    ((element)root.get_element_by_id("transparencityINP")).set_value(sciter::value(settings.transparencity));

    ((element)root.get_element_by_id("backgroudColorCLR")).set_style_attribute("background-color", StrToWstr(settings.backgroudColor).c_str());
    ((element)root.get_element_by_id("dividerLineColorCLR")).set_style_attribute("background-color", StrToWstr(settings.dividerLineColor).c_str());

    ((element)root.get_element_by_id("fontColorCLR")).set_style_attribute("background-color", StrToWstr(settings.fontColor).c_str());

    ((element)root.get_element_by_id("countingOnLineEndSW")).set_value(sciter::value(settings.countingOnLineEnd));
    ((element)root.get_element_by_id("isAllLinesSuperlinesSW")).set_value(sciter::value(settings.isAllLinesSuperlines));
    ((element)root.get_element_by_id("showLineNumbersSW")).set_value(sciter::value(settings.showLineNumbers));
    ((element)root.get_element_by_id("clickToCopySW")).set_value(sciter::value(settings.clickToCopy));

    // add loading more settings     ///TODO
}