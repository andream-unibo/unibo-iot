#ifndef GAME_ENGINE_H
#define GAME_ENGINE_H

enum GAME_STATES
{
    INITIALIZING,
    WAITING_ROUND,
    WAITING_PLAYER_REACTION,
    ROUND_OVER
};

/**
 * Set up the game engine with a seed for random generation.
*/
void setupGameEngine(unsigned long seed);

/**
 * Adds a point to a Player.
 * 
 * Param: 1 or 2.
*/
void incrementPointsFor(int player);

/**
 * Remove a point from a Player.
 * 
 * Param: 1 or 2.
*/
void decrementPointsFor(int player);

void setPointsOf(int player, int points);

int getPointsOf(int player);

/**
 * Returns current game state.
*/
GAME_STATES getCurrentState();

/**
 * Resets the game (player points) and goes back to initial welcome stage.
*/
void reset();

/**
 * Switches game state to Round state.
*/
void startRoundWaiting();

/**
 * Executed every game tick.
*/
void gameEngineUpdate();

#endif