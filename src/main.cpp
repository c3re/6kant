#include <Arduino.h>

#include "globals.hpp"

void IRAM_ATTR onButtonPress1() {
    game.handleButtonPress(0);
}

void IRAM_ATTR onButtonPress2() {
    game.handleButtonPress(1);
}

void IRAM_ATTR onButtonPress3() {
    game.handleButtonPress(2);
}

void IRAM_ATTR onButtonPress4() {
    game.handleButtonPress(3);
}

void IRAM_ATTR onButtonPress5() {
    game.handleButtonPress(4);
}

void IRAM_ATTR onButtonPress6() {
    game.handleButtonPress(5);
}

void setup() {
  delay(3500);
  
  // Initialize game field LEDs
  FastLED.addLeds<LED_TYPE, GAME_FIELD_LED_DATA_PIN, COLOR_ORDER>(game.game_field_leds, NUM_GAME_FIELD_LEDS)
    .setCorrection(TypicalLEDStrip)
    .setDither(BRIGHTNESS < 255);

  // Initialize player buttons LEDs
  FastLED.addLeds<LED_TYPE, GAME_BUTTONS_LED_DATA_PIN, COLOR_ORDER>(game.game_player_buttons_leds, 6)
    .setCorrection(TypicalLEDStrip)
    .setDither(BRIGHTNESS < 255);

  FastLED.setBrightness(BRIGHTNESS);

  // Initialize buzzer
  pinMode(BUZZER_PIN, OUTPUT);

  // Initialize game
  game = Game();

  // Set up interrupts for player buttons
  attachInterrupt(digitalPinToInterrupt(BTN1_PIN), onButtonPress1, FALLING);
  attachInterrupt(digitalPinToInterrupt(BTN2_PIN), onButtonPress2, FALLING);
  attachInterrupt(digitalPinToInterrupt(BTN3_PIN), onButtonPress3, FALLING);
  attachInterrupt(digitalPinToInterrupt(BTN4_PIN), onButtonPress4, FALLING);
  attachInterrupt(digitalPinToInterrupt(BTN5_PIN), onButtonPress5, FALLING);
  attachInterrupt(digitalPinToInterrupt(BTN6_PIN), onButtonPress6, FALLING);
}

void loop() {
  game.render();
  
  FastLED.show();

  delay(20);
}
