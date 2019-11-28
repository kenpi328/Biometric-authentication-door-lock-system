/*
 * Respond to commands over a websocket to do math
 */

var socketio = require('socket.io');
var io;

var dgram = require('dgram');
var fs = require('fs')
var picFile = ''

exports.listen = function(server) {
	io = socketio.listen(server);
	io.set('log level 1');
	
	io.sockets.on('connection', function(socket) {

		handleCommand(socket);
	});
	
	
};

function handleCommand(socket) {

	socket.on('cv', function(data) {
		console.log('opencv command: ' + data);
		var PORT = 12346;
		var HOST = '192.168.7.1';
		var buffer = new Buffer(data);

		var client = dgram.createSocket('udp4');
		client.send(buffer, 0, buffer.length, PORT, HOST, function(err, bytes){
			console.log('Message sent to UDP socket host: ' + HOST + 'port: ' + PORT);
		});

		client.on('listening', function(){
			var address = client.address();
		});

		client.on('message', function(message, remote){
			console.log("UDP Client: message Rx" + remote.address + ':' + remote.port +' - ' + message);
			responded = true
			var command = message.toString('utf8')
			var decodedCommand = command.split('&') //All message that updates a field will be written in the format of "field:value" and therefore it's necessary to split the two parts
			if(command.search("&") != -1){ //if the received message is an update command (with a ':'), then process the command and send to the front end
				if(decodedCommand[0] == 'CVErr'){ //If it's a message that updates mode, change the sign of mode to the name of mode
					socket.emit(decodedCommand[0], decodedCommand[1])
				}
				else if(decodedCommand[0] == 'CVAuthPos'){
					console.log('Removing file' + picFile);
					fs.unlink(picFile, (err) => {
						if (err) {
							console.error(err)
							return
						}
					});
					picFile = ''
					socket.emit('AuthPos', decodedCommand[1])
				}
			}
			else if(command == 'CVAuthNeg'){
				console.log('Removing file' + picFile);
				fs.unlink(picFile, (err) => {
					if (err) {
						console.error(err)
						return
					}
				});
				picFile = ''
				socket.emit('AuthNeg')
			}
			else if(command == 'here'){
				socket.emit('cvhere')
			}
		});
	});

	// Pased string of comamnd to relay
	socket.on('dl', function(data) {
		console.log('doorlock command: ' + data);
		var responded = false;
		var PORT = 12345;
		var HOST = '127.0.0.1';
		var buffer = new Buffer(data);
		var client = dgram.createSocket('udp4');
		// Info for connecting to the local process via UDP
		if(data == 'pingws'){
			socket.emit('wshere')
		}
		else{ //if not, relay the message to the program
			client.send(buffer, 0, buffer.length, PORT, HOST, function(err, bytes){
				responded = false;
				console.log('Message sent to UDP socket host: ' + HOST + 'port: ' + PORT);
				/*setTimeout(function(){ //if not responded (see code below), wait for half second, and then send the error message
					if(!responded){
						socket.emit('systemErr', 'SERVER ERROR: No response from the application. Is it running?')
					}
				}, 1500);*/
			});
		}

		client.on('listening', function(){
			var address = client.address();
		});
	
		client.on('message', function(message, remote){
			console.log("UDP Client: message Rx" + remote.address + ':' + remote.port +' - ' + message);
			responded = true
			var command = message.toString('utf8')
			var decodedCommand = command.split('&') //All message that updates a field will be written in the format of "field:value" and therefore it's necessary to split the two parts
			if(command.search("&") != -1){ //if the received message is an update command (with a ':'), then process the command and send to the front end
				if(decodedCommand[0] == 'PicFile'){
					picFile = './public/' + decodedCommand[1];
					socket.emit(decodedCommand[0], decodedCommand[1], decodedCommand[2], decodedCommand[3])
				}
				else if(decodedCommand[0] == 'Auth'){
					console.log('Removing file' + picFile);
					fs.unlink(picFile, (err) => {
						if (err) {
							console.error(err)
							return
						}
					});
					if(decodedCommand[1] == '%Invalid'){
						socket.emit('AuthNeg')
					}
					else{
						socket.emit('AuthPos', decodedCommand[1])
					}
				}
			}
			else if(command == 'here'){
				socket.emit('here')
			}
		});

	});

};