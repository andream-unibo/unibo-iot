#include <Arduino.h>
#include "leds.h"
#include "pins.h"

#define FADE_INTERVAL 10
#define LED_TIMEOUT 2000
#define MAX_BRIGHTNESS 255
#define MIN_BRIGHTNESS 0
#define FADE_STEP 5

struct PlayerLeds {
    int greenPin;
    int redPin;
    unsigned long greenTimerStart;
    unsigned long redTimerStart;
    bool greenActive;
    bool redActive;
};

PlayerLeds playerLeds[2] = {
    {GREEN_LED_LEFT, RED_LED_LEFT, 0, 0, false, false},
    {GREEN_LED_RIGHT, RED_LED_RIGHT, 0, 0, false, false}
};

bool isFading = false;
int fadeValue = 0;
int currentFadeStep = FADE_STEP;
unsigned long lastFadeTime = 0;

void setupLeds() {
    for (int i = 0; i < 2; i++) {
        pinMode(playerLeds[i].greenPin, OUTPUT);
        pinMode(playerLeds[i].redPin, OUTPUT);
    }
    ledsSetFading(false);
}

void ledsSetFading(bool active) {
    isFading = active;
    if (!active) {
        analogWrite(RED_LED_LEFT, MIN_BRIGHTNESS);
        analogWrite(RED_LED_RIGHT, MIN_BRIGHTNESS);
    } else {
        fadeValue = MIN_BRIGHTNESS;
        currentFadeStep = FADE_STEP;
        lastFadeTime = millis();
    }
}

void turnGreenOn(int player) {
    digitalWrite(playerLeds[player].greenPin, HIGH);
    playerLeds[player].greenActive = true;
    playerLeds[player].greenTimerStart = millis();
}

void turnGreenOff(int player) {
    digitalWrite(playerLeds[player].greenPin, LOW);
    playerLeds[player].greenActive = false;
}

void turnRedOn(int player) {
    if (isFading) {
        return;
    }
    digitalWrite(playerLeds[player].redPin, HIGH);
    playerLeds[player].redActive = true;
    playerLeds[player].redTimerStart = millis();
}

void turnRedOff(int player) {
    if (isFading) {
        return;
    }
    digitalWrite(playerLeds[player].redPin, LOW);
    playerLeds[player].redActive = false;
}

void updateFading() {
    if (millis() - lastFadeTime > FADE_INTERVAL) {
        lastFadeTime = millis();
        fadeValue += currentFadeStep;

        if (fadeValue <= MIN_BRIGHTNESS || fadeValue >= MAX_BRIGHTNESS) {
            currentFadeStep = -currentFadeStep;
            fadeValue = constrain(fadeValue, MIN_BRIGHTNESS, MAX_BRIGHTNESS);
        }

        analogWrite(RED_LED_LEFT, fadeValue);
        analogWrite(RED_LED_RIGHT, MAX_BRIGHTNESS - fadeValue);
    }
}

void checkLedTimeouts(int player) {
    if (playerLeds[player].greenActive && (millis() - playerLeds[player].greenTimerStart > LED_TIMEOUT)) {
        turnGreenOff(player);
    }
    if (!isFading && playerLeds[player].redActive && (millis() - playerLeds[player].redTimerStart > LED_TIMEOUT)) {
        turnRedOff(player);
    }
}

void ledsUpdate() {
    if (isFading) {
        updateFading();
    }
    checkLedTimeouts(0);
    checkLedTimeouts(1);
}