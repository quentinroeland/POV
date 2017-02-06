var express = require('express');
var server = express();

app.use(express.static('public'));

server.get('/index', function(request, response) {
	response.sendFile(__dirname + '/index.html');
});

server.get('/pixels', function(request, response) {
	response.sendFile(__dirname + '/pixels.html');
});

server.get('/simulator', function(request, response) {
	response.sendFile(__dirname + '/simulator.html');
});
server.listen(8080);