#ifndef CAMERA_H
#define CAMERA_H
#include <stdbool.h>
void Camera_initialize();
_Bool Camera_detect();
void Camera_read(int resHor, int resVert, char* dstPath);
void Camera_cleanup();
char* Camera_getTimeStampedPic();
#endif