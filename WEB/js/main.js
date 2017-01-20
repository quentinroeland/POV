function readURL(input) {
    if (input.files && input.files[0]) {
        var reader = new FileReader();

        reader.onload = function (e) {
            $('#preview')
                .attr('src', e.target.result)
                .width(90)
                .height(90);
            var img = document.getElementById('preview');
            var canvas = document.createElement('canvas');
            canvas.width = img.width;
            canvas.height = img.height;
            canvas.getContext('2d').drawImage(img, 0, 0, img.width, img.height);
            var toSend = getDataToSend(canvas);
            console.log(toSend);
            postData(toSend);
            
        };

        reader.readAsDataURL(input.files[0]);
    }
}


function nav(section){
    $("#home").addClass('hide');
    $("#"+section).removeClass('hide');
    $("#"+section).addClass('active');
    initCarousel();
}

function goHomeFrom(section){
    $("#"+section).removeClass('active');
    $("#"+section).addClass('hide');
    $("#home").removeClass('hide');
}

//source of the below function : http://www.jquery4u.com/jquery-functions/jquery-convert-rgb-hex-color/
function rgb2hex(rgb) { rgb = rgb.match(/^rgb\((\d+),\s*(\d+),\s*(\d+)\)$/); 
    return "#" +   ("0" + parseInt(rgb[1], 10).toString(16)).slice(-2) +   ("0" + parseInt(rgb[2], 10).toString(16)).slice(-2) +   ("0" + parseInt(rgb[3], 10).toString(16)).slice(-2);
}


function getDataToSend(canvas){
    var pixelsData = [];
    for (var i = 0; i < 47; i++) {
        var row = [];
        for (var j = 0; j < 47; j++) {
            var pixel = canvas.getContext('2d').getImageData(i, j, 1, 1).data;
            var r = pixel[0];
            var g = pixel[1];
            var b = pixel[2];
            var tmp = [r,g,b]
            row.push(tmp);
        }
        pixelsData.push(row);
    }
    
    var colorsTab = [];
    for(var r = 0 ; r < 23 ;r++){
        var row = [];
        for(var d = 0 ; d < 120 ; d++){
            var teta = d *3;
            x = Number(r*Math.cos(teta) + 23).toFixed(0);
            y = Number(r*Math.sin(teta) + 23).toFixed(0);
            row.push(pixelsData[x][y]) 
        }
        colorsTab.push(row);
    }
    var toReturn = {};
    toReturn.body = {};
    toReturn.body.i = 23;
    toReturn.body.j = 120;
    toReturn.body.image = colorsTab;
    return toReturn;
}


function postData(data){    
var url = "192.168.1.34/image/";
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

function initCarousel(){
    $('.carousel').carousel();
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
    initCarousel();
});