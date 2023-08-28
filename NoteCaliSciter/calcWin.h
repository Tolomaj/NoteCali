#include <cctype>

#define KEY_PRESED_DOWN 32769
#define LEFT_ALT 4
#define RIGHT_ALT 5
#define CTRL 1

#define COLOR_WARNING L"#E57C23"
#define COLOR_ERROR L"#DF2E38"
#define COLOR_MSG L"#10819A"
#define COLOR_OK L"#36852c"

class CalculatrWin : public sciter::window {
private:
    bool isSnipetOpen = false;
    bool windowIsTopMOST = false;
    Controler* controler;   // odkaz na controler pro volání eventù
    LineRegister* lineSeparator; // odkaz na obìkt který rozdeluje text an linky

    bool firstEvent = true; //  první naètení elementù
    sciter::dom::element highites; //odkaz na èasto používané elementy
    sciter::dom::element mathInput; //odkaz na èasto používané elementy
    sciter::dom::element mathOutput; //odkaz na èasto používané elementy

#define PREV_CHARS_NUM 3    //poèet preventovaných znaku v html
    const wchar peventedC[PREV_CHARS_NUM] = { L'<'   ,L'>'   ,L'&' };   //preventovane znaky
    const wstring peventedAlias[PREV_CHARS_NUM] = { L"&lt;",L"&gt;",L"&amp;" }; // alias pro preventovany znak


public:
    CalculatrWin(Controler* controler, LineRegister* lineSeparator) : window(SW_POPUP, { (MONITOR_WIDTH - CALC_WIN_WIDTH) / 2, (MONITOR_HEIGHT - CALC_WIN_HEIGHT) / 2 , (MONITOR_WIDTH + CALC_WIN_WIDTH) / 2, (MONITOR_HEIGHT + CALC_WIN_HEIGHT) / 2 }) {
        this->controler = controler;
        this->lineSeparator = lineSeparator;
    }

    sciter::dom::element getElementById(std::string id);

    void updateStyles();   // pøekreslí styl okna.

    void handle_size(HELEMENT he);  // updatne jak vypadá okno schová jméno když je moc malé

    void toggleSettingsWin();   // otevøe/zavøe okno nastavení

    void setText(std::wstring dta, bool focus = true);  // nastaví text userinputu

    POINT getScroll(sciter::dom::element* highites);

    void handleScroll();

    void preventFuncChars(mline* line);   //upravi linie tak aby neobsahovani funkcni zanky pro html jejich pseudonymi

    void publish(std::vector<mline> lines); // publish solutions and highlights

    void focus();

    void showNotification(wstring NotificationText, wstring bColor);

    void togleSnippetHelp();

    void reCalculateInput() {
        controler->procesChangedInput(sciterStrToWStr(mathInput.get_value().to_string()));
    }

    void moveCursorDown(sciter::dom::element textAreaElem); // posune kurzor na niší øádek

    void moveCursorUp(sciter::dom::element textAreaElem); // posune kurzor na višší øádek

    void moveCursor(sciter::dom::element textAreaElem, int many);

    virtual bool on_key(HELEMENT he, HELEMENT target, UINT event_type, UINT code, UINT keyboardStates) { 
        
        debugLOG("kc " + to_string(code) + " *-* ks " + to_string(keyboardStates) + " tpe s " + to_string(event_type));

        if(code == 259 /*backspace key code*/ && mathInput.get_value().to_string().length() <= 0) { return true; } // prevent backspace v prázdné text area // když je zmáèknut backsapce na prázdné text area program spande

        if (code <= 265 && code >= 262) { // TODO pøidat do astavení nožná
            handleScroll();
            if(event_type == KEY_PRESED_DOWN-1){
                if (code == 262) {
                    moveCursor(mathInput, +1); // posnutí kurzoru o jedna do prava
                }
                if (code == 263) { // šipka do leva
                    moveCursor(mathInput, -1); // posnutí kurzoru o jedna do leva
                }
                if (code == 264) {
                    moveCursorDown(mathInput);
                }
                if (code == 265) {
                    moveCursorUp(mathInput);
                }

            }
            return true;// prevence šipek a nastavení vlastního chování 
        }

        if (event_type == KEY_PRESED_DOWN && code == 261) {
            mathInput.set_text(L"");
            mathOutput.set_text(L"");
            highites.set_html((LPCBYTE)"", 0);
            highites.update();
            return true;// prevence šipek a nastavení vlastního chování 
        }

        if (keyboardStates == RIGHT_ALT && code == 86) { return true; } // prevent rightAlt+v kvuli @ když je vloženo levím ctrl

        if (keyboardStates == CTRL && event_type == KEY_PRESED_DOWN){ // keyboardStates == 1 is ctrl
            if (code == 70) { // ctrl + f
                mathInput.set_text(L"");
                mathOutput.set_text(L"");
                highites.set_html((LPCBYTE)"", 0);
                highites.update();
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

        //debugLOG("something with:" + std::to_string(params.cmd) + " - " + WstrToStr(params.name) + " - " + std::to_string(params.reason) + " - " + sciterStrToStr(elementId));
       
        if (params.cmd == 193) { debugLOG("EV - Closing Program."); this->close(); PostQuitMessage(WM_QUIT); return true; }// handlování zavøení okna

        if (firstEvent && params.cmd == DOCUMENT_READY) { //naètení èasto používaných obìktù
            debugLOG("EV - CALC - HTML loaded. Geting Main Elements Objects.");
            highites = getElementById("highlights");
            mathInput = getElementById("mathInput");
            mathOutput = getElementById("mathOutput");
            mathInput.eval(aux::utf2w("this.focus();")); // not work probably TODO
            firstEvent = false;
        }

        if (params.cmd == EDIT_VALUE_CHANGED) {  // zmenilo se pole zadání vypocitat ho a publishnout
            debugLOG("EV - CALC - Text Value Changed");
            controler->procesChangedInput(sciterStrToWStr(target.get_value().to_string()));
            handleScroll();
            return true; 
        }
  
        if (((params.cmd == 32928 || params.cmd == 160) && ( target.get_element_uid() == mathInput.get_element_uid() ))) {
            debugLOG("EV - CALC - Scroll.");
            handleScroll(); return true;  // øesení scrollovaní
        }

        if (((params.cmd == 32928 || params.cmd == 160) && target.get_element_uid() == mathOutput.get_element_uid())) {
            debugLOG("EV - CALC - Scroll V.");
            mathInput.call_method("scrollSincV");
            return true;  // øesení scrollovaní
        }
        
        if (params.cmd == BUTTON_CLICK) {// set window top most
            debugLOG(wstring(L"EV - CALC - Click. elemID: ") + elementId);
            if (target.get_attribute("class") == L"mathOutputLine") { toClipboard(get_hwnd(), sciterStrToStr(target.get_attribute("val", L"0"))); return true; } // copy to click. zkopírování hodnoty do clipboardu
            if (elementId == L"shHide") { togleSnippetHelp(); return true; }
            
            wstring snip;
            if (target.get_tag() == "img") {
                snip = ((element)target.parent()).get_attribute("snip");
            } else {
                snip = target.get_attribute("snip");
            }
            if (snip != L"") {

                int end = snip.find(L"|");
                //snip.substr(end, snip.length());

                mathInput.call_method("insertTextEx", snip.substr(0, end), snip.substr(end+1, snip.length()));

                //std::string si = "this.textarea.insertText('" + WstrToStr(snip.substr(0, end)) + "')";
                //mathInput.eval(aux::utf2w(si));

                return true;
            }
            if (elementId == L"closeB") {  this->close(); PostQuitMessage(WM_QUIT); return true; } //zavreni okna
            if (elementId == L"pinB") {
                SetWindowPos(get_hwnd(), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
                getElementById("pinB").set_style_attribute("display", L"none");
                getElementById("unpinB").set_style_attribute("display", L"block");
                return true; 
            }
            if (elementId == L"unpinB") { // unset window top most
                SetWindowPos(get_hwnd(), HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
                getElementById("pinB").set_style_attribute("display", L"block");
                getElementById("unpinB").set_style_attribute("display", L"none");
                return true;
            }
            if (elementId == L"meuB") { controler->toggleSettings(); return true; } //otevreni/ zavreni nastaveni
        }
        return false;
    }

 
};


void CalculatrWin::moveCursor(sciter::dom::element textAreaElem,int many) { // many -> o kolik se kurzor posune do prava
    int pos = textAreaElem.eval(aux::utf2w("this.textarea.selectionStart")).get(0) + many;
    if (pos < 0) {
        pos = 0;
    }
    textAreaElem.eval(aux::utf2w("this.textarea.selectRange(" + to_string(pos) + "," + to_string(pos) + ")")); // nastaví kurzor na pøíslunou pozici
};




void CalculatrWin::moveCursorUp(sciter::dom::element textAreaElem) {
    int pos = textAreaElem.eval(aux::utf2w("this.textarea.selectionStart")).get(0);
    sciter::string txt = textAreaElem.get_value().to_string();

    int len = 0, lineStart = 0, pastLineStart = 0;

    for (int i = pos - 1; i > 0; i--) {
        if (txt[i] == '\n') {
            lineStart = i + 1;
            break;
        }
    }

    for (int i = lineStart - 2; i > 0; i--) {
        if (txt[i] == '\n') {
            pastLineStart = i + 1;
            break;
        }
    }

    int fromLineStart = pos - lineStart;
    int nvPos = pastLineStart + fromLineStart;
    int nextLineLen = lineStart - pastLineStart;

    if (nvPos < 0 || lineStart - 1 <= 0) {
        textAreaElem.eval(aux::utf2w("this.textarea.selectRange( 0 , 0)")); // nastaví kurzor na pøíslunou pozici
        return;
    }

    if (fromLineStart > nextLineLen - 1) {
        textAreaElem.eval(aux::utf2w("this.textarea.selectRange(" + to_string(lineStart - 1) + "," + to_string(lineStart - 1) + ")")); // nastaví kurzor na pøíslunou pozici
    }
    else {
        //if(pastLineStart != lineStart-1){
        textAreaElem.eval(aux::utf2w("this.textarea.selectRange(" + to_string(nvPos) + "," + to_string(nvPos) + ")")); // nastaví kurzor na pøíslunou pozici
        //}
    }



}

void CalculatrWin::moveCursorDown(sciter::dom::element textAreaElem) {
    int pos = textAreaElem.eval(aux::utf2w("this.textarea.selectionStart")).get(0);
    sciter::string txt = textAreaElem.get_value().to_string();

    int lineStart = 0, nextLineStart = txt.length(), nextLineEnd = txt.length();

    for (int i = pos - 1; i > 0; i--) {
        if (txt[i] == '\n') {
            lineStart = i + 1;
            break;
        }
    }

    for (int i = pos; i < txt.length(); i++) {
        if (txt[i] == '\n') {
            nextLineStart = i + 1;
            break;
        }
    }

    for (int i = nextLineStart; i < txt.length(); i++) {
        if (txt[i] == '\n') {
            nextLineEnd = i + 1;
            break;
        }
    }

    int fromLineStart = pos - lineStart;
    int nvPos = nextLineStart + fromLineStart;

    if (fromLineStart >= nextLineEnd - nextLineStart) {
        if (nextLineEnd == txt.length()) {
            textAreaElem.eval(aux::utf2w("this.textarea.selectRange(" + to_string(nextLineEnd) + "," + to_string(nextLineEnd) + ")")); // nastaví kurzor na pøíslunou pozici
        } else {
            textAreaElem.eval(aux::utf2w("this.textarea.selectRange(" + to_string(nextLineEnd - 1) + "," + to_string(nextLineEnd - 1) + ")"));
        }
    } else {
        if (nextLineStart != nextLineEnd) {
            textAreaElem.eval(aux::utf2w("this.textarea.selectRange(" + to_string(nvPos) + "," + to_string(nvPos) + ")"));
        }
    }
}

void CalculatrWin::togleSnippetHelp() {
    if (isSnipetOpen) {
        eval(const_chars("document.getElementById('snipetHelp').style.bottom = '-50px';"));
        getElementById("shHimg").set_attribute("src", L"src/pushUp.svg");
    } else {
        eval(const_chars("document.getElementById('snipetHelp').style.bottom = '10px';"));
        getElementById("shHimg").set_attribute("src", L"src/pushDown.svg");
    }
    isSnipetOpen = !isSnipetOpen;
};

void CalculatrWin::showNotification(wstring NotificationText, wstring bColor) {
    getElementById("notification").set_text(NotificationText.c_str());
    getElementById("notification").set_style_attribute("background-color", bColor.c_str()); // orange color

    eval(const_chars("setTimeout(function() {document.getElementById('notification').style.display = 'block';document.getElementById('notification').style.opacity = 100}, 500);"));
    eval(const_chars("setTimeout(function() { document.getElementById('notification').style.opacity = 0 }, 4000);"));
    eval(const_chars("setTimeout(function() { document.getElementById('notification').style.display = 'none' }, 4500);"));
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
        htmlin.append(lines.at(i).originLine + L"<le id=\"le" + std::to_wstring(i) + L"\">i</le>\n");
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

        double paddingBtwLines = 0;
        double solutionLineHeight = (settings.fontSize + 4) * LineCount; // <- zahynu
                       

        //V / pøidat pøedání nesolvnut=
        solutionString.append(L"<button id=\"molID" + std::to_wstring(i) + L"\" class=\"mathOutputLine\" val=\"" + lines.at(i).solution + L"\" style=\"padding: " + std::to_wstring(paddingBtwLines) + L"px 0; height:" + std::to_wstring(solutionLineHeight) + L"\" >");
        solutionString.append(lines.at(i).solutionModifier + L" ");
        if (lines.at(i).localVariableName != L"") {
            solutionString.append(lines.at(i).localVariableName);
        }
        if (lines.at(i).error.type <= 0 || settings.showErrText) { // dont print errors
            solutionString.append(lines.at(i).solution); // composite non copiable part with copiable part of solution
            debugLOG("solution:");
            debugLOG(lines.at(i).solution);
        }
        solutionString.append( L"</button>"); // composite non copiable part with copiable part of solution

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
    mathInput.call_method("scrollSinc");
    //POINT scroll = getScroll(&mathInput);
    //highites.set_scroll_pos(scroll);
    //mathOutput.set_scroll_pos(scroll);
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
   getElementById("highlights").set_style_attribute("opacity", L"1");
#endif

   if (settings.showSnipet) {
       getElementById("snipetHelp").set_style_attribute("display", L"inline");
       getElementById("shHide").set_style_attribute("display", L"inline-flex");
   } else {
       getElementById("snipetHelp").set_style_attribute("display", L"none");
       getElementById("shHide").set_style_attribute("display", L"none");
   }

   if (settings.snipetAlwaisVisible){
       getElementById("shHide").set_style_attribute("display", L"none");
       getElementById("snipetHelp").set_style_attribute("width", L"calc(100% - 24px)");
       if (isSnipetOpen == false) {
           togleSnippetHelp();
       }
   } else {
       if (settings.showSnipet) {
           getElementById("shHide").set_style_attribute("display", L"inline-flex");
           getElementById("snipetHelp").set_style_attribute("width", L"calc(100% - 70px)");
           if (isSnipetOpen == true) {
               togleSnippetHelp();
           }
       }
   }

   eval(aux::chars(si.c_str(), si.length()));   //spustení js na UI stranì a nastavý styli

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
