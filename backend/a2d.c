#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "a2d.h"
#define A2D_FILE_PATH_LEN 50
#define A2D_FILE_DIRECTORY "/sys/bus/iio/devices/iio:device0/"
#define A2D_FILE_MIN 0
#define A2D_FILE_MAX 6
void A2D_initialize(void){
    return;
}

int A2D_getValue(int i2dInd){
    int result = -1;
    if(i2dInd < A2D_FILE_MIN || i2dInd > A2D_FILE_MAX){
        return result;
    }
    char path[A2D_FILE_PATH_LEN];
    sprintf(path, A2D_FILE_DIRECTORY);
    sprintf(path+strlen(path), "in_voltage%d_raw", i2dInd);
    FILE* file = fopen(path, "r");
    if(file == NULL){
        return result;
    }
    int itemsRead = fscanf(file, "%d", &result);
    if (itemsRead <= 0) {
        printf("ERROR: Unable to read values from voltage input file.\n");
        result = -1;
        return result;
    }
    // Close file
    fclose(file);
    return result;
}

void A2D_cleanup(void){
    return;
}