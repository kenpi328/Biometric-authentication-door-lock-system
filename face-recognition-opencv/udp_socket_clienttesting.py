import socket   # UDP socket
import urllib.request	# module to download image file
import sys

UDP_IP = "127.0.0.1"
UDP_PORT = 12346
BUFFER_SIZE = 1500

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
message = b'https://i.imgur.com/Uk6Z95z.jpg'
server_address = (UDP_IP,UDP_PORT)
try: 
	print('sending message')
	sent = sock.sendto(message, server_address)

	data, server = sock.recvfrom(4096)
	print('received {!r}'.format(data))

finally:
	print('closing socket')
	sock.close()