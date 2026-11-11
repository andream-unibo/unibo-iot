#ifndef PINS_H
#define PINS_H

#include <Arduino.h>

#define GREEN_LED_LEFT 4 
#define RED_LED_RIGHT 6 // PWM for pulsing phase
#define GREEN_LED_RIGHT 7 
#define RED_LED_LEFT 5 // PWM for pulsing phase

// Buttons on pins 2 and 3 are needed for int0 e int1 to wake up the system.

#define BUTTON_LEFT 2
#define BUTTON_RIGHT 3

#define PIN_POTENTIOMETER A0

#endif