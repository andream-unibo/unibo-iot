#include "gameEngine.h"
#include "input.h"
#include <Arduino.h>

#define MAX_WAIT 7500 + 1

#define PLAYER_1 1
#define PLAYER_2 2
#define LOSE_VALUE -1

#define RANDOM_MIN 1
#define RANDOM_MAX 3 + 1

struct Player {
    int points;
    bool lost;
};

Player players[2];

int currentRound = 0;
// milliseconds
unsigned long roundWaitTimeStart = 0;
unsigned long roundWaitTime = 0;
unsigned long playerWaitTimeStart = 0;
unsigned long playerTimeOut = 0;

int roundObjective = 0;

GAME_STATES currentState = GAME_STATES::INITIALIZING;

Player *getPlayer(int player) {
    return &players[player - 1];
}

void setupGameEngine(unsigned long seed) {
    randomSeed(seed);
}

void incrementPointsFor(int player) {
    getPlayer(player)->points++;
}

void decrementPointsFor(int player) {
    getPlayer(player)->points--;
}

void setPointsOf(int player, int points) {
    getPlayer(player)->points = points;
}

int getPointsOf(int player) {
    return getPlayer(player)->points;
}

void setPlayerLost(int player) {
    getPlayer(player)->lost = true;
}

bool hasPlayerLost(int player) {
    return getPlayer(player)->lost;
}

GAME_STATES getCurrentState() {
    return currentState;
}

void reset() {
    currentState = GAME_STATES::INITIALIZING;
    roundObjective = 0;
    setPointsOf(PLAYER_1, 0);
    setPointsOf(PLAYER_2, 0);
}

void startRoundWaiting() {
    currentState = GAME_STATES::WAITING_ROUND;
    roundWaitTimeStart = millis();
    roundWaitTime = random(0, MAX_WAIT);
    currentRound++;
}

void checkPlayerEarlyTap(int player, bool button) {
    if (button) {
        decrementPointsFor(player);
    }
}

void waitRound(bool player1Button, bool player2Button) {
    if (millis() - roundWaitTimeStart > roundWaitTime) {
        currentState = GAME_STATES::WAITING_PLAYER_REACTION;
        playerWaitTimeStart = millis();
    }
}

void listeningPlayerHits(int player, bool button) {
    if (button && !hasPlayerLost(player)) {
        getPlayer(player)->points++;
    }
    if (getPointsOf(player) > roundObjective) {
        setPlayerLost(player);
        decrementPointsFor(player);
        currentState = GAME_STATES::ROUND_OVER;
    }
}

void initState(bool player1Button, bool player2Button) {
    if (player1Button && player2Button) {
        startRoundWaiting();
    }
}

void roundState(bool player1Button, bool player2Button) {
    listeningPlayerHits(PLAYER_1, player1Button);
    listeningPlayerHits(PLAYER_2, player2Button);
    if (millis() - playerTimeOut > playerWaitTimeStart) {
        currentState = GAME_STATES::ROUND_OVER;
    }
}

void gameEngineUpdate() {
    bool player1Button = getButtonState(PLAYER_1);
    bool player2Button = getButtonState(PLAYER_2);

    switch (getCurrentState()) {
        case GAME_STATES::INITIALIZING:
            initState(player1Button, player2Button);
            break;
        case GAME_STATES::WAITING_ROUND:
            checkPlayerEarlyTap(PLAYER_1, player1Button);
            checkPlayerEarlyTap(PLAYER_2, player2Button);
            waitRound(player1Button, player2Button);
            break;
        case GAME_STATES::WAITING_PLAYER_REACTION:
            roundState(player1Button, player2Button);
            break;
    }
}