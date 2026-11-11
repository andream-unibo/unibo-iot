#ifndef GAME_ENGINE_H
#define GAME_ENGINE_H

enum GAME_STATES { INIT, ROUND };

/**
 * Set up the game engine with a seed for random generation.
*/
void setupGameEngine(unsigned long seed);

int randomNumber();

/**
 * Adds a point to a Player.
 * 
 * Param: 1 or 2.
*/
void addPoint(int player);

/**
 * Remove a point from a Player.
 * 
 * Param: 1 or 2.
*/
void removePoint(int player);

/**
 * Returns current game state.
*/
GAME_STATES getCurrentState();

/**
 * Switches game state to Round state.
*/
void startRound(int number);

/**
 * Resets the game (player points) and goes back to initial welcome stage.
*/
void reset();

#endif