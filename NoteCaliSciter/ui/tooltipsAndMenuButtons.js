var selectedTabName = "Style";
var tabDescription = "Here is style settings";

function selectTab(btnName, tabName, tooltipisOn, description = "neni") { // otevøení tabu a nastavení defaultního typu

    selectedTabName = tabName.charAt(0).toUpperCase() + tabName.replace('Tab', '').slice(1);
    tabDescription = description;

    setStyleToolTip(selectedTabName, description);



    document.getElementById("tabWithToolTip").style.display = tooltipisOn ? "block" : "none";

    var tabs = document.getElementsByClassName("tabArea");
    for (let i = 0; i < tabs.length; i++) { tabs[i].style.display = "none"; }

    var tabButtons = document.getElementsByClassName("tabButton");
    for (let i = 0; i < tabButtons.length; i++) { tabButtons[i].classList.remove("selected"); } // unselect all buttons

    document.getElementById(btnName).classList.add("selected"); // select seleected button
    document.getElementById(tabName).style.display = "block";
}


document.getElementById("styleBtn").onclick = function () { selectTab("styleBtn", "styleTab", true, "Here is style settings.<br>If you want custom colors you must select custom profile."); }
document.getElementById("interfaceBtn").onclick = function () { selectTab("interfaceBtn", "interfaceTab", true, "This tab describes haw to comunicate with calculator.<br> And calculator Features"); }
document.getElementById("systemBtn").onclick = function () { selectTab("systemBtn", "systemTab", true, "Here you can set system settings. All bohavior can by changed there"); }
document.getElementById("variableBtn").onclick = function () { selectTab("variableBtn", "variablesTab", true, "Here is your variales<br>You can add more Variables in file opened by button down there<br>For wariables to work you must hit reload button or restart aplication"); }
document.getElementById("functionBtn").onclick = function () { selectTab("functionBtn", "functionsTab", true, "There you can set Functions"); }
document.getElementById("feedbackBtn").onclick = function () { selectTab("feedbackBtn", "feedbackTab", false); }
document.getElementById("updatesBtn").onclick = function () { selectTab("updatesBtn", "updatesTab", false); }


function setStyleToolTip(name, description) {
    document.getElementById("toolTip").getElementsByClassName("tip")[0].innerHTML = description;
    document.getElementById("toolTip").getElementsByClassName("header")[0].innerHTML = name;
};

for (const hasTipElement of document.getElementsByClassName("hasTip")) { // všem elementùm classy hasTip
    hasTipElement.addEventListener("mouseover", (e) => {
        var label = e.currentTarget.getElementsByClassName("oLable");
        var TipName = (label != 0) ? label[0].innerHTML : e.currentTarget.innerHTML; // if dont have olabel class then is geted all iner html !
        setStyleToolTip(TipName, e.currentTarget.getAttribute("tip"));
    }, false);
    hasTipElement.addEventListener("mouseleave", (e) => {
        setStyleToolTip(selectedTabName, tabDescription);
    }, false);
}

for (const categoryButton of document.getElementsByClassName("categoryButton")) { // attach event for all buttons // categories
    categoryButton.addEventListener("click", (e) => {
        for (const categoryButton of document.getElementsByClassName("categoryButton")) { categoryButton.style.background = "#2B6CA8"; } // all buttons to unselected color
        e.currentTarget.style.background = "#205280"; // self is selected

        var displayOU = (e.currentTarget.getAttribute("value") == "3") ? "inline" : "none"; // add notclickable for user to know what hapend
        for (const customOpt of document.getElementsByClassName("customPa")) { customOpt.style.display = displayOU; } // if is custom tab show all buttons if not show only noncustom
    }, false);
}

function SetOpenedCategory(i) {
    var categoryButton = document.getElementsByClassName("categoryButton");
    for (const categoryButton of document.getElementsByClassName("categoryButton")) { categoryButton.style.background = "#2B6CA8"; } // all buttons to unselected color
    categoryButton[i].style.background = "#205280"; // self is selected

    var displayOU = (i == "3") ? "inline" : "none"; // add notclickable for user to know what hapend

    for (const customOpt of document.getElementsByClassName("customPa")) { customOpt.style.display = displayOU; } // if is custom tab show all buttons if not show only noncustom
}

//Window.this.SettingsWin.loadSettingsInWindow();