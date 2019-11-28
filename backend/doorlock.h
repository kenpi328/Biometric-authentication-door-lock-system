#ifndef DOORLOCK_H_
#define DOORLOCK_H_
#define _BSD_SOURCE

#define EXPORT "/sys/class/gpio/export"
#define DOORLOCK_VAR "/sys/class/gpio/gpio71/value"
#define DOORLOCK_DIR "/sys/class/gpio/gpio71/direction"

void naSleep();
void initDoorlock();
void lockDoor();
void unlockDoor();


#endif
