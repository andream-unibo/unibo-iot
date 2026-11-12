#include "gameEngine.h"
#include "input.h"
#include "screen.h"
#include "leds.h"
#include <Arduino.h>

#define PLAYER_1 0
#define PLAYER_2 1

#define MAX_WAIT 7500 + 1

#define LOSE_VALUE -1

#define RANDOM_MIN 1
#define RANDOM_MAX 3 + 1

#define WIN_CHECK_WAIT 250

#define WIN_TIMER 2000
#define GAMEOVER_TIMER 10000
#define START_TRANSITION_TIME 300

#define INITIAL_T1 10000
#define DIFFICULTY_FACTOR 2

struct Player {
    int points;
    int roundClicks;
    unsigned long winTimeStart;
};

Player players[2];

// milliseconds
unsigned long stateTimer;
unsigned long stateTimerStart;

unsigned long playerTimeOut;

int roundObjective = 0;

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
    if (players[player].roundClicks > roundObjective) {
        playerLoss(player);
    }
    else if (players[player].roundClicks == roundObjective) {
        players[player].winTimeStart = millis();
    }
}

void checkPlayerWin(int player) {
    if (players[player].winTimeStart == 0) {
        return;
    }
    bool winTimeCheck = millis() - players[player].winTimeStart > WIN_CHECK_WAIT;
    bool pointsCheck = players[player].roundClicks == roundObjective;
    if (winTimeCheck && pointsCheck) {
        playerWin(player);
    }
}

void playerNewRound(int player) {
    players[player].roundClicks = 0;
    players[player].winTimeStart = 0;
}

void newRound() {
    playerNewRound(PLAYER_1);
    playerNewRound(PLAYER_2);
    stateTimer = random(0, MAX_WAIT);
    stateTimerStart = millis();
    roundObjective = random(RANDOM_MIN, RANDOM_MAX);
    playerTimeOut -= playerTimeOut * (5 * DIFFICULTY_FACTOR) / 100;
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
            playerTimeOut = INITIAL_T1;
            players[PLAYER_1].points = 0;
            players[PLAYER_2].points = 0;
            break;
        case GAME_STATES::GAME_START:
            screenSetText("Go!");
            screenSetScrolling(false);
            ledsSetFading(false);
            turnRedOff(PLAYER_1);
            turnRedOff(PLAYER_2);
            break;
        case GAME_STATES::WAITING_ROUND:
            screenSetText("");
            turnGreenOff(PLAYER_1);
            turnGreenOff(PLAYER_2);
            turnRedOff(PLAYER_1);
            turnRedOff(PLAYER_2);
            newRound();
            break;
        case GAME_STATES::WAITING_PLAYER_REACTION:
            screenShowTarget(roundObjective);
            break;
        case GAME_STATES::ROUND_OVER:
            screenShowPlayerScores(players[PLAYER_1].points, players[PLAYER_2].points);
            break;
        case GAME_STATES::GAME_OVER:
            if (players[PLAYER_1].points > players[PLAYER_2].points) {
                screenSetText("Game Over - The winner is Player 1");
            } else if (players[PLAYER_1].points < players[PLAYER_2].points) {
                screenSetText("Game Over - The winner is Player 2");
            } else {
                screenSetText("Game Over - Tie");
            }
            screenSetScrolling(true);
            break;
        default:
            break;
    }
    currentState = newState;
    stateTimerStart = millis();
}

void gameEngineUpdate() {
    bool player1Button = getButtonState(PLAYER_1);
    bool player2Button = getButtonState(PLAYER_2);

    switch (currentState) {
        case GAME_STATES::WELCOME:
            if (player1Button && player2Button) {
                changeState(GAME_STATES::GAME_START);
            }
            break;
        case GAME_STATES::GAME_START:
            if (millis() - stateTimerStart > START_TRANSITION_TIME) {
                changeState(GAME_STATES::WAITING_ROUND);
            }
            break;
        case GAME_STATES::WAITING_ROUND:
            checkPlayerEarlyTap(PLAYER_1, player1Button);
            checkPlayerEarlyTap(PLAYER_2, player2Button);
            
            if (millis() - stateTimerStart > stateTimer) {
                stateTimerStart = millis();
                changeState(GAME_STATES::WAITING_PLAYER_REACTION);
            }
            break;
        case GAME_STATES::WAITING_PLAYER_REACTION:
            listeningPlayerHits(PLAYER_1, player1Button);
            listeningPlayerHits(PLAYER_2, player2Button);

            checkPlayerWin(PLAYER_1);
            checkPlayerWin(PLAYER_2);
            
            if (millis() - stateTimerStart > playerTimeOut) {
                changeState(GAME_STATES::GAME_OVER);
            }
            break;
        case GAME_STATES::ROUND_OVER:
            if (millis() - stateTimerStart > WIN_TIMER) {
                changeState(GAME_STATES::WAITING_ROUND);
            }
            break;
        case GAME_STATES::GAME_OVER:
            if (millis() - stateTimerStart > GAMEOVER_TIMER) {
                changeState(GAME_STATES::WELCOME);
            }
        default:
            break;
    }
}