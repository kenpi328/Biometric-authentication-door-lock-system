// udpskt.h
// Module to spawn a separate thread to listen to UDP connection on port given, and provide reply to command received
//This module is modified based on Parker's AS3 solution

#include <stdbool.h>
#ifndef UDPSKT_H
#define UDPSKT_H
// Begin/end the background thread which listens to command on UDP port "port"
void Socket_startListening(int port);
void Socket_stopListening(void);
// Get control signal for main thread to handle stop command. It will return true when stop command is received and let main thread to proceed with termination
_Bool Socket_getStopSignal(void);
//Additional functions to directly send the tempo, mode and volume. Only works when the socket thread is on. Used for updating the Node.js client when those values are changed through joystick
void Socket_sendPicFileName(char*, int, int);
void Socket_sendAuthRes(char*);
_Bool Socket_getPausedSignal(void);
#endif