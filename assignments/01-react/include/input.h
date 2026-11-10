#ifndef INPUT_H
#define INPUT_H

// Pins 2 and 3 are needed for int0 e int1 to wake up the system.
#define BTN_1 2
#define BTN_2 3

enum Difficulty { EASY, MEDIUM, HARD, IMPOSSIBLE };

/**
 * Get the state of a button for the specified player.
 * Return true if the button is pressed, false otherwise.
 */
bool getButtonState(int player);

/**
 * Return the difficulty level for the next game.
 */
Difficulty getDifficulty();

/**
 * Setup pinModes of buttons and potentiometer
 */
void setupInput();

/**
 * Set the system into deep sleep 
 */
void gameSleep();

/**
 * Awake the system from deep sleep 
 */
void gameWake();

/**
 * Update buttons and potentiometer to the current input
 */
void inputUpdate();

#endif

