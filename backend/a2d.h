//a2d.h
//Module to retrieve the readings from the A2D device
#ifndef A2D_H
#define A2D_H
//Initializer function. Currently do nothing
void A2D_initialize(void);
//Get the readings from the A2D device and convert it into integer
int A2D_getValue(int);
//Closer function. Currently do nothing
void A2D_cleanup(void);
#endif