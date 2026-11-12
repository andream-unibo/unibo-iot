#include <Arduino.h>
#include "input.h"
#include "pins.h"
#include "avr/sleep.h"

#define DEBOUNCE_TIME 50
#define DEEP_SLEEP_TIME 10000

int getDifficulty()
{
    return map(analogRead(PIN_POTENTIOMETER), 0, 1023, 1, 4);
}

void setupInput()
{
    pinMode(BUTTON_LEFT, INPUT_PULLUP);
    pinMode(BUTTON_RIGHT, INPUT_PULLUP);
    pinMode(PIN_POTENTIOMETER, INPUT);
}


struct ButtonData
{
    int buttonPin;
    unsigned long lastPressed;
    bool lastRawState;
    bool lastCleanState;
    bool cleanInput;
};

ButtonData buttons[2] = {
  {BUTTON_LEFT, 0, HIGH, HIGH, false},
  {BUTTON_RIGHT, 0, HIGH, HIGH, false}
};

void checkButton(int player) 
{
  bool currentRawState { !digitalRead(buttons[player].buttonPin) }; 

  if (currentRawState != buttons[player].lastRawState)
  {
    buttons[player].lastPressed = millis();
  }

  if ((millis() - buttons[player].lastPressed) > DEBOUNCE_TIME)
  {
    if (currentRawState != buttons[player].lastCleanState)
    {
      buttons[player].lastCleanState = currentRawState;
      
      if (buttons[player].lastCleanState == HIGH) 
      {
        buttons[player].cleanInput = true;
      }
    }
  }
  
  buttons[player].lastRawState = currentRawState; 
}

void inputUpdate()
{
    checkButton(0);
    checkButton(1);
    getDifficulty();
}

bool getButtonState(int player)
{
    bool clicked { buttons[player].cleanInput };
    buttons[player].cleanInput = false;
    return clicked;
}

void gameSleep()
{
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);

    attachInterrupt(digitalPinToInterrupt(BUTTON_LEFT), gameWake, LOW);
    attachInterrupt(digitalPinToInterrupt(BUTTON_RIGHT), gameWake, LOW);

    sleep_mode();

    detachInterrupt(digitalPinToInterrupt(BUTTON_LEFT));
    detachInterrupt(digitalPinToInterrupt(BUTTON_RIGHT));
}

void gameWake()
{

}

