#include <Arduino.h>
#include <WiFi.h>

#include "ButtonHandlers.hpp"
#include "LEDSetup.hpp"
#include "WebServerSetup.hpp"

void sendColorData();

void setup() {
  delay(3500);
  WiFi.softAP(ssid, password);

  randomSeed(analogRead(0));

  webSocket.begin();
  setupLEDs();

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

  setupWebServer();

  game.reset();
}

void loop() {
  game.render();
  digitalWrite(BUZZER_PIN, game.buzzer_active);
  
  FastLED.show();
  sendColorData();

  uint32_t current_time = millis();
  while (millis() - current_time < 20) {
    webSocket.loop();
  }
}

void sendColorData() {
  uint8_t colorData[NUM_GAME_FIELD_LEDS * 3 + 6 * 3];

  for (int i = 0; i < NUM_GAME_FIELD_LEDS; i++) {
    colorData[i * 3]     = game.game_field_leds[i].r;  // R
    colorData[i * 3 + 1] = game.game_field_leds[i].g;  // G
    colorData[i * 3 + 2] = game.game_field_leds[i].b;  // B
  }
  
  for (int i = 0; i < 6; i++) {
    colorData[NUM_GAME_FIELD_LEDS * 3 + i * 3]     = game.game_player_buttons_leds[i].r;  // R
    colorData[NUM_GAME_FIELD_LEDS * 3 + i * 3 + 1] = game.game_player_buttons_leds[i].g;  // G
    colorData[NUM_GAME_FIELD_LEDS * 3 + i * 3 + 2] = game.game_player_buttons_leds[i].b;  // B
  }

  webSocket.broadcastBIN(colorData, sizeof(colorData));
}
