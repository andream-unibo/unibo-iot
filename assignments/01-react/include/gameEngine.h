#ifndef GAME_ENGINE_H
#define GAME_ENGINE_H

enum GAME_STATES { INIT, };

int randomNumber();

int addPoint(int player);

int removePoint(int player);

int startRound(int number);

#endif