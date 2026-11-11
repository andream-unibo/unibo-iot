#ifndef INPUT_H
#define INPUT_H

enum Difficulty { EASY, MEDIUM, HARD, IMPOSSIBLE };

/**
 * Get button state for each player
*/
bool getButtonState(int player);

/**
 * Current game difficulty mapped from potentiometer
*/
Difficulty getDifficulty();

/**
 * Defines pin modes
*/
void setupInput();

/**
 * Places the system into sleep mode
*/
void gameSleep();

/**
 * System wake up handler
*/
void gameWake();

/**
 * Updates input variables also caring about debounce
*/
void inputUpdate();

#endif

