#include "gameEngine.h"
#include "input.h"
#include "screen.h"
#include "leds.h"
#include <Arduino.h>

#define PLAYER_1 0
#define PLAYER_2 1



#define LOSE_VALUE -1

#define TARGET_MIN 1
#define TARGET_MAX 3 + 1

#define WIN_CHECK_WAIT 250

#define WIN_TIMER 2000
#define GAME_OVER_TIMER 10000
#define START_TRANSITION_TIME 300
#define GAME_OVER_RED_LED_TIME 2000
#define INITIAL_T1 10000
#define MIN_TIME 2000

// Requested parameters
#define SLEEP_TIME 10000
#define DIFFICULTY_FACTOR 150 // ms 
#define MAX_WAIT 7500 + 1

struct Player {
    int points;
    int roundClicks;
    unsigned long winTimeStart;
};

Player players[2];

// milliseconds
unsigned long stateTimer;
unsigned long stateTimerStart;

unsigned long playerTimeOut = INITIAL_T1;

int roundTarget = 0;
int difficulty = 1;

GAME_STATES currentState;

void setupGameEngine(unsigned long seed) {
    randomSeed(seed);
    changeState(GAME_STATES::WELCOME);
}

void playerLoss(int player) {
    players[player].points--;
    turnRedOn(player);
    changeState(GAME_STATES::ROUND_OVER);
}

void playerWin(int player) {
    players[player].points++;
    turnGreenOn(player);
    changeState(GAME_STATES::ROUND_OVER);
}

void checkPlayerEarlyTap(int player, bool button) {
    if (button) {
        playerLoss(player);
    }
}

void listeningPlayerHits(int player, bool button) {
    if (button) {
        players[player].roundClicks++;
    }
    if (players[player].roundClicks > roundTarget) {
        playerLoss(player);
    }
    else if (players[player].roundClicks == roundTarget) {
        players[player].winTimeStart = millis();
    }
}

void checkPlayerWin(int player) {
    if (players[player].winTimeStart == 0) {
        return;
    }
    bool winTimeCheck = millis() - players[player].winTimeStart > WIN_CHECK_WAIT;
    bool pointsCheck = players[player].roundClicks == roundTarget;
    if (winTimeCheck && pointsCheck) {
        playerWin(player);
    }
}

void playerResetRound(int player) {
    players[player].roundClicks = 0;
    players[player].winTimeStart = 0;
}

unsigned long calcPlayerTimeout() {
    unsigned long delta = DIFFICULTY_FACTOR * difficulty;
    if (playerTimeOut > MIN_TIME + delta) {
        return playerTimeOut - delta;
    }
    return MIN_TIME;
}

void changeState(GAME_STATES newState) {
    switch (newState) {
        case GAME_STATES::WELCOME: 
            screenSetText("Welcome to the React! Game. Press Buttons to Start");
            screenSetScrolling(true);
            ledsSetFading(true);
            turnRedOn(PLAYER_1);
            turnRedOn(PLAYER_2);
            turnGreenOff(PLAYER_1);
            turnGreenOff(PLAYER_2);
            stateTimer = SLEEP_TIME;
            break;
        case GAME_STATES::GAME_START:
            screenSetText("Go!");
            screenSetScrolling(false);
            ledsSetFading(false);
            turnRedOff(PLAYER_1);
            turnRedOff(PLAYER_2);
            stateTimer = START_TRANSITION_TIME;
            players[PLAYER_1].points = 0;
            players[PLAYER_2].points = 0;
            difficulty = getDifficulty();
            playerTimeOut = INITIAL_T1;
            break;
        case GAME_STATES::WAITING_ROUND:
            screenSetText("");
            turnGreenOff(PLAYER_1);
            turnGreenOff(PLAYER_2);
            turnRedOff(PLAYER_1);
            turnRedOff(PLAYER_2);
            playerResetRound(PLAYER_1);
            playerResetRound(PLAYER_2);
            stateTimer = random(0, MAX_WAIT);
            stateTimerStart = millis();
            roundTarget = random(TARGET_MIN, TARGET_MAX);
            playerTimeOut = calcPlayerTimeout();
            break;
        case GAME_STATES::WAITING_PLAYER_REACTION:
            screenShowTarget(roundTarget);
            stateTimer = playerTimeOut;
            break;
        case GAME_STATES::ROUND_OVER:
            screenShowPlayerScores(players[PLAYER_1].points, players[PLAYER_2].points);
            stateTimer = WIN_TIMER;
            break;
        case GAME_STATES::GAME_OVER_RED_LEDS:
            turnRedOn(PLAYER_1);
            turnRedOn(PLAYER_2);
            if (players[PLAYER_1].points > players[PLAYER_2].points) {
                screenSetText("Game Over - The winner is Player 1");
            } else if (players[PLAYER_1].points < players[PLAYER_2].points) {
                screenSetText("Game Over - The winner is Player 2");
            } else {
                screenSetText("Game Over - Tie");
            }
            screenSetScrolling(true);
            stateTimer = GAME_OVER_RED_LED_TIME;
            break;
        case GAME_STATES::GAME_OVER:
            turnRedOff(PLAYER_1);
            turnRedOff(PLAYER_2);
            stateTimer = GAME_OVER_TIMER - GAME_OVER_RED_LED_TIME;
            break;
        default:
            break;
    }
    currentState = newState;
    stateTimerStart = millis();
}

void changeStateAfterStateTime(GAME_STATES state) {
    if (millis() - stateTimerStart > stateTimer) {
        changeState(state);
    }
}

void gameEngineUpdate() {
    bool player1Button = getButtonState(PLAYER_1);
    bool player2Button = getButtonState(PLAYER_2);

    switch (currentState) {
        case GAME_STATES::WELCOME:
            if (player1Button && player2Button) {
                changeState(GAME_STATES::GAME_START);
            } else if (player1Button || player2Button) {
                stateTimerStart = millis();
            } else if (millis() - stateTimerStart > SLEEP_TIME) {
                gameSleep();
                changeState(GAME_STATES::WELCOME);
            }
            break;
        case GAME_STATES::GAME_START:
            changeStateAfterStateTime(GAME_STATES::WAITING_ROUND);
            break;
        case GAME_STATES::WAITING_ROUND:
            checkPlayerEarlyTap(PLAYER_1, player1Button);
            checkPlayerEarlyTap(PLAYER_2, player2Button);
            changeStateAfterStateTime(GAME_STATES::WAITING_PLAYER_REACTION);
            break;
        case GAME_STATES::WAITING_PLAYER_REACTION:
            listeningPlayerHits(PLAYER_1, player1Button);
            listeningPlayerHits(PLAYER_2, player2Button);

            checkPlayerWin(PLAYER_1);
            checkPlayerWin(PLAYER_2);
            
            changeStateAfterStateTime(GAME_STATES::GAME_OVER_RED_LEDS);
            break;
        case GAME_STATES::ROUND_OVER:
            changeStateAfterStateTime(GAME_STATES::WAITING_ROUND);
            break;
        case GAME_STATES::GAME_OVER_RED_LEDS:
            changeStateAfterStateTime(GAME_STATES::GAME_OVER);
            break;
        case GAME_STATES::GAME_OVER:
            changeStateAfterStateTime(GAME_STATES::WELCOME);
            break;
        default:
            break;
    }
}