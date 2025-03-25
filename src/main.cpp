#include <Arduino.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <WebSocketsServer.h>

#include "globals.hpp"
#include "www.html.hpp"

AsyncWebServer server(80);
WebSocketsServer webSocket = WebSocketsServer(81);

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

void sendColorData(uint8_t num);

void setup() {
  delay(3500);
  WiFi.softAP(ssid, password);

  webSocket.begin();
  //webSocket.onEvent(webSocketEvent);

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
  
  server.on("/BTN/1", HTTP_GET, [](AsyncWebServerRequest *request){
    game.handleButtonPress(0);
    request->send(200);
  });

  server.on("/BTN/2", HTTP_GET, [](AsyncWebServerRequest *request){
    game.handleButtonPress(1);
    request->send(200);
  });

  server.on("/BTN/3", HTTP_GET, [](AsyncWebServerRequest *request){
    game.handleButtonPress(2);
    request->send(200);
  });

  server.on("/BTN/4", HTTP_GET, [](AsyncWebServerRequest *request){
    game.handleButtonPress(3);
    request->send(200);
  }); 

  server.on("/BTN/5", HTTP_GET, [](AsyncWebServerRequest *request){
    game.handleButtonPress(4);
    request->send(200);
  });

  server.on("/BTN/6", HTTP_GET, [](AsyncWebServerRequest *request){
    game.handleButtonPress(5);
    request->send(200);
  });

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/html", htmlCode);
  });
  
  server.begin();
}

void loop() {
  game.render();
  digitalWrite(BUZZER_PIN, game.buzzer_active);
  
  FastLED.show();
  //webSocket.broadcastBIN((uint8_t*) "\x01", 1);
  sendColorData(0x02);

  uint32_t current_time = millis();
  while (millis() - current_time < 20) {
    webSocket.loop();
  }
}

void sendColorData(uint8_t num) {
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
