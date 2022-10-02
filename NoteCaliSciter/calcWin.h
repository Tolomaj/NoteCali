#include <cctype>


void lop() {

};

class CalculatrWin : public sciter::window {
public:
    CalculatrWin() : window(SW_POPUP | SW_ENABLE_DEBUG, { (MONITOR_WIDTH - CALC_WIN_WIDTH) / 2, (MONITOR_HEIGHT - CALC_WIN_HEIGHT) / 2 , (MONITOR_WIDTH + CALC_WIN_WIDTH) / 2, (MONITOR_HEIGHT + CALC_WIN_HEIGHT) / 2 }) {}
    
    sciter::dom::element getElementById(std::string id);
    

    sciter::dom::element highites;
    sciter::dom::element mathInput;
    sciter::dom::element mathOutput;
    int texAreaUid = 0;

    bool firstEvent = true;
     
    void handleScroll() {
        POINT scroll; RECT r; SIZE s;
        mathInput.get_scroll_info(scroll, r, s);
        highites.set_scroll_pos(scroll);
        mathOutput.set_scroll_pos(scroll); 
    }

    void updateSettings() {
        //std::string s = ;
        //eval(const_chars(s.c_str()));
        //std::string s = "document.style.variable('FontSize','" + std::to_string(40) + "')";
        //eval(const_chars(s.c_str()));
    }


    virtual bool handle_event(HELEMENT, BEHAVIOR_EVENT_PARAMS& params) {
        sciter::dom::element target = params.heTarget;
        sciter::string elementId = target.get_attribute("id");

        if (firstEvent && params.cmd == DOCUMENT_READY) {
            firstEvent = false;
            highites = getElementById("highlights");
            mathInput = getElementById("mathInput");
            mathOutput = getElementById("mathOutput");
            texAreaUid = mathInput.get_element_uid();
        }


        if (params.cmd == EDIT_VALUE_CHANGED) {
            solver.solve(sciterStrToWStr(target.get_value().to_string()));
            solver.publish(highites, mathOutput, sciter::dom::element::root_element(get_hwnd()));
        }

        if (((params.cmd == 32928 || params.cmd == 160) && target.get_element_uid() == texAreaUid) || params.cmd == EDIT_VALUE_CHANGED) { 
            handleScroll();
        }

        if (params.cmd == BUTTON_CLICK) {
           
            if (target.get_attribute("class") == L"mathOutputLine") {
                sciter::string s = target.text();
                std::string so = std::string(s.begin(), s.end());
                toClipboard(get_hwnd(), so);     


              std::string si = "document.style.variable('FontSize','" + std::to_string(40) + "')";  // testing
              eval(aux::utf2w(si)); // testing
            }

            if (elementId == L"closeB") {
                this->close();
                PostQuitMessage(WM_QUIT);
                return true; // handled
            }
            if (elementId == L"meuB") {
                if (settingsWin != nullptr && settingsWin->is_valid()) {
                    settingsWin->close();
                } else {
                    settingsWin = new SettingsWin();
                    settingsWin->load(L"this://app/settings.htm");
                    SetWindowPos(settingsWin->get_hwnd(), 0, CalculateValidPositionX(), CalculateValidPositionY(), SETTINGS_WIN_WIDTH, SETTINGS_WIN_HEIGHT, SW_POPUP | SW_ENABLE_DEBUG);
                    settingsWin->expand();
                } 
                return true; // handled
            }
        }

        return false;
    }

    void handle_size(HELEMENT he);

    int CalculateValidPositionX();

    int CalculateValidPositionY();

    void toggleSettingsWin();
};


sciter::dom::element CalculatrWin::getElementById(std::string id) {
    sciter::dom::element root = sciter::dom::element::root_element(get_hwnd());
    return root.get_element_by_id(id.c_str());
};


void CalculatrWin::handle_size(HELEMENT he) {
    RECT rect;
    GetWindowRect(get_hwnd(), &rect);
    getElementById("appName").set_style_attribute("opacity", (rect.right - rect.left < 220) ? L"0" : L"1");
}

int CalculatrWin::CalculateValidPositionX() {
    RECT rect;
    GetWindowRect(get_hwnd(), &rect);
    bool isLeftSideBigger = rect.left > SETTINGS_WIN_WIDTH + WIN_MARGIN;
    bool winFits = rect.left > SETTINGS_WIN_WIDTH + WIN_MARGIN || MONITOR_WIDTH - rect.right > SETTINGS_WIN_WIDTH + WIN_MARGIN;
    if (winFits) {
        return (rect.left + rect.right - SETTINGS_WIN_WIDTH) / 2 + ((rect.right - rect.left + SETTINGS_WIN_WIDTH) / 2 + WIN_MARGIN) * (isLeftSideBigger ? -1 : 1);
    }
    else {
        return (MONITOR_WIDTH - SETTINGS_WIN_WIDTH) / 2;
    }
}


int CalculatrWin::CalculateValidPositionY() {
    RECT rect;
    GetWindowRect(this->get_hwnd(), &rect);
    int position = (rect.top + rect.bottom) / 2 - SETTINGS_WIN_HEIGHT / 2;
    if (position < 0) { position = 0; }
    if (position > MONITOR_HEIGHT - SETTINGS_WIN_HEIGHT) { position = MONITOR_HEIGHT - SETTINGS_WIN_HEIGHT; }
    return position;
}