#include<stdbool.h>
#include "camera.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define CAMERA_PATH "/dev/video0"
#define CAMERA_CMD_LEN 150
#define CAMERA_MAX_VERT 960
#define CAMERA_MAX_HORI 720

static _Bool Camera_fileExist(char* path){
    FILE* file = NULL;
    file = fopen(path, "r");
    if(file == NULL){
        return false;
    }
    fclose(file);
    return true;
}

_Bool Camera_detect(){
    return Camera_fileExist(CAMERA_PATH);
}

void Camera_read(int resHor, int resVert, char* dstPath){
    char command[CAMERA_CMD_LEN];
    sprintf(command, "LD_PRELOAD=/usr/lib/arm-linux-gnueabihf/libv4l/v4l2convert.so fswebcam -r ");
    sprintf(command+strlen(command), "%dx%d", resHor, resVert);
    sprintf(command+strlen(command), " --jpeg 85 ");
    sprintf(command+strlen(command), "%s", dstPath);
    system(command);
}

char* Camera_getTimeStampedPic(){
    if(!Camera_detect()){
        return NULL;
    }
    //time code from https://stackoverflow.com/questions/5141960/get-the-current-time-in-c
    time_t rawtime;
    struct tm * timeinfo;

    time ( &rawtime );
    timeinfo = localtime ( &rawtime );
    char* fileName = malloc(CAMERA_CMD_LEN * sizeof(*fileName));
    sprintf(fileName, "doorLockCap-%d%d%d-%d:%d:%d.jpg", (timeinfo -> tm_year) + 1900, (timeinfo -> tm_mon) + 1, timeinfo -> tm_mday, timeinfo->tm_hour, timeinfo->tm_min, timeinfo -> tm_sec);
    Camera_read(800, 600, fileName);
    return fileName;
}

void Camera_initialize(){
    return;
}

void Camera_cleanup(){
    return;
}
