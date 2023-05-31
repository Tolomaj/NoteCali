function attachDestroys() {
    var variables = document.getElementsByClassName("var");
    console.log("attaching to:");

    for (let i = 0; i < variables.length; i++) {
        var destroys = variables[i].getElementsByClassName("destroy");
        console.log(destroys);
        if (destroys.length > 0) {
            console.log(destroys[0]);
            destroys[0].addEventListener("mousedown", (e) => {
                console.log("delBt");
                e.target.parentElement.remove();
            }, false);
        }
    }
}

function loadVariables(variableLines) {
   var varArr = variableLines.split(';');
    var vararea = document.getElementById("varArea");
    vararea.innerHTML = '<div class="var system"><div class="name">system : pi</div><div class="value">3.141...</div></div><div class="var system" style="margin-bottom:15px"><div class="name">system : e</div><div class="value">2.718...</div></div>'
    for (let i = 0; i < varArr.length; i = i + 2) {
        var elementStr = '<div class="var"> <input type="text" class="name" novalue="name" value="' + varArr[i] + '" /> <input type="number" class="value" value="' + varArr[i + 1] + '" novalue="value"/> <img class="destroy" src="src/trash.svg"/>  </div>';
        vararea.insertAdjacentHTML('beforeend', elementStr);
    }
    vararea.focus(); // to placeholders procedded corectly.
    // attach destroys
    attachDestroys();
}

// add save button
function filterValid() {
    const variables = document.querySelectorAll('.var:not(.system)');
    variables.forEach(variable => { // ty poté smaže 
        if (variable.getElementsByClassName("name")[0].value == "" || variable.getElementsByClassName("value")[0].value == undefined) {
            variable.remove();
        }
    });
}

function tagUnvalid() {
    //make red all unvalid variables
}

document.getElementById("varAddBTN").onclick = function () {
    var vararea = document.getElementById("varArea");
    var elementStr = '<div class="var"> <input type="text" class="name" novalue="name" value="" /> <input type="number" class="value" value="" novalue="value"/> <img class="destroy" src="src/trash.svg"/>  </div>';
    vararea.insertAdjacentHTML('beforeend', elementStr);
    vararea.focus(); // to placeholders procedded corectly.
    attachDestroys();
}
document.getElementById("varLoadBTN").onclick = function () {
    //filterValid();
    const variables = document.querySelectorAll('.var:not(.system)');//.getElementsByClassName("var");
    var variableList = "";
    for (let i = 0; i < variables.length; i++) {
        if (variables[i].getElementsByClassName("name")[0].value != "" || variables[i].getElementsByClassName("value")[0].value != undefined ) {
            variableList += variables[i].getElementsByClassName("name")[0].value + " = " + String(variables[i].getElementsByClassName("value")[0].value) + "\n";
        }
    }


    Window.this.SettingsWin.saveVariables(variableList);
}