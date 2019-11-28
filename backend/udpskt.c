//This module is modified based on Parker's AS3 solution
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "udpskt.h"
#include "camera.h"
#include "doorlock.h"
#include <pthread.h>
#include <stdbool.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include <time.h>
#define SOCKET_MAX_LEN 1500 //Maximum length for each message (packet). Measured using the size of one character (one byte)
#define SOCKET_LOCK_WAIT_SEC 5
#define SOCKET_LOCK_WAIT_MSC 0
#define SOCKET_NODE_PUB_FOLDER "doorlock_nodejs/public/"
#define SOCKET_PATH_BUF_SIZE 100

static pthread_t subThreadID; //the ID of the socket thread for thread control
static int bindingPort = 0; //the port to receive message
enum Socket_command {
    CapPic,
    Stop,
    AuthPos,
    AuthNeg,
    Other,
    Ping,
    Pause,
    Resume,
    Unlock,
    Lock
}; //enum used to decode and process command
static _Bool stopSignal = false; //stop signal used to signal the main thread that the program shall terminate
static _Bool threadRunning = false; //Signal to indicate whether the thread is running. Used for the send message functions. No message can be sent if the thread is not running
static _Bool pausedSignal = true; //Signal to pause facial recognition

//Function used to process command. It will take the string of command received and convert it to enum value for processing
static enum Socket_command Socket_processCmd(char* command);
//Sub thread function. It will open a connection on the port given, and for each loop, it will receive information from the client and give corresponding reply based on the command given.
static void* Socket_subThread();

static struct sockaddr_in sin_remote; //Pulled the socket-related variable as static so it can be used by other functions to connect with the client
static unsigned int sin_len;
static int socketDescriptor;

static enum Socket_command Socket_processCmd(char* command){
    if(strncmp(command, "capture picture", strlen("capture picture")) == 0){
        return CapPic;
    }
    else if(strncmp(command, "stop", strlen("stop")) == 0){
        return Stop;
    }
    else if(strncmp(command, "authenticated name ", strlen("authenticated name ")) == 0){
        return AuthPos;
    }
    else if(strncmp(command, "authenticated", strlen("authenticated")) == 0){
        return AuthPos;
    }
    else if(strncmp(command, "not authenticated", strlen("not authenticated")) == 0){
        return AuthNeg;
    }
    else if(strncmp(command, "pingdl", strlen("pingdl")) == 0){
        return Ping;
    }
    else if(strncmp(command, "pause", strlen("pause")) == 0){
        return Pause;
    }
    else if(strncmp(command, "resume", strlen("resume")) == 0){
        return Resume;
    }
    else if(strncmp(command, "unlock", strlen("unlock")) == 0){
        return Unlock;
    }
    else if(strncmp(command, "lock", strlen("lock")) == 0){
        return Lock;
    }
    else{
        return Other;
    }
}

_Bool Socket_getPausedSignal(){
    _Bool result = false;
    pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
    pthread_mutex_lock(&mutex);
    result = pausedSignal;
    pthread_mutex_unlock(&mutex);
    return result;
}

void Socket_startListening(int port){
    bindingPort = port;
    threadRunning = true;
    pthread_create(&subThreadID, NULL, &Socket_subThread, NULL);
}

void Socket_stopListening(void){
    pthread_join(subThreadID, NULL);
}

void Socket_sendPicFileName(char* fileName, int height, int width){
    if(!threadRunning){
        return;
    }
    else if(fileName == NULL){
        return;
    }
    char message_reply[SOCKET_MAX_LEN];
    sprintf(message_reply, "PicFile&%s&%d&%d", fileName, height, width);
    sendto( socketDescriptor,
    message_reply, strlen(message_reply),
    0,
    (struct sockaddr *) &sin_remote, sin_len);
}
void Socket_sendAuthRes(char* personName){
    if(!threadRunning){
        return;
    }
    char message_reply[SOCKET_MAX_LEN];
    if(personName == NULL){
        sprintf(message_reply, "Auth&%Invalid");
    }
    else{
        sprintf(message_reply, "Auth&%s", personName);  
    }
    sendto( socketDescriptor,
    message_reply, strlen(message_reply),
    0,
    (struct sockaddr *) &sin_remote, sin_len);
}

static void* Socket_subThread(){
    static struct sockaddr_in sin;
    sin_len = sizeof(sin_remote);
    memset(&sin, 0, sizeof(sin));
    memset(&sin_remote, 0, sizeof(sin_remote));
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = htonl(INADDR_ANY);
    sin.sin_port = htons(bindingPort);
    socketDescriptor = socket(PF_INET, SOCK_DGRAM, 0);
    bind(socketDescriptor, (struct sockaddr*)&sin, sizeof(sin));
    int recv_count = 0;
    char message_recv[SOCKET_MAX_LEN];
    char message_reply[SOCKET_MAX_LEN];
    enum Socket_command command = Other;
    struct timespec delay = {SOCKET_LOCK_WAIT_SEC, SOCKET_LOCK_WAIT_MSC};
    char* fileName;
    while(true){
        recv_count = recvfrom(socketDescriptor, message_recv, SOCKET_MAX_LEN, 0, (struct sockaddr*)&sin_remote, &sin_len);
        if(recv_count < SOCKET_MAX_LEN){
            message_recv[recv_count] = 0;
        }
        else{
            message_recv[SOCKET_MAX_LEN] = 0;
        }
        command = Socket_processCmd(message_recv);
        if(command == CapPic){
            fileName = Camera_getTimeStampedPic();
            if(access(fileName, F_OK) != -1){
                char destPath[SOCKET_PATH_BUF_SIZE];
                sprintf(destPath, SOCKET_NODE_PUB_FOLDER "%s", fileName);
                rename(fileName, destPath);
                Socket_sendPicFileName(fileName, 800, 600);
            }
            else{
                printf("File %s Not Found!\n", fileName);
            }
            free(fileName);
            
        }
        else if(command == AuthPos || command == Unlock){
            _Bool previousPauseSig;
            pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
            pthread_mutex_lock(&mutex);
            previousPauseSig = pausedSignal;
            pausedSignal = true;
            pthread_mutex_unlock(&mutex);
            printf("Door lock opening...\n");
            unlockDoor(); // To be uncommented after the lock becomes available
            //Socket_sendAuthRes(message_recv + strlen("authenticated name "));
            nanosleep(&delay, (struct timespec *) NULL);
            lockDoor();
            pthread_mutex_lock(&mutex);
            pausedSignal = previousPauseSig;
            pthread_mutex_unlock(&mutex);
        }
        //else if(command == AuthNeg){
            //Socket_sendAuthRes(NULL);
        //}
        else if(command == Pause){
            pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
            pthread_mutex_lock(&mutex);
            pausedSignal = true;
            pthread_mutex_unlock(&mutex);
        }
        else if(command == Resume){
            pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
            pthread_mutex_lock(&mutex);
            pausedSignal = false;
            pthread_mutex_unlock(&mutex);
        }
        else if(command == Lock){
            lockDoor();
        }
        else if(command == Stop){
            pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
            pthread_mutex_lock(&mutex);
            stopSignal = true;
            threadRunning = false;
            pthread_mutex_unlock(&mutex);
            break;
        }
        else if(command == Ping){
            sprintf(message_reply, "here");
            sendto( socketDescriptor,
			message_reply, strlen(message_reply),
			0,
			(struct sockaddr *) &sin_remote, sin_len);
        }
        else if(command == Other){
            printf("%s\n", message_recv);
            continue;
        }
    }
    close(socketDescriptor);
    return NULL;
}

_Bool Socket_getStopSignal(void){
    _Bool result = false;
    pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
    pthread_mutex_lock(&mutex);
    result = stopSignal;
    pthread_mutex_unlock(&mutex);
    return result;
}
