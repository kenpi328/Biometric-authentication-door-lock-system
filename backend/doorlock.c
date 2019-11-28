#include "doorlock.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

void naSleep(){
	struct timespec ts;
	ts.tv_sec = 0;
	ts.tv_nsec = 100000000;
	nanosleep(&ts, NULL);
}


static int writeToFile(char* fileName, char* var) //export the files
{
	//Use fopen() to open the file for write access.
	FILE *pfile = fopen(fileName, "w");
	if (pfile == NULL){
		return 1;
	}
	//Write tot data to the file using fprintf():
	fprintf(pfile, "%s", var);
	//Close the file using fclose():
	fclose(pfile);
	return 0;
}
void initDoorlock()
{
    writeToFile(EXPORT, "71");
	naSleep();
	writeToFile(DOORLOCK_DIR,"out");
}
void lockDoor()
{
	writeToFile(DOORLOCK_VAR,"1");
}
void unlockDoor()
{
	writeToFile(DOORLOCK_VAR,"0");
}