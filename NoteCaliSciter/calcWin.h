#include <cctype>

#define KEY_PRESED_DOWN 32769
#define LEFT_ALT 4
#define RIGHT_ALT 5
#define CTRL 1

class CalculatrWin : public sciter::window {
private:
    Controler *controler;   // odkaz na controler pro volání eventù
    LineRegister* lineSeparator;

    bool firstEvent = true; //  první naètení elementù
    sciter::dom::element highites; //odkaz na èasto používané elementy
    sciter::dom::element mathInput; //odkaz na èasto používané elementy
    sciter::dom::element mathOutput; //odkaz na èasto používané elementy

    #define PREV_CHARS_NUM 3    //poèet preventovaných znaku v html
    const wchar peventedC[PREV_CHARS_NUM] =       { L'<'   ,L'>'   ,L'&' };   //preventovane znaky
    const wstring peventedAlias[PREV_CHARS_NUM] = { L"&lt;",L"&gt;",L"&amp;" }; // alias pro preventovany znak




public:
    CalculatrWin(Controler * controler, LineRegister* lineSeparator) : window(SW_POPUP , { (MONITOR_WIDTH - CALC_WIN_WIDTH) / 2, (MONITOR_HEIGHT - CALC_WIN_HEIGHT) / 2 , (MONITOR_WIDTH + CALC_WIN_WIDTH) / 2, (MONITOR_HEIGHT + CALC_WIN_HEIGHT) / 2 }) {
        this->controler = controler;
        this->lineSeparator = lineSeparator;
    }

    sciter::dom::element getElementById(std::string id);
    
    void updateStyles();   // updatne styl okna.

    void handle_size(HELEMENT he);  // updatne jak vypadá okno schová jméno když je moc malé

    void toggleSettingsWin();   // otevøe/zavøe okno nastavení

    void setText(std::wstring dta, bool focus = true);  // nastaví text userinputu

    POINT getScroll(sciter::dom::element* highites);

    void handleScroll();

    void preventFuncChars(mline* line);   //upravi linie tak aby neobsahovani funkcni zanky pro html jejich pseudonymi

    void publish(std::vector<mline> lines); // publish solutions and highlights
    
    void focus();

    virtual bool on_key(HELEMENT he, HELEMENT target, UINT event_type, UINT code, UINT keyboardStates) { 
        if (keyboardStates == RIGHT_ALT && code == 86) { // prevent rightAlt+v
            return true;
        }



        if (keyboardStates == CTRL && event_type == KEY_PRESED_DOWN){ // keyboardStates == 1 is ctrl
            if (code == 70) { // ctrl + f
                mathInput.set_text(L"");
                mathOutput.set_text(L"");
            }

        }

        if (keyboardStates == LEFT_ALT && event_type == KEY_PRESED_DOWN) { // keyboardStates == 4 is alt
           // debugLOG("keyWithALT - " + to_string((int)code));
        }

        return false;
    }

    

    virtual bool handle_event(HELEMENT, BEHAVIOR_EVENT_PARAMS& params) {
        sciter::dom::element target = params.heTarget;
        sciter::string elementId = target.get_attribute("id");

       

        debugLOG("something with:" + std::to_string(params.cmd) + " - " + WstrToStr(params.name) + " - " + std::to_string(params.reason) + " - " + sciterStrToStr(elementId));

       
        if (params.cmd == 193) { this->close(); PostQuitMessage(WM_QUIT); return true;  }// handlování zavøení okna

        if (firstEvent && params.cmd == DOCUMENT_READY) { //naètení èasto používaných obìktù
            highites = getElementById("highlights");
            mathInput = getElementById("mathInput");
            mathOutput = getElementById("mathOutput");
            mathInput.eval(aux::utf2w("this.focus();")); // not work probably

            firstEvent = false;
        }

        if (params.cmd == EDIT_VALUE_CHANGED) {  // zmenilo se pole zadání vypocitat ho a publishnout
            controler->procesChangedInput(sciterStrToWStr(target.get_value().to_string())); return true; 
        }
  
        if (((params.cmd == 32928 || params.cmd == 160) && target.get_element_uid() == mathInput.get_element_uid()) || params.cmd == EDIT_VALUE_CHANGED) {
            handleScroll(); return true;  // øesení scrollovaní
        }
        
        if (params.cmd == BUTTON_CLICK) {
            if (target.get_attribute("class") == L"mathOutputLine") { toClipboard(get_hwnd(), sciterStrToStr(target.get_attribute("val", L"0"))); return true; } // copy to click. zkopírování hodnoty do clipboardu
            if (elementId == L"closeB") { this->close(); PostQuitMessage(WM_QUIT); return true; } //zavreni okna
            if (elementId == L"meuB") { controler->toggleSettings(); return true; } //otevreni/ zavreni nastaveni
        }
        return false;
    }

 
};


void CalculatrWin::publish(std::vector<mline> lines) { // publish solutions and highlights 
    for (int i = 0; i < lines.size(); i++) { // char preventing < > &
        preventFuncChars(&lines.at(i));
    }


    std::wstring htmlin = L"";

    // highlights TODO
    if (settings.ishighlitingOn()) {
        /*testing*/
        if (lines.size() > 0) {
            //lines.at(0).line = L"<mf>" + lines.at(0).line + L"</mf>";
        }
        /*tersting end*/
    }
    //highlights end


    int lineCount = 0;
    for (int i = 0; i < lines.size(); i++) {     //composite lines together with line ends and other things
        if (lines.at(i).command != L"") {
            htmlin += L";" + lines.at(i).command;
            if (lines.at(i).line != L"") { htmlin += L" "; }
        }
        htmlin.append(lines.at(i).line + L"<le id=\"le" + std::to_wstring(i) + L"\">i</le>\n");
    }
    aux::w2utf utf8(htmlin);
    highites.set_html((LPCBYTE)utf8, utf8.length());
    highites.update();


    std::wstring solutionString = L"";
    double prevousLineEndPosX = -settings.fontPadding;

    for (size_t i = 0; i < lines.size(); i++) {
        sciter::dom::element lineEnd = highites.get_element_by_id((L"le" + std::to_wstring(i)).c_str()); //vezme linku k teré patøí øešení a vezme její pozici
        int LineEndPosX = lineEnd.get_location(PADDING_BOX).bottom - highites.get_location(PADDING_BOX).top + getScroll(&highites).y;
        int LineCount = (int)round((LineEndPosX - prevousLineEndPosX) / (settings.fontSize + settings.fontPadding)); // vypoèítá kolik linek pøíklad zabrá aby vysledek byl stejnì vysoký

        wstring type = settings.clickToCopy ? L"button" : L"p"; // sets if is clck copiable
        double paddingBtwLines = 0;
        double solutionLineHeight = (settings.fontSize + 4) * LineCount; // <- zahynu
                       

                                                           //V / must be selectable if is span !!! // TODO
        solutionString.append(L"<" + type + L" id=\"molID" + std::to_wstring(i) + L"\" class=\"mathOutputLine\" val=\"" + lines.at(i).solution + L"\" style=\"padding: " + std::to_wstring(paddingBtwLines) + L"px 0; height:" + std::to_wstring(solutionLineHeight) + L"\" >");
        solutionString.append(lines.at(i).solutionModifier + L" ");
        if (lines.at(i).localVariableName != L"") {
            solutionString.append(lines.at(i).localVariableName);
        }
        if (lines.at(i).error.type <= 0 || settings.showErrText) { // dont print errors
            solutionString.append(lines.at(i).solution); // composite non copiable part with copiable part of solution
            debugLOG("solution:");
            debugLOG(lines.at(i).solution);
        }
        solutionString.append( L"</" + type + L">"); // composite non copiable part with copiable part of solution

        prevousLineEndPosX = LineEndPosX;
    }
    aux::w2utf utf82(solutionString);
    mathOutput.set_html((LPCBYTE)utf82, utf82.length());
    mathOutput.update();

}

void CalculatrWin::preventFuncChars(mline* line) {    //upravi linie tak aby neobsahovani funkcni zanky pro html jejich pseudonymi
    for (size_t i = 0; i < line->line.size(); i++) {
        for (size_t c = 0; c < PREV_CHARS_NUM; c++) { // kazdy znak oskousi jestli není funkci
            if (line->line.at(i) == peventedC[c]) {
                line->line.replace(i, 1, peventedAlias[c]); // pokud ano nahradimeho aliasem
                i = i + peventedAlias[c].size();
            }
        }
    }
}

POINT CalculatrWin::getScroll(sciter::dom::element* element) {
    POINT scroll; RECT r; SIZE s;
    element->get_scroll_info(scroll, r, s);
    return scroll;
}

void CalculatrWin::handleScroll() {
    POINT scroll = getScroll(&mathInput);
    highites.set_scroll_pos(scroll);
    mathOutput.set_scroll_pos(scroll);
}

void CalculatrWin::setText(std::wstring dta, bool focus) {
    mathInput.set_text(dta.c_str());
    mathInput.update();
    if (focus) {
        std::string si2 = "this.focus(); this.textarea.selectRange(" + std::to_string(dta.length()) + "," + std::to_string(dta.length()) + "); ";
        mathInput.eval(aux::utf2w(si2));
    }
};

void CalculatrWin::updateStyles() { //možná i pro chování poèítání ?? / todo
   debugLOG(" Updating Styles");
   std::string si = "document.style.variable('FontSize','" + std::to_string(settings.fontSize) + "');";
   //si += "document.style.variable('FontPadding','" + std::to_string(settings.fontSize + settings.fontPadding) + "px');"; //
   si += "document.style.variable('FontColor','" + settings.fontColor + "');";
   si += "document.style.variable('SolutionFontColor','" + settings.solutionFontColor + "');";
   si += "document.style.variable('ClickColor','" + settings.clickColor + "');";
   si += "document.style.variable('HowerColor','" + settings.howerColor + "');";
   si += "document.style.variable('BackgroundColor', '" + settings.backgroudColor + "'); ";
   si += "document.style.variable('dividerLineColor','" + settings.dividerLineColor + "');";
   si += "document.style.variable('TitleColor','" + settings.titleColor + "');";

   si += "document.style.variable('dividerLinePos','" + to_string(settings.dividerLinePos) + "%');";
   si += "document.style.variable('dividerLineInvPos','" + to_string(100 - settings.dividerLinePos) + "%');";
   
#if DEBUG
   si += "document.style.variable('LineEndSze','10px');";
#else
   si += "document.style.variable('LineEndSze','0px');";
#endif

   eval(aux::chars(si.c_str(), si.length()));   //spustení js na UI stranì

   if (!firstEvent) { // for input styles
       controler->procesChangedInput(sciterStrToWStr(mathInput.get_value().to_string()));   //pøepoèítání pøíkladu // TODO -> jen updatnout styly
   }

   handle_size( sciter::dom::element::root_element(get_hwnd())   ); // refreshnout vse spojené s velikostí okna


   
};

sciter::dom::element CalculatrWin::getElementById(std::string id) {
    return ((element)element::root_element(get_hwnd())).get_element_by_id(id.c_str());
};

void CalculatrWin::handle_size(HELEMENT he) {
    debugLOG(" Handling Size");
    RECT rect;
    GetWindowRect(get_hwnd(), &rect);
    getElementById("appName").set_style_attribute("opacity", (rect.right - rect.left > 220 && settings.showAppName) ? L"1" : L"0");
}
