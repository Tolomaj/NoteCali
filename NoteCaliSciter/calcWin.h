#include <cctype>


class CalculatrWin : public sciter::window {
private:
    Controler *controler;

public:
    CalculatrWin(Controler * controler) : window(SW_POPUP | SW_ENABLE_DEBUG, { (MONITOR_WIDTH - CALC_WIN_WIDTH) / 2, (MONITOR_HEIGHT - CALC_WIN_HEIGHT) / 2 , (MONITOR_WIDTH + CALC_WIN_WIDTH) / 2, (MONITOR_HEIGHT + CALC_WIN_HEIGHT) / 2 }) {
        this->controler = controler;
    }
    
    sciter::dom::element getElementById(std::string id);
    
    void updateStyles();

    void handle_size(HELEMENT he);

    void toggleSettingsWin();

    void updateSettings();


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

    virtual bool handle_event(HELEMENT, BEHAVIOR_EVENT_PARAMS& params) {
        sciter::dom::element target = params.heTarget;
        sciter::string elementId = target.get_attribute("id");

        if (params.cmd == INPUT_KEYBOARD) {
            debugLOG(params.data.to_string());
        }

        if (firstEvent && params.cmd == DOCUMENT_READY) {
            firstEvent = false;
            highites = getElementById("highlights");
            mathInput = getElementById("mathInput");
            mathOutput = getElementById("mathOutput");
            texAreaUid = mathInput.get_element_uid();
        }


        if (params.cmd == EDIT_VALUE_CHANGED) {
            std::wstring dta = sciterStrToWStr(target.get_value().to_string());
            std::wstring comand;
            int change = solver.processComands(&dta,&comand);
            controler->doCommand(comand);
           // printElement(target);

            if (change != -1) {
                target.set_text(dta.c_str());
                target.update();
                debugLOG(std::to_string(change) + " - " + std::to_string(dta.length()));
                std::string si2 = "this.focus(); this.textarea.selectRange(" + std::to_string(dta.length()) + "," + std::to_string(dta.length()) + "); ";
                target.eval(aux::utf2w(si2));
                /* std::string si2 = "document.getElementById(\"mathInput\").textarea.selectRange(2, 5); ";
                target.eval(aux::utf2w(si2));*/
            }
            

            
            solver.solve(dta);
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
                updateStyles();
            }

            if (elementId == L"closeB") {
                this->close();
                PostQuitMessage(WM_QUIT);
                return true; // handled
            }
            if (elementId == L"meuB") {
                controler->toggleSettings();
                return true; // handled
            }
        }

        return false;
    }


};


void CalculatrWin::updateSettings() {

};

void CalculatrWin::updateStyles() {
    
   std::wstring si = L"document.style.variable('FontSize','" + std::to_wstring(settings.fontSize) + L"');";
   si += L"document.style.variable('FontColor','" + settings.fontColor + L"');";
   si += L"document.style.variable('SolutionFontColor','" + settings.solutionFontColor + L"');";
   si += L"document.style.variable('ClickColor','" + settings.clickColor + L"');";
   si += L"document.style.variable('HowerColor','" + settings.howerColor + L"');";
   si += L"document.style.variable('BackgroundColor','" + settings.backgroudColor + L"');";
   si += L"document.style.variable('dividerLineColor','" + settings.dividerLineColor + L"');";
   debugLOG(si);
   eval(aux::wchars(si.c_str(), si.length())); // testing
   debugLOG(si);
};

sciter::dom::element CalculatrWin::getElementById(std::string id) {
    sciter::dom::element root = sciter::dom::element::root_element(get_hwnd());
    return root.get_element_by_id(id.c_str());
};


void CalculatrWin::handle_size(HELEMENT he) {
    RECT rect;
    GetWindowRect(get_hwnd(), &rect);
    if (settings.showAppName) {
        getElementById("appName").set_style_attribute("opacity", (rect.right - rect.left < 220) ? L"0" : L"1");
    }
    else {
        getElementById("appName").set_style_attribute("opacity", L"0");
    }
    
}

