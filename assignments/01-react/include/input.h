#ifndef INPUT_H
#define INPUT_H

// Pins 2 and 3 are needed for int0 e int1 to wake up the system.
#define BTN_1 2
#define BTN_2 3

enum Difficulty { EASY, MEDIUM, HARD, IMPOSSIBLE };

int getButtonState(int player);

Difficulty getDifficulty();

void gameSleep();

void gameWake();

void inputUpdate();

#endif

