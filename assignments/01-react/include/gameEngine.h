#ifndef GAME_ENGINE_H
#define GAME_ENGINE_H

enum GAME_STATES { INIT };

/**
 * Set up the game engine with a seed for random generation.
*/
void setupGameEngine(unsigned long seed);

int randomNumber();

int addPoint(int player);

int removePoint(int player);

int startRound(int number);

#endif