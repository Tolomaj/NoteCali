#include <cctype>


class CalculatrWin : public sciter::window {
private:
    Controler *controler;
    LineRegister* lineSeparator;

public:
    CalculatrWin(Controler * controler, LineRegister* lineSeparator) : window(SW_POPUP | SW_ENABLE_DEBUG, { (MONITOR_WIDTH - CALC_WIN_WIDTH) / 2, (MONITOR_HEIGHT - CALC_WIN_HEIGHT) / 2 , (MONITOR_WIDTH + CALC_WIN_WIDTH) / 2, (MONITOR_HEIGHT + CALC_WIN_HEIGHT) / 2 }) {
        this->controler = controler;
        this->lineSeparator = lineSeparator;
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

    void setText(std::wstring dta,bool focus = true) {
        mathInput.set_text(dta.c_str());
        mathInput.update();
        if (focus) {
            std::string si2 = "this.focus(); this.textarea.selectRange(" + std::to_string(dta.length()) + "," + std::to_string(dta.length()) + "); ";
            mathInput.eval(aux::utf2w(si2));
        }
    };

    POINT getScroll(sciter::dom::element * highites) {
        POINT scroll; RECT r; SIZE s;
        mathInput.get_scroll_info(scroll, r, s);
        return scroll;
    }

    void handleScroll() {
        POINT scroll; RECT r; SIZE s;
        mathInput.get_scroll_info(scroll, r, s);
        highites.set_scroll_pos(scroll);
        mathOutput.set_scroll_pos(scroll); 
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
            debugLOG("starting");
            std::wstring data = sciterStrToWStr(target.get_value().to_string());
            debugLOG("startid");
            controler->procesChangedInput(data);
            debugLOG("inputProcesed");
        }

        if (((params.cmd == 32928 || params.cmd == 160) && target.get_element_uid() == texAreaUid) || params.cmd == EDIT_VALUE_CHANGED) { 
            handleScroll(); return true;
        }

        if (params.cmd == BUTTON_CLICK) {
           
            if (target.get_attribute("class") == L"mathOutputLine") {
                sciter::string s = target.text();
                std::string so = std::string(s.begin(), s.end());
                toClipboard(get_hwnd(), so); 
                updateStyles(); // idk
                return true;
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

    #define PREV_CHARS_NUM 3
    wchar peventedC[PREV_CHARS_NUM] =       { L'<'   ,L'>'   ,L'&' };
    wstring peventedAlias[PREV_CHARS_NUM] = { L"&lt;",L"&gt;",L"&amp;" };

    void preventFuncChars(mline *line) {
        for (int i = 0; i < line->line.size(); i++) {     //composite lines together with line ends and other things
            wchar testedC = line->line.at(i);
            for (size_t c = 0; c < PREV_CHARS_NUM; c++){
                if (testedC == peventedC[c]) {
                    line->line.replace(i, 1, peventedAlias[c]);
                    i = i + peventedAlias[c].size();
                }
            }
        }
    }



    void publish(std::vector<mline> lines) {
        // char preventing
        for (int i = 0; i < lines.size(); i++) {     //composite lines together with line ends and other things
            preventFuncChars(&lines.at(i));
        }


        std::wstring htmlin = L"";
        // highlights
        if (settings.ishighlitingOn()) {
           /*testing*/ 
            if (lines.size() > 0) {
                lines.at(0).line = L"<mf>" + lines.at(0).line + L"</mf>";
            }
            /*tersting end*/
        }
        //highlights end


        int lineCount = 0;
        for (int i = 0; i < lines.size();i++) {     //composite lines together with line ends and other things
            if (lines.at(i).lineModifier != L"") {
                htmlin += L";" + lines.at(i).lineModifier;
                if (lines.at(i).line != L"") { htmlin += L" "; }
            }
            htmlin.append(lines.at(i).line + L"<le id=\"le" + std::to_wstring(i) + L"\">i</le>\n");
        }
        aux::w2utf utf8(htmlin);
        highites.set_html((LPCBYTE)utf8, utf8.length());
        highites.update();


        debugLOG("best");
       
        std::wstring solutionString = L"";
        int prevousLineEndPosX = -settings.fontPadding;

        for (size_t i = 0; i < lines.size(); i++) {
            sciter::dom::element lineEnd = highites.get_element_by_id((L"le" + std::to_wstring(i)).c_str());
            int LineEndPosX = lineEnd.get_location(PADDING_BOX).bottom - highites.get_location(PADDING_BOX).top + getScroll(&highites).y;
            int LineCount = round((LineEndPosX - prevousLineEndPosX) / (settings.fontSize + 2.0f * settings.fontPadding));

            wstring type = settings.clickToCopy ? L"button" : L"span";
            solutionString.append(L"<" + type + L" id=\"molID" + std::to_wstring(i) + L"\" class=\"mathOutputLine\" style=\"padding: " + std::to_wstring((LineCount - 1) * (settings.fontSize + 2 * settings.fontPadding) / 2) + L"px 0;\">" + lines.at(i).solution + L"</" + type + L">");
            
            prevousLineEndPosX = LineEndPosX;
        }
        aux::w2utf utf82(solutionString);
        mathOutput.set_html((LPCBYTE)utf82, utf82.length());
        mathOutput.update();

    }
};



void CalculatrWin::toggleSettingsWin() {
    if (settingsWin != nullptr && settingsWin->is_valid()) {
        settingsWin->close();
    }
    else {
        settingsWin = new SettingsWin();
        settingsWin->load(L"this://app/settings.htm");
        SetWindowPos(settingsWin->get_hwnd(), 0, CalculateValidPositionX(), CalculateValidPositionY(), SETTINGS_WIN_WIDTH, SETTINGS_WIN_HEIGHT, SW_POPUP | SW_ENABLE_DEBUG);
        settingsWin->expand();
    }
};


void CalculatrWin::updateStyles() {
    
   std::string si = "document.style.variable('FontSize','" + std::to_string(settings.fontSize) + "');";
   si += "document.style.variable('FontColor','" + settings.fontColor + "');";
   si += "document.style.variable('SolutionFontColor','" + settings.solutionFontColor + "');";
   si += "document.style.variable('ClickColor','" + settings.clickColor + "');";
   si += "document.style.variable('HowerColor','" + settings.howerColor + "');";
   si += "document.style.variable('BackgroundColor', '" + settings.backgroudColor + "'); ";
   si += "document.style.variable('dividerLineColor','" + settings.dividerLineColor + "');";
   debugLOG(si);
   eval(aux::chars(si.c_str(), si.length())); // testing
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
