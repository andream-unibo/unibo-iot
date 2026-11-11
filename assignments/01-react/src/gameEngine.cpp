#include "../include/gameEngine.h"
#include <Arduino.h>

#define RANDOM_MIN 1
#define RANDOM_MAX 3

void setupGameEngine(unsigned long seed) {
    randomSeed(seed);
}

int randomNumber() {
    
}