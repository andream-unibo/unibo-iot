#include <Arduino.h>
#include "gameEngine.h"
#include "leds.h"
#include "screen.h"
#include "input.h"

// To differentiate stage setup from stage loop
GAME_STATES previousState = (GAME_STATES) -1;

void welcomeSetup();

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
			case GAME_STATES::INITIALIZING: 
				welcomeSetup(); 
				break;
		}
		previousState = currentState;
	}

	gameEngineUpdate();
	ledsUpdate();
	inputUpdate();
	screenUpdate();
}

void welcomeSetup() {
	screenSetText("Welcome to the React! Game. Press Buttons to Start");
	screenSetScrolling(true);
	ledsSetFading(true);
}

