# Biometric-authentication-door-lock-system

Short demo movie: https://www.youtube.com/watch?v=egyDvM5JLgA

![Screenshot from 2019-11-27 17-57-00](https://user-images.githubusercontent.com/50088458/69770860-757d9e00-113f-11ea-9158-cacbc7d36bf3.png )
![withTerminal](https://user-images.githubusercontent.com/50088458/69770893-90e8a900-113f-11ea-9191-c38f84607a81.png)

"Biometric Authentication Door Lock System" will unlock a locked
door lock by visually recognizing the face of our team members. The system
will take a picture after a certain audio threshold (loudness) is reached and
will compare this picture with the face recognition model in the host using
OpenCV and DLIB, if this image is accurate enough to compare to the facial
model in its system, the system will send a signal to unlock the connected
door lock.
The visual system we will use to compare facial imagery is the OpenCV and
dlib library. Required additional external components will include a webcam
used to take a picture and recognize visual imagery, a microphone to
recognize audio inputs and a connected door lock that will unlock if the
Beaglebone signal is sent. Each of these devices will include the supported
linux drivers either online or included in their hardware.
There are 3 software components of the project: The backend program, the
Node.js front end module, and the OpenCV recognition engine. Their
relationship can be illustrated by the following graph:

![Screenshot from 2019-11-27 17-56-41](https://user-images.githubusercontent.com/50088458/69770906-9c3bd480-113f-11ea-83a7-1d257623e94c.png)



The backend program is responsible for controlling the door lock as well as
capturing the picture. It will keep monitoring the microphone through A2D and
once it reaches the threshold, it will activate the camera and take the picture.
Then, it will copy the picture into the Node.js folder and send the filename to
the Node.js frontend by the UDP socket.The Node.js frontend program is mainly in charge of interacting with the user
and display the output from the backend program. Upon receiving the photo, it
will display the picture and generate the URL of the picture, and finally send it
to the OpenCV facial recognition engine through UDP socket so it can be
downloaded. It also has 4 additional buttons that send commands to the
backend program. Those buttons include buttons to pause and resume
monitoring the microphone, the button to manually capture an image
regardless of the microphone status, and the button to manually unlock the
door in the cases of necessary.
Upon receiving the URL of the picture, the OpenCV module (running on the
desktop) will download the picture using HTTP and save it locally. Then, it will
use OpenCV to detect whether a fact that is pre-encoded in it is in that picture
and send the result back along with the name of the person it recognizes.
Once the facial recognition result is received by the Node.js, the Node.js
frontend program is going to clean up the temporary file used as it’s no longer
needed. Then, it will display the result and send it back to the backend
program. If the result is positive (or the manual unlock button is clicked), the
backend program will open the door lock by GPIO pin, and wait for 5 second
before locking it again. While the door lock is unlocked, the camera will stop
capturing any other picture and the Node.js will not accept any picture input.
The entire recognition process will take about 10 seconds. However, most of
the time will be spent on the photo capture and recognition, with capture takes
about 5 seconds and recognition takes about 5 seconds. The network
transmission is almost instant given all the components of this project is within
the same network, if not the same device.
