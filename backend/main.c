#include "a2d.h"
#include "camera.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "udpskt.h"
#include "doorlock.h"
#include <unistd.h>
#define MIC_A2D 6
#define TIMEOUT_SEC 5
#define TIMEOUT_MSC 0
#define LISTENING_PORT 12345
#define MIC_THRESHOLD 4050
#define PATH_BUF_SIZE 100
#define NODE_PUB_FOLDER "doorlock_nodejs/public/"

int main(){
    int reading = -1;
    struct timespec delay = {TIMEOUT_SEC, TIMEOUT_MSC};
    char* fileName = NULL;
    char destPath[PATH_BUF_SIZE];
    _Bool paused = true;
    initDoorlock();
    lockDoor();
    A2D_initialize();
    Camera_initialize();
    Socket_startListening(LISTENING_PORT);
    while(!Socket_getStopSignal()){
        paused = Socket_getPausedSignal();
        while(paused){
            paused = Socket_getPausedSignal();
        }
        if(!Camera_detect()){
            printf("No camera connected! Exiting...\n");
            return EXIT_FAILURE;
        }
        reading = A2D_getValue(MIC_A2D);
        if(reading >= MIC_THRESHOLD){
            printf("Voice detected! Reading: %d. Capturing picture now...\n", reading);
            fileName = Camera_getTimeStampedPic();
            if(access(fileName, F_OK) != -1){
                sprintf(destPath, NODE_PUB_FOLDER "%s", fileName);
                rename(fileName, destPath);
                Socket_sendPicFileName(fileName, 800, 600);
            }
            else{
                printf("File %s Not Found!\n", fileName);
            }
            free(fileName);
            nanosleep(&delay, (struct timespec *) NULL);
        }
    }
    Socket_stopListening();
    Camera_cleanup();
    A2D_cleanup();
    return 0;
}
