//////////////////////
/**  UPLOAD IMG    **/
//////////////////////

//set global var
//define the result size (in pixels) of the image crop
var IMG_HEIGH = 47;
var IMG_WIDTH = 47;

//load file from input file and display a preview of its content
function readImgPath(input) {
    if(input.files && input.files[0]) {
        var reader = new FileReader();
        reader.onload = function (e) {
            $('#preview')
                .attr('src', e.target.result)
                .width(IMG_HEIGH)
                .height(IMG_WIDTH);
            //do not change the height and width
        };
        reader.readAsDataURL(input.files[0]);
    }
}

//init send function with a picture
function sendFromImg(img_id){
    var img = document.getElementById(img_id);
    var canvas = getCanvasToSend(img); //convert picture to canvas
    postData(getDataToSend(canvas));
}

//init send function with a specified canvas
function sendFromCanvas(canvas_id){
    var canvas = document.getElementById(canvas_id);
    var img = document.createElement("img"); // create img tag
    img.src = canvas.toDataURL();
    img.width = IMG_WIDTH;
    img.height = IMG_HEIGH;
    var canvas_tosend = getCanvasToSend(img);
    postData(getDataToSend(canvas_tosend));
}

//init send function with the gallery canvas
function sendFromGallery(){
    var filename = document.getElementById("select_gallery").value;
    var img = document.createElement("img"); // create img tag
    img.src = filename;
    img.width = IMG_WIDTH;
    img.height = IMG_HEIGH;
    var canvas = getCanvasToSend(img);
    postData(getDataToSend(canvas));
}

//init send function with the text canvas
function sendFromText(canvasName){
    var canvas = document.getElementById(canvasName);
    
    var pixelsData = [];
    for (var i = 0; i < 23; i++) {
        var row = [];
        for (var j = 0; j < 72; j++) {
            var pixel = canvas.getContext('2d').getImageData(j, 22-i, 1, 1).data;
            var r = pixel[0];
            var g = pixel[1];
            var b = pixel[2];
            var tmp = rgbToHex(r, g, b); 
            row.push(tmp);
        }
        pixelsData.push(row);
    }
    
    //for pixels.html
    localStorage.setItem('color_array', JSON.stringify(pixelsData));
    
    var toSend = {};
    toSend.i = 23;
    toSend.j = 72;
    toSend.image = pixelsData;
    
    postData(toSend);
}

//convert img to canvas
function getCanvasToSend(img){
    var canvas = document.createElement('canvas');
    canvas.width = window.IMG_WIDTH;
    canvas.height = window.IMG_HEIGH;
    canvas.getContext('2d').drawImage(img, 0, 0, img.width, img.height);
    return canvas;
}

//////////////////////
/**   IMG GEN      **/
//////////////////////

// generate data from canvas
// return an array of 23*72 which contains color code in hex format
function getDataToSend(canvas){
    var pixelsData = [];
    for (var i = 0; i < 47; i++) {
        var row = [];
        for (var j = 0; j < 47; j++) {
            var pixel = canvas.getContext('2d').getImageData(j, i, 1, 1).data;
            var r = pixel[0];
            var g = pixel[1];
            var b = pixel[2];
            var tmp = rgbToHex(r, g, b); 
            //var tmp = [r,g,b]
            row.push(tmp);
        }
        pixelsData.push(row);
    }
    
    //for pixels.html
    localStorage.setItem('color_array', JSON.stringify(pixelsData));
    
    var colorsTab = [];
    for (var led = 0; led < 23; led++){
        var led_circle = [];
        for( var pos = 0; pos < 72; pos++){
            var teta =  2*Math.PI*pos*5/360; //conversion from deg to radian
            x = Number(led*Math.cos(teta) + 23).toFixed(0); //x position in a polar scale
            y = Number(led*Math.sin(teta) + 23).toFixed(0); //y position in a polar scale
            led_circle.push(pixelsData[y][x]);
        }
        colorsTab.push(led_circle);
    }
    var toReturn = {};
    toReturn.i = 23; //number of led per branch
    toReturn.j = 72; //number of states during a complete rotatition
    toReturn.image = colorsTab;
    return toReturn;
}

//////////////////////
/**   GEN TOOLS    **/
//////////////////////

//convert color from rgb to hex
//source of the below function : http://www.jquery4u.com/jquery-functions/jquery-convert-rgb-hex-color/
function rgb2hex(rgb) { 
    rgb = rgb.match(/^rgb\((\d+),\s*(\d+),\s*(\d+)\)$/); 
    return "#" 
        + ("0" + parseInt(rgb[1], 10).toString(16)).slice(-2)
        + ("0" + parseInt(rgb[2], 10).toString(16)).slice(-2)
        + ("0" + parseInt(rgb[3], 10).toString(16)).slice(-2);
}

//convert color from component (red, blue or green) to hex
function componentToHex(c) {
    var hex = c.toString(16);
    return hex.length == 1 ? "0" + hex : hex;
}

//convert color from rgb to hex
function rgbToHex(r, g, b) {
    return componentToHex(r) + componentToHex(g) + componentToHex(b);
}

//////////////////////
/**   IMG POST     **/
//////////////////////

function postData(data){    
    //store in localStorage for simulator.html and pixels.html
    localStorage.setItem('image', JSON.stringify(data));
    
    //define url of the server
    var url = "http://192.168.1.34/image";
    var method = 'POST';
    
    // You REALLY want async = true.
    // Otherwise, it'll block ALL execution waiting for server response.
    var async = true;
    
    // Make a request for each circle (all state of a led for a total rotation)
    // due to request size treatment problems on ESP board
    for(var led = 0; led < 23; led++){
        var request = new XMLHttpRequest();
        var toSend = "" + pad(led,3);
        for(var index = 0 ; index < 72 ; index++){
            toSend += ',' + data.image[led][index];
            
        }
        
        request.open(method, url, async);
        request.setRequestHeader('Content-type', 'application/x-www-form-urlencoded');
        request.onload = function () {
            var status = request.status; // HTTP response status, e.g., 200 for "200 OK"
            var data = request.responseText; // Returned data, e.g., an HTML document.
            console.log(request);
        }
        var formData = new FormData();
        formData.append("body",toSend);
        setTimeout(request.send(formData),500*led);
        console.log(request);
    }
    

}

//////////////////////
/**   PAGE TOOLS   **/
//////////////////////

//go to a section from home
function nav(section){
    $("#home").addClass('hide');
    $("#"+section).removeClass('hide');
    $("#"+section).addClass('active');
}

//go back to home from a section
function goHomeFrom(section){
    $("#"+section).removeClass('active');
    $("#"+section).addClass('hide');
    $("#home").removeClass('hide');
}

//clean the drawing canvas
function clearCanvas(){
    var canvas = document.getElementById('colors_sketch');
    canvas.getContext('2d').clearRect(0,0, canvas.height, canvas.width);
}

function initImagePicker(){
    var dir = "pictures/";
    var ext = ".png";
    
    $.ajax({
        //This will retrieve the contents of the folder if the folder is configured as 'browsable'
        url: dir,
        success: function (data) {
            //List all .png file names in the page
            $(data).find("a:contains(" + ext + ")").each(function () {
                var filename = this.href.replace(window.location.host, "").replace("http://", "");
                
                var opt = "<option data-img-src='" + filename + "' height=80px width=80px value='" + filename + "'></option>"                
                $("#select_gallery").append(opt);
                // */
            });
        },
        complete: function (data) {
            $("#select_gallery").imagepicker();
        }
    });    
}

function initTextInput(){
    var canvas = document.getElementById('textCanvas');
    var context = canvas.getContext('2d');
    var imgElement = document.getElementById('textImg');
    
    //add event listener on the text input
    //for each tap, it will convert text to canvas which will bu use to generate data to send to the esp
    document.getElementById('textinput').addEventListener('keyup', function (){
        var value = this.value.toLocaleString().toUpperCase(); //put to upper case the value of the input
        context.font="20px Verdana"; //Set font
        context.fillStyle = "#FF0000";; //set color
        context.fillText(value, 0, 17); //set the text in the canvas with a specified height               
        imgElement.src = context.canvas.toDataURL(); //display the 
    }, false);
}

$(document).ready(function(){
    //init canvas for drawing
    //init color picker
    $.each(['#f00', '#ff0', '#0f0', '#0ff', '#00f', '#f0f', '#000', '#fff'], function() {
      $('#colors_demo .tools').append("<a href='#colors_sketch' data-color='" + this + "' style='width: 10px; background: " + this + ";'></a> ");
    });
    //init size picker
    $.each([15, 20, 25], function() {
      $('#colors_demo .tools').append("<a href='#colors_sketch' data-size='" + this + "' style='background: #ccc'>" + this + "</a> ");
    });
    //add the clear button
    $('#colors_demo .tools').append("<a onclick='clearCanvas()' data-size='" + this + "' style='background: #ccc'>Clean</a> ");
    $('#colors_sketch').sketch();
    
    //init gallery
    initImagePicker();
    
    //init text input
    initTextInput();
});

//normalize the number of digit of a number (int)
function pad(number, length) {
    var str = '' + number; //convert number from int to string
    while (str.length < length) {
        str = '0' + str; //add '0' at the begin of the string
    }
   
    return str;
}