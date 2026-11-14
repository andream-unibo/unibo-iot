#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <string.h>
#include "screen.h"

#define LCD_ADDRESS 0x27
#define LCD_COLS 20
#define LCD_ROWS 4
#define SCROLL_INTERVAL 250
#define SCROLL_GAP 3
#define MAX_TEXT_LENGTH 64

LiquidCrystal_I2C lcd(LCD_ADDRESS, LCD_COLS, LCD_ROWS);

char currentText[MAX_TEXT_LENGTH] = "";
int textLength = 0;
bool scrolling = false;
int scrollIndex = 0;
unsigned long lastScrollTime = 0;

void setupScreen() {
    lcd.init();
    lcd.backlight();
    lcd.clear();
}

void screenSetText(const char* text) {
    strncpy(currentText, text, MAX_TEXT_LENGTH - 1);
    currentText[MAX_TEXT_LENGTH - 1] = '\0';
    textLength = strlen(currentText);
    scrollIndex = 0;
    
    lcd.clear();
    if (!scrolling && textLength > 0) {
        lcd.setCursor(0, 0);
        lcd.print(currentText);
    }
}

void screenSetScrolling(bool isScrolling) {
    scrolling = isScrolling;
    scrollIndex = 0;
    lastScrollTime = millis();

    if (!scrolling) {
        lcd.clear();
        if (textLength > 0) {
            lcd.setCursor(0, 0);
            lcd.print(currentText);
        }
    }
}

void screenShowTarget(int target) {
    scrolling = false;
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(target);
}

void screenShowPlayerScores(int score1, int score2) {
    scrolling = false;
    lcd.clear();
    
    lcd.setCursor(0, 0);
    lcd.print("P1: ");
    lcd.print(score1);

    lcd.setCursor(LCD_COLS - 8, 0);
    lcd.print("P2: ");
    lcd.print(score2);
}

void screenSetSleep(bool isSleeping) {
    if (isSleeping) {
        lcd.clear();
        lcd.noBacklight();
    } else {
        lcd.backlight();
    }
}

void screenUpdate() {
    if (!scrolling || textLength == 0) {
        return;
    }

    if (millis() - lastScrollTime > SCROLL_INTERVAL) {
        lastScrollTime = millis();
        
        char lineBuffer[LCD_COLS + 1];
        int totalLoopLength { textLength + SCROLL_GAP };

        for (int i = 0; i < LCD_COLS; i++) {
            int charIndex { (scrollIndex + i) % totalLoopLength };
            if (charIndex < textLength) {
                lineBuffer[i] = currentText[charIndex];
            } else {
                lineBuffer[i] = ' ';
            }
        }
        lineBuffer[LCD_COLS] = '\0';

        lcd.setCursor(0, 0);
        lcd.print(lineBuffer);

        scrollIndex = (scrollIndex + 1) % totalLoopLength;
    }
}