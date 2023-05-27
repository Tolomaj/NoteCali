
//pøipojení handlerù na SWITCH
function bindSwitch(element) {
    element.addEventListener("click", (e) => {
        var sw = e.currentTarget.getElementsByClassName("inp")[0];
        sw.checked = !sw.checked;
        Window.this.SettingsWin.setSwitch(sw.id, sw.checked);
        sw.focus();
    }, false);
}
var options = document.getElementsByClassName("option"); // nastavení chování možností
for (let i = 0; i < options.length; i++) {
    var inpE = options[i].getElementsByClassName("inp");
    if (inpE != 0 && inpE[0].tagName == "SWITCH") {
        bindSwitch(options[i]);
    }
}



function addToNumInput(inputObject, modifier) {
    var amout = (inputObject.getAttribute("step") == undefined) ? 0 : inputObject.getAttribute("step");
    inputObject.value = (inputObject.value == undefined) ? 0 : inputObject.value + amout * modifier;
    if (inputObject.value > inputObject.getAttribute("max")) {
        inputObject.value = inputObject.getAttribute("min")
    }
    if (inputObject.value < inputObject.getAttribute("min")) {
        inputObject.value = inputObject.getAttribute("max")
    }
    inputObject.focus();
    Window.this.SettingsWin.setNumInput(inputObject.id, inputObject.value);
    //pøidat callback to c++
}

var numberDecreasers = document.getElementsByClassName("rDwnB");
for (let i = 0; i < numberDecreasers.length; i++) {
    numberDecreasers[i].addEventListener("mousedown", (e) => {
        addToNumInput(e.target.parentElement.parentElement.getElementsByTagName("input")[0], -1);
    }, false);
}

var numberIncresers = document.getElementsByClassName("rUpB");
for (let i = 0; i < numberIncresers.length; i++) {
    numberIncresers[i].addEventListener("mousedown", (e) => {
        addToNumInput(e.target.parentElement.parentElement.getElementsByTagName("input")[0], 1);
    }, false);
}


