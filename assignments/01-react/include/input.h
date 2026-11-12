#ifndef INPUT_H
#define INPUT_H

/**
 * Return the difficulty level for the next game.
 * It will use PWM to map the Potentiometer into the difficulty levels.
 */
int getDifficulty();

/**
 * Setup pinModes of Buttons and Potentiometer.
 */
void setupInput();

/**
 * Verifies the current state of a Button for the specified player with debouncing.
 * It saves the clean state of the specific Button.
 */
void checkButton(int player);

/**
 * Verifies if the last press was before or after DEEP_SLEEP_TIME.
 */
bool checkLastPressed(int player);

/**
 * Update Buttons and Potentiometer current state.
 */
void inputUpdate();

/**
 * Get the clean state of a Button for the specified player.
 * Return true if the Button is pressed, false otherwise.
 */
bool getButtonState(int player);

/**
 * Set the system into Deep Sleep after 10 seconds, if no Button is pressed.
 */
void gameSleep();

/**
 * Awake the system from Deep Sleep when each of the 2 Buttons is pressed.
 */
void gameWake();

#endif

