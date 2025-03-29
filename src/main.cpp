#include <Arduino.h>
#include <WiFi.h>

#include "ButtonHandlers.hpp"
#include "LEDSetup.hpp"
#include "WebServerSetup.hpp"

#define NUM_BUTTONS 6
const int buttonPins[NUM_BUTTONS] = {BTN1_PIN, BTN2_PIN, BTN3_PIN, BTN4_PIN, BTN5_PIN, BTN6_PIN};
volatile bool buttonPressed[NUM_BUTTONS] = {false};
volatile unsigned long lastPressTime[NUM_BUTTONS] = {0};

void sendColorData();
void handleButtonPress();

void setup() {
  delay(3500);
  Serial.begin(115200);
  WiFi.softAP(ssid, password);

  randomSeed(analogRead(0));

  webSocket.begin();
  setupLEDs();

  // Initialize buzzer
  pinMode(BUZZER_PIN, OUTPUT);

  // Initialize game
  game = Game();

  // Set up interrupts for player buttons
  for (int i = 0; i < NUM_BUTTONS; i++) {
    pinMode(buttonPins[i], INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(buttonPins[i]), handleButtonPress, FALLING);
  }

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

  for (int i = 0; i < NUM_BUTTONS; i++) {
    if (buttonPressed[i]) {
      // Rufe die Funktion aus der Game-Klasse auf
      game.handleButtonPress(i);
      buttonPressed[i] = false; // Zurücksetzen
    }
  }
}

void handleButtonPress() {
  unsigned long currentTime = millis();
  for (int i = 0; i < NUM_BUTTONS; i++) {
    if (digitalRead(buttonPins[i]) == LOW) {
      if (currentTime - lastPressTime[i] > 20) { // Entprellung
        buttonPressed[i] = true;
        lastPressTime[i] = currentTime;
      }
    }
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
