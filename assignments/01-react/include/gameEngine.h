#ifndef GAME_ENGINE_H
#define GAME_ENGINE_H

enum GAME_STATES
{
    WELCOME,
    GAME_START,
    WAITING_ROUND,
    WAITING_PLAYER_REACTION,
    ROUND_OVER,
    GAME_OVER_RED_LEDS,
    GAME_OVER
};

void setupGameEngine(unsigned long seed);
void playerLoss(int player);
void playerWin(int player);
void checkPlayerEarlyTap(int player, bool button);
void listeningPlayerHits(int player, bool button);
void checkPlayerWin(int player);
void playerResetRound(int player);
void newRound();
void changeState(GAME_STATES newState);
void gameEngineUpdate();


#endif