#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "camera.h"
#include "a2d.h"

int main(){
    int reading = -1;
    A2D_initialize();
    int i = 0;
    while(true){
        reading = A2D_getValue(2);
        if(reading >= 3000){
            printf("Voice detected! Reading: %d\n", reading);
            i++;
        }
    }
    A2D_cleanup();
    return 0;
}
