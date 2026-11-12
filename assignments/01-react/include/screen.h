#ifndef SCREEN_H
#define SCREEN_H

void setupScreen();

void screenUpdate();

void screenShowTarget(int target);

void screenShowPlayerScores(int score1, int score2);

void screenSetText(String text);

void screenSetScrolling(bool isScrolling);

void screenSetSleep(bool isSleeping);

#endif