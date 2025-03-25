#include "LEDSetup.hpp"

void setupLEDs() {
  // Initialize game field LEDs
  FastLED.addLeds<LED_TYPE, GAME_FIELD_LED_DATA_PIN, COLOR_ORDER>(game.game_field_leds, NUM_GAME_FIELD_LEDS)
    .setCorrection(TypicalLEDStrip)
    .setDither(BRIGHTNESS < 255);

  // Initialize player buttons LEDs
  FastLED.addLeds<LED_TYPE, GAME_BUTTONS_LED_DATA_PIN, COLOR_ORDER>(game.game_player_buttons_leds, 6)
    .setCorrection(TypicalLEDStrip)
    .setDither(BRIGHTNESS < 255);

  FastLED.setBrightness(BRIGHTNESS);
  FastLED.setMaxPowerInVoltsAndMilliamps(5, MAX_CURRENT);
} 
