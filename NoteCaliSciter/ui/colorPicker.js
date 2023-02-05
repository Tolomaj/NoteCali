var nowPickingOption = "";
var nowPickingColorDestination;
var pickers = document.getElementsByClassName("colorpicker");   // get all color options that can open colorpicker

var isMouseDown = false;

function hsv_to_hsl(h, s, v) { var l = (2 - s) * v / 2; if (l != 0) { if (l == 1) { s = 0; } else if (l < 0.5) { s = s * v / (l * 2); } else { s = s * v / (2 - l * 2); } } return [h, s, l]; }
function hslToHex(h, s, l) { l /= 100; const a = s * Math.min(l, 1 - l) / 100; const f = n => { const k = (n + h / 30) % 12; const color = l - a * Math.max(Math.min(k - 3, 9 - k, 1), -1); return Math.round(255 * color).toString(16).padStart(2, '0'); }; return `#${f(0)}${f(8)}${f(4)}`; }

for (let i = 0; i < pickers.length; i++) {  // add click event to color picker
    pickers[i].addEventListener("click", (e) => {
        document.getElementById("pickerSelection").style.display = "block";
        nowPickingOption = e.target.getAttribute("id");
        nowPickingColorDestination = e.target;
    }, false);
}



var lastH, lastS, lastL, lastA;
function setColot() { // set color in prewiew
    var color = hsv_to_hsl(document.getElementById("hueSlider").value / 360, Math.floor(lastS) / 100, Math.floor(lastL) / 100);
    document.getElementById("colorPrew").style.background = "hsl(" + color[0] * 360 + ", " + color[1] * 100 + ", " + color[2] * 100 + ")";
    document.getElementById("colorPrew").style.opacity = document.getElementById("transparencitySlider").value / 100;
    document.getElementById("colorTInput").value = hslToHex(color[0] * 360, color[1] * 100, color[2] * 100);
    Window.this.SettingsWin.setSet(nowPickingOption, document.getElementById("colorTInput").value, "f"); //äpply color without saving
}







document.addEventListener('mouseup', function () { isMouseDown = false; }, true);

document.getElementById("sg").addEventListener("mousedown", (e) => { // mouse clicked on picking area
    isMouseDown = true;
    mouseEventResponse(e);  // change color only if mouse is down
}, false);

document.getElementById("sg").addEventListener("mousemove", (e) => {// mouse move on picking area
    mouseEventResponse(e);// change color only if mouse is down
}, false);

function mouseEventResponse(e) {
    if (isMouseDown) {
        var boundaryRect = document.getElementById("sg").getBoundingClientRect(); 

        var yCoords = Math.min(Math.max((e.clientY - boundaryRect.top) / boundaryRect.height, 0), 1); // get coordiantes in box 0 - 1
        var xCoords = Math.min(Math.max((e.clientX - boundaryRect.left) / boundaryRect.width, 0), 1); // get coordiantes in box 0 - 1

        document.getElementById("dragDot").style.marginTop = yCoords * document.getElementById("sg").getBoundingClientRect().height - 5;
        document.getElementById("dragDot").style.marginLeft = xCoords * document.getElementById("sg").getBoundingClientRect().width - 5;

        lastL = 100 - yCoords * 100;
        lastS = xCoords * 100;

        setColot();
    }
}








document.getElementById("hueSlider").addEventListener("change", function (evt) {
    document.getElementById("bcg").style.background = "hsl(" + document.getElementById("hueSlider").value + ", 100%, 50%)";
    setColot();
});
document.getElementById("transparencitySlider").addEventListener("change", function (evt) { setColot(); });


document.getElementById("pickingSave").onclick = function () { // save seted color
    nowPickingColorDestination.style.backgroundColor = document.getElementById("colorTInput").value;
    Window.this.SettingsWin.setSet(nowPickingOption, document.getElementById("colorTInput").value, "t");
    document.getElementById("pickerSelection").style.display = "none";
}

document.getElementById("pickingCanceled").onclick = function () { // reset prewiews color
    Window.this.SettingsWin.setSet(nowPickingOption, nowPickingColorDestination.style.backgroundColor, "t");
    document.getElementById("pickerSelection").style.display = "none";
}
