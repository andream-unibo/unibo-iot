#include <Arduino.h>
#include "gameEngine.h"
#include "leds.h"
#include "screen.h"
#include "input.h"

void setup()
{
	setupLeds();
	setupInput();
	setupScreen();
	setupGameEngine(analogRead(A5));
}
	
void loop()
{
	gameEngineUpdate();
	ledsUpdate();
	inputUpdate();
	screenUpdate();
}