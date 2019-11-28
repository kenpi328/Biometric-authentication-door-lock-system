# USAGE
# python recognize_faces_image.py --encodings encodings.pickle --image examples/example_01.png

# import the necessary packages
import face_recognition
import argparse
import pickle
import cv2
import os
import time
import socket   # UDP socket
import urllib.request	# module to download image file
import sys
import socket   # UDP socket
import urllib.request	# module to download image file
import sys
#size = 800, 800
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
	try:
		urllib.request.urlretrieve(url, "face.jpg")
	except urllib.error.URLError as e:
		print(e.reason)

# TODO: Error Message (404 URL error, openCV linking)

# format: CVAuthPos&Name, CVAuthNeg
# CVErr&Msg
def sendMessage(result, name, address):
	if result == True:
		message = str.encode("CVAuthPos&" + name)
	elif result == False:
		message = b'CVAuthNeg'
	else:
		message = b'CVErr'
	sock.sendto(message, address)
	print('sent result: {} back to {}'.format(result, address))
##JJCODE HERE######################

# construct the argument parser and parse the arguments
ap = argparse.ArgumentParser()
ap.add_argument("-e", "--encodings", required=False,
	help="path to serialized db of facial encodings")
ap.add_argument("-i", "--image", required=False,
	help="path to input image")
ap.add_argument("-d", "--detection-method", type=str, default="cnn",
	help="face detection model to use: either `hog` or `cnn`")
args = vars(ap.parse_args())

args["encodings"] = "encodingsKPJ.pickle"
args["image"] = "face.jpg"
# load the known faces and embeddings
print("[INFO] loading encodings...")


# load the input image and convert it from BGR to RGB

while(1):
	names = []
	print('\nwaiting to receive message')


	imgdata, address = sock.recvfrom(4096)

	print('received {} bytes from {}'.format(len(imgdata), address))
	decodeURL = imgdata.decode("utf-8")
	print(decodeURL)
	if imgdata:
		downloadImage(decodeURL)


	data = pickle.loads(open(args["encodings"], "rb").read())

	image = cv2.imread(args["image"])
	#cv2.imshow("Image", image)
	rgb = cv2.cvtColor(image, cv2.COLOR_BGR2RGB)

	# detect the (x, y)-coordinates of the bounding boxes corresponding
	# to each face in the input image, then compute the facial embeddings
	# for each face
	print("[INFO] recognizing faces...")
	boxes = face_recognition.face_locations(rgb,
		model=args["detection_method"])
	encodings = face_recognition.face_encodings(rgb, boxes)

	# loop over the facial embeddings
	for encoding in encodings:
		# initialize the list of names for each face detected
		# attempt to match each face in the input image to our known
		# encodings
		matches = face_recognition.compare_faces(data["encodings"], encoding, tolerance = 0.35)
		name = "Unknown"

		# check to see if we have found a match
		if True in matches:
			# find the indexes of all matched faces then initialize a
			# dictionary to count the total number of times each face
			# was matched
			matchedIdxs = [i for (i, b) in enumerate(matches) if b]
			counts = {}

			# loop over the matched indexes and maintain a count for
			# each recognized face face
			for i in matchedIdxs:
				name = data["names"][i]
				counts[name] = counts.get(name, 0) + 1

			# determine the recognized face with the largest number of
			# votes (note: in the event of an unlikely tie Python will
			# select first entry in the dictionary)
			name = max(counts, key=counts.get)

		# update the list of names
		names.append(name)

	# loop over the recognized faces
	for ((top, right, bottom, left), name) in zip(boxes, names):
		# draw the predicted face name on the image
		cv2.rectangle(image, (left, top), (right, bottom), (0, 255, 0), 2)
		y = top - 15 if top - 15 > 15 else top + 15
		cv2.putText(image, name, (left, y), cv2.FONT_HERSHEY_SIMPLEX,
			0.75, (0, 255, 0), 2)

	# show the output image
	data = 'Unknown'
	result = False
	if 'ken' in names:
		data = 'Ken'
		result = True
		#print("Found Ken!")
	elif 'parker' in names:
		data = 'Parker'
		result = True
	elif 'jj' in names:
		data = 'JJ'
		result = True

		#print("Found Parker!")
	print("Found " + data + "!")


	#if imgdata:

		# OPEN CV HERE
		#result = openCV()

		# openCV should return (Pos or Neg + Name) or (Err + Error message)
		# TODO: reconstruct sendMessage function
		#name = 'jj'
	sendMessage(result, data, address)

	#cv2.imshow("Image", image)
	#time.sleep(30)
	#cv2.waitKey(0)
	if os.path.exists('face.jpg'):
		os.remove('face.jpg')
