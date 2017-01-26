//////////////////////
/**  UPLOAD IMG    **/
//////////////////////

var IMG_HEIGH = 47;
var IMG_WIDTH = 47;

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

function sendFromImg(img_id){
    var img = document.getElementById(img_id);
    var canvas = getCanvasToSend(img);
    $("#here").append(canvas);
    postData(getDataToSend(canvas));
}

function sendFromCanvas(canvas_id){
    var canvas = document.getElementById(canvas_id);
    var img = document.createElement("img"); // create img tag
    img.src = canvas.toDataURL();
    img.width = IMG_WIDTH;
    img.height = IMG_HEIGH;
    var canvas_tosend = getCanvasToSend(img);
    $("#colors_demo").append(img);
    postData(getDataToSend(canvas_tosend));
}

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

function getDataToSend(canvas){
    var pixelsData = [];
    for (var i = 0; i < 47; i++) {
        var row = [];
        for (var j = 0; j < 47; j++) {
            var pixel = canvas.getContext('2d').getImageData(i, j, 1, 1).data;
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
            var teta =  2*Math.PI + 2*Math.PI*pos*5/360;
            x = Number(led*Math.cos(teta) + 23).toFixed(0);
            y = Number(led*Math.sin(teta) + 23).toFixed(0);
            led_circle.push(pixelsData[y][x])
        }
        colorsTab.push(led_circle);
    }
    var toReturn = {};
    toReturn.i = 23;
    toReturn.j = 72;
    toReturn.image = colorsTab;
    return toReturn;
}

//////////////////////
/**   GEN TOOLS    **/
//////////////////////

//source of the below function : http://www.jquery4u.com/jquery-functions/jquery-convert-rgb-hex-color/
function rgb2hex(rgb) { 
    rgb = rgb.match(/^rgb\((\d+),\s*(\d+),\s*(\d+)\)$/); 
    return "#" 
        + ("0" + parseInt(rgb[1], 10).toString(16)).slice(-2)
        + ("0" + parseInt(rgb[2], 10).toString(16)).slice(-2)
        + ("0" + parseInt(rgb[3], 10).toString(16)).slice(-2);
}

function componentToHex(c) {
    var hex = c.toString(16);
    return hex.length == 1 ? "0" + hex : hex;
}

function rgbToHex(r, g, b) {
    return componentToHex(r) + componentToHex(g) + componentToHex(b);
}

//////////////////////
/**   IMG POST     **/
//////////////////////

function postData(data){    
    //for simu.html
    localStorage.setItem('image', JSON.stringify(data.body))
    var url = "http://192.168.2.101/image/";
    var method = "POST";
    // You REALLY want async = true.
    // Otherwise, it'll block ALL execution waiting for server response.
    var async = true;

    var request = new XMLHttpRequest();

    // Before we send anything, we first have to say what we will do when the
    // server responds. This seems backwards (say how we'll respond before we send
    // the request? huh?), but that's how Javascript works.
    // This function attached to the XMLHttpRequest "onload" property specifies how
    // the HTTP response will be handled. 
    request.onload = function () {

       // Because of javascript's fabulous closure concept, the XMLHttpRequest "request"
       // object declared above is available in this function even though this function
       // executes long after the request is sent and long after this function is
       // instantiated. This fact is CRUCIAL to the workings of XHR in ordinary
       // applications.

       // You can get all kinds of information about the HTTP response.
       var status = request.status; // HTTP response status, e.g., 200 for "200 OK"
       var data = request.responseText; // Returned data, e.g., an HTML document.
       console.log(request);
    }

    request.open(method, url, async);

    request.setRequestHeader("Content-Type", "application/json;charset=UTF-8");
    // Or... request.setRequestHeader("Content-Type", "text/plain;charset=UTF-8");
    // Or... whatever

    // Actually sends the request to the server.
    request.send(data);
}

//////////////////////
/**   PAGE TOOLS   **/
//////////////////////

function nav(section){
    $("#home").addClass('hide');
    $("#"+section).removeClass('hide');
    $("#"+section).addClass('active');
}

function goHomeFrom(section){
    $("#"+section).removeClass('active');
    $("#"+section).addClass('hide');
    $("#home").removeClass('hide');
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
                var opt = document.createElement("option");
                opt.value = filename;
            });
        }
    });
    
    $("select").imagepicker();
}

$(document).ready(function(){
    //init canvas for drawing
    $.each(['#f00', '#ff0', '#0f0', '#0ff', '#00f', '#f0f', '#000', '#fff'], function() {
      $('#colors_demo .tools').append("<a href='#colors_sketch' data-color='" + this + "' style='width: 10px; background: " + this + ";'></a> ");
    });
    $.each([10, 15, 20], function() {
      $('#colors_demo .tools').append("<a href='#colors_sketch' data-size='" + this + "' style='background: #ccc'>" + this + "</a> ");
    });
    $('#colors_sketch').sketch();
    
    //init gallery
    initImagePicker();
});