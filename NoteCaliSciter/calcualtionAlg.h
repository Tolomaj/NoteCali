#include <vector>
#include <string>

class mline {
	std::wstring line = L"";
	bool isSuperLine = false; // default from settings add
	bool isVariable = false;
	bool isGraph = false;
	bool hasNonExistencialVar = false;
	std::wstring localVariableName = L"";
	std::vector<std::wstring> dependecees;
    std::wstring solution = L"";
};

class matematicalSolver {
    std::wstring notFormatedInput; //remove
	std::vector<mline> lines;
	int prevousLineCount = -1;

	void updateSystemVariables() {

	}

	void separateLines() {

	}

	void solveDiferencingLines() {
          
	} 

	void solveLine(std::string s) {

	}

    int getLineCount(const std::wstring& output) {
        int totalLines = 0;
        for (int i = output.length(); i-- > 0;) {
            if (output.at(i) == '\n') {
                totalLines++;
            }
        }
        return totalLines;
    }


    std::wstring formateHighlights(const std::wstring& from, int newLineCount); // also formate lineEnds Used for Solutions

    std::wstring generateSolutionList(sciter::dom::element highlight, int newLineCount);

    void updateSolutionList(sciter::dom::element highlight, int newLineCount);

   

public:
    


    int processComands(std::wstring * str, std::wstring * comand) {
        int pos = str->find(L"@");
        if (pos == std::string::npos) {
            debugLOG("iiiidd");
            return -1;
        }
        for (size_t i = pos; i < str->length(); i++){
            if (str->at(i) == '\n') {
                *comand = str->substr(pos+1, i);
                debugLOG(L"COMAND STRS: " + *comand);
                str->erase(pos,i+1);
                return i;
            }
        }
        return -1;
    }

	void solve(std::wstring s) {
		debugLOG(s);
        notFormatedInput = s;
	}

    void publish(sciter::dom::element highlight, sciter::dom::element solutionsOutput, sciter::dom::element root) {
        // prevent <div></div> characters
        //add hinting sistem <hint></hint>
        // to > html add turnoffable linoe counting

        //processComands(&notFormatedInput);
        

        int nowLineCount = getLineCount(notFormatedInput);


        std::wstring htmlin = formateHighlights(notFormatedInput, nowLineCount); //add nevelement that hadles lineEnds
        aux::w2utf utf8(htmlin);
        highlight.set_html((LPCBYTE)utf8, utf8.length());

        highlight.update(); // update for corect solutuin sizing


        htmlin = generateSolutionList(highlight, nowLineCount); // cretes space for solutions (add next step counting)
        aux::w2utf utf82(htmlin);
        solutionsOutput.set_html((LPCBYTE)utf82, utf82.length());

        
        prevousLineCount = nowLineCount;
    }



}solver;

std::wstring matematicalSolver::formateHighlights(const std::wstring& from, int newLineCount) {
    if (from.empty()) { return L""; }
    std::wstring output = from;
    
    // highlights
    if (settings.ishighlitingOn()) {
        if (output.length() > 8) {
            output.insert(6, L"</mf>");
            output.insert(2, L"<mf>");
        }
    }

    //highlights end


    int lineCount = 0;
     for (int i = output.length(); i-- > 0;) {
        if (output.at(i) == '\n') {
            output.insert(i,L"<le id=\"le" + std::to_wstring(newLineCount - lineCount -1) + L"\">i</le>"); // to get vhere line ends
            lineCount++;
        } 
    }

  
    std::wstring a = output.at(output.length() - 1) == '\n' ? L"<ph>.</ph>" : L"";
    output.append( L"<le id=\"le" + std::to_wstring(newLineCount) + L"\">:</le>" + a); // to get vhere line ends
    return output;
}


POINT getScroll(sciter::dom::element element) {
    POINT scroll; RECT re; SIZE s;
    element.get_scroll_info(scroll, re, s);
    return scroll;
}


std::wstring matematicalSolver::generateSolutionList(sciter::dom::element highlight, int newLineCount) {     // crete string for solutions (depending on line width defacto stored in highlighte)
    std::wstring solutionString = L"";
    int prevousLineEndPosX = -settings.fontPadding;
    for (size_t i = 0; i <= newLineCount; i++){
        sciter::dom::element lineEnd = highlight.get_element_by_id((L"le" + std::to_wstring(i)).c_str());

        int LineEndPosX = lineEnd.get_location(PADDING_BOX).bottom - highlight.get_location(PADDING_BOX).top + getScroll(highlight).y;

        int LineCount = round((LineEndPosX - prevousLineEndPosX)/(settings.fontSize + 2.0f * settings.fontPadding));

        std::wstring resultString = L"id=\"molID" + std::to_wstring(i) + L"\" class=\"mathOutputLine\" style=\"padding: " + std::to_wstring((LineCount - 1) * (settings.fontSize + 2 * settings.fontPadding) / 2) + L"px 0;\">" + std::to_wstring(i) + L":" + std::to_wstring(LineCount);

        if (settings.clickToCopy) {
            solutionString.append(L"<button " + resultString + L"</button>");
        }else {
            solutionString.append(L"<span " + resultString + L"</span>");
        }
        prevousLineEndPosX = LineEndPosX;
    }
    return solutionString;
}

void matematicalSolver::updateSolutionList(sciter::dom::element highlight, int newLineCount) { // not working properly i chose regenreate eweri time
    int prevousLineEndPosX = -settings.fontPadding;
    for (size_t i = 0; i <= newLineCount; i++) {
        sciter::dom::element lineEnd = highlight.get_element_by_id((L"le" + std::to_wstring(i)).c_str());

        int LineEndPosX = lineEnd.get_location(PADDING_BOX).bottom - highlight.get_location(PADDING_BOX).top + getScroll(highlight).y;

        int LineCount = round((LineEndPosX - prevousLineEndPosX) / (settings.fontSize + 2.0f * settings.fontPadding));

        std::wstring resultString = L"id=\"molID" + std::to_wstring(i) + L"\" class=\"mathOutputLine\" style=\"padding: " + std::to_wstring((LineCount - 1) * (settings.fontSize + 2 * settings.fontPadding) / 2) + L"px 0;\">" + std::to_wstring(i) + L":" + std::to_wstring(LineCount);
        sciter::dom::element highlighArea = highlight.get_element_by_id((L"molID" + std::to_wstring(i)).c_str());
        int paddingSize = ((LineCount - 1) * (settings.fontSize + 2 * settings.fontPadding) / 2);
        debugLOG(paddingSize);
        debugLOG(LineCount);
        highlighArea.set_style_attribute("padding-top", std::to_wstring(paddingSize).c_str());
        highlighArea.set_style_attribute("padding-bottom",  std::to_wstring(paddingSize).c_str());

        prevousLineEndPosX = LineEndPosX;
    }
}