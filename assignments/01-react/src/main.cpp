#include <Arduino.h>
#include "gameEngine.h"
#include "leds.h"
#include "screen.h"
#include "input.h"

// To differentiate stage setup from stage loop
GAME_STATES previousState = (GAME_STATES) -1;

void welcomeSetup();
void welcome();

void setup()
{
	setupLeds();
	setupInput();
	setupScreen();
	setupGameEngine(analogRead(A5));
	reset();
}

void loop()
{

	GAME_STATES currentState = getCurrentState();

	// Game state setup, executed only when state changes
	if (currentState != previousState) {
		switch (currentState) {
			case GAME_STATES::INIT: 
				welcomeSetup(); 
				break;
		}
		previousState = currentState;
	}

	// Game state loop, executed every tick
	switch (currentState)
	{
		case GAME_STATES::INIT:
			welcome();
			break;
	}

	// Update leds, input and screen every tick
	ledsUpdate();
	inputUpdate();
	screenUpdate();
}

void welcomeSetup() {
	screenSetText("Welcome to the React! Game. Press Buttons to Start");
	screenSetScrolling(true);
	ledsSetFading(true);
}

void welcome() {
	if (getButtonState(1) && getButtonState(2)) {
		startRound(1);
	}
}


