import socket   # UDP socket
import urllib.request	# module to download image file
import sys

UDP_IP = ""
UDP_PORT = 12346
BUFFER_SIZE = 1500
server_address = (UDP_IP, UDP_PORT)

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.bind(server_address)

## Download file by its URL
## test url: https://i.imgur.com/Uk6Z95z.jpg
def downloadImage(imageURL):
	print("Downloading image file")
	url = imageURL # url on image file
	urllib.request.urlretrieve(url, "face.jpg") 

# TODO: Error Message (404 URL error, openCV linking)

# format: CVAuthPos&Name, CVAuthNeg
# CVErr&Msg
def sendMessage(result, name, address):
	if result == True:
		message = b'CVAuthPos&' + b'name'
	elif result == False:
		message = b'CVAuthNeg'
	else:
		message = b'CVErr' 
	sock.sendto(message, address)
	print('sent result: {} back to {}'.format(result, address))

############ DELETE LATER ############
# TEST FUNCTION TO STIMULATE OPENCV - DELETE LATER
import random
def openCV():
	return random.choice([True, False])
######################################

while True:
	print('\nwaiting to receive message')
	data, address = sock.recvfrom(4096)

	print('received {} bytes from {}'.format(len(data), address))
	decodeURL = data.decode("utf-8")
	print(decodeURL)
	if data:
		downloadImage(decodeURL)

		# OPEN CV HERE 
		result = openCV()

		# openCV should return (Pos or Neg + Name) or (Err + Error message)
		# TODO: reconstruct sendMessage function
		name = 'jj'
		sendMessage(result, name, address)

		

