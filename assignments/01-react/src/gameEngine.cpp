#include "gameEngine.h"
#include "leds.h"
#include "screen.h"
#include "input.h"
#include <Arduino.h>

#define RANDOM_MIN 1
#define RANDOM_MAX 3 + 1

int playerPoints[] = {0, 0};
int currentRound = 0;

GAME_STATES currentState = GAME_STATES::INIT;

void setupGameEngine(unsigned long seed) {
    randomSeed(seed);
}   

int randomNumber() {
    return random(RANDOM_MIN, RANDOM_MAX);
}

void addPoint(int player) {
    playerPoints[player - 1] += 1;
}   

void removePoint(int player) {
    playerPoints[player - 1] -= 1;
}

GAME_STATES getCurrentState() {
    return currentState;
}

void startRound() {
    currentState = GAME_STATES::ROUND;
    currentRound++;
}

void reset() {
    currentState = GAME_STATES::INIT;
    playerPoints[0] = 0;
    playerPoints[1] = 0;
}