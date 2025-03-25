#include <Arduino.h>

#include "globals.hpp"

void setup() {
  delay(3500);
  
  FastLED.addLeds<LED_TYPE, GAME_FIELD_LED_DATA_PIN, COLOR_ORDER>(game_field_leds, NUM_GAME_FIELD_LEDS)
    .setCorrection(TypicalLEDStrip)
    .setDither(BRIGHTNESS < 255);

  FastLED.addLeds<LED_TYPE, GAME_BUTTONS_LED_DATA_PIN, COLOR_ORDER>(game_buttons_leds, 6)
    .setCorrection(TypicalLEDStrip)
    .setDither(BRIGHTNESS < 255);

  FastLED.setBrightness(BRIGHTNESS);
}

void loop() {
  FastLED.show();
}