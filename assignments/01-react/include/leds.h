#ifndef LEDS_H
#define LEDS_H

#define PIN_L1 4 
#define PIN_L2 5 
#define PIN_L3 6 
#define PIN_L4 7 

void ledsInit();

void ledsSetFading(bool active);

void ledsUpdate();

void turnGreenOn(int player);

void turnRedOn(int player);

#endif