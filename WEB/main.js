function readURL(input) {
    if (input.files && input.files[0]) {
        var reader = new FileReader();

        reader.onload = function (e) {
            $('#preview')
                .attr('src', e.target.result)
                .width(47)
                .height(47);
        };

        reader.readAsDataURL(input.files[0]);
    }
}


$(function () {
    var box = $('#box')[0];
    $('#colored_sketch').sketch();
    $("#slider-horiz").slider({
        orientation: "horizontal",
        min: 0,
        max: 360,
        value: 0,
        slide: function (event, ui) {
            //console.log('/'+hsl(ui.value, 100%, 50%));
            //$('#box').attr('data-color', 'hsl(' + ui.value + ', 100%, 50%)');
            box.style.background = 'hsl(' + ui.value + ', 100%, 50%)';
            var clr = $('#box').css('background-color');
            $('#box').attr('data-color', clr).trigger('click');
        }
    });
});


//source of the below function : http://www.jquery4u.com/jquery-functions/jquery-convert-rgb-hex-color/

function rgb2hex(rgb) { rgb = rgb.match(/^rgb\((\d+),\s*(\d+),\s*(\d+)\)$/); 
    return "#" +   ("0" + parseInt(rgb[1], 10).toString(16)).slice(-2) +   ("0" + parseInt(rgb[2], 10).toString(16)).slice(-2) +   ("0" + parseInt(rgb[3], 10).toString(16)).slice(-2);
}