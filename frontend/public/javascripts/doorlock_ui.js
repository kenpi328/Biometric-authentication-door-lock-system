"use strict";
// Client-side interactions with the browser.

// Web sockets: automatically establish a socket with the server
var socket = io.connect();
var picURL = "";
var picInAuth = false;
// Make connection to server when web page is fully loaded.
$(document).ready(function() {
	window.setInterval(function(){
		sendDLCommand("pingdl")
	}, 1000);
	// Make the text-entry box have keyboard focus.
	$('#send-command').focus();
	
	// Allow sending the form (pressing enter).
	$('#capture-image').click(function() {
		sendDLCommand("capture picture")
	});

	$('#pause').click(function() {
		sendDLCommand("pause")
	});

	$('#resume').click(function() {
		sendDLCommand("resume")
	});

	$('#unlock').click(function() {
		sendDLCommand("unlock")
		picInAuth = true
		window.setTimeout(function(){
			picInAuth = false
		}, 5000);
	});

	// Handle data coming back from the server
	socket.on('PicFile', function(fileName, height, width) {
		if(!picInAuth){
			$('#image').attr("src", '/'+fileName)
			$('#image').attr("height", width)
			$('#image').attr("width", height)
			$('#status').html('Photo detected! Sending to authentication server...')
			picURL = window.location.origin + '/' + fileName;
			console.log(picURL)
			sendCVCommand(picURL)
			picInAuth = true;
		}
	});
	
	
	socket.on('AuthPos', function(name) {
		$('#status').html('Validated. Welcome back, ' + name)
		sendDLCommand("authenticated")
		window.setTimeout(function(){
			picInAuth = false
		}, 5000);
	});

	socket.on('AuthNeg', function(){
		picInAuth = false;
		$('#status').html('Invalid entry!')
		sendDLCommand('lock')
	});

	socket.on('systemErr', function(value){
		$('#status').html('Error detected: ' + value)
	});
	
});

function sendDLCommand(message) {
	socket.emit('dl', message);
};

function sendCVCommand(message) {
	socket.emit('cv', message);
};
