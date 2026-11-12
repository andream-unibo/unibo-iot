#ifndef LEDS_H
#define LEDS_H

void setupLeds();

void ledsSetFading(bool active);

void ledsUpdate();

void turnGreenOn(int player);

void turnGreenOff(int player);

void turnRedOn(int player);

void turnRedOff(int player);

#endif