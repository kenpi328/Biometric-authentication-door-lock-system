OUTDIR = $(HOME)/cmpt433/public/myApps
CROSS_COMPILE = arm-linux-gnueabihf-
CC_C = $(CROSS_COMPILE)gcc
CFLAGS = -g -std=c99 -pthread -D _POSIX_C_SOURCE=200809L -lm

all: main.o camera.o doorlock.o a2d.o udpskt.o
	$(CC_C) camera.o main.o doorlock.o a2d.o udpskt.o $(CFLAGS) -o dl
	cp dl $(OUTDIR)/
tester.o:
	$(CC_C) $(CFLAGS) -c tester.c
udpskt.o:
	$(CC_C) $(CFLAGS) -c udpskt.c
doorlock.o:
	$(CC_C) $(CFLAGS) -c doorlock.c
main.o:
	$(CC_C) $(CFLAGS) -c main.c
camera.o:
	$(CC_C) $(CFLAGS) -c camera.c
a2d.o:
	$(CC_C) $(CFLAGS) -c a2d.c
clean:
	rm -f *.o dl
	rm -f $(OUTDIR)/dl
