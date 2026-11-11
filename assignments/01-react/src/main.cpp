#include <Arduino.h>
#include "../include/gameEngine.h"
#include "../include/leds.h"
#include "../include/screen.h"
#include "../include/input.h"

void setup() {
  setupLeds();
  setupInput();
  setupScreen();
  setupGameEngine(analogRead(0));
}

void loop() {

}