#pragma once

#define BUZZER_PIN 3

#define BTN1_PIN 12
#define BTN2_PIN 13
#define BTN3_PIN 14
#define BTN4_PIN 15
#define BTN5_PIN 16
#define BTN6_PIN 17

#define GAME_FIELD_LED_DATA_PIN 18
#define GAME_BUTTONS_LED_DATA_PIN 19

#define LED_TYPE    WS2811
#define COLOR_ORDER GRB
#define NUM_GAME_FIELD_LEDS 60
#define BRIGHTNESS  255

#define PLAYER1_COLOR CRGB(255, 0, 0) //OR 0, 51, 102 ?
#define PLAYER2_COLOR CRGB(0, 255, 0) //OR 0, 102, 51
#define PLAYER3_COLOR CRGB(0, 0, 255) //OR 102, 0, 0
#define PLAYER4_COLOR CRGB(255, 255, 0) //OR 51, 0, 102
#define PLAYER5_COLOR CRGB(0, 255, 255) //OR 0, 102, 102
#define PLAYER6_COLOR CRGB(255, 0, 255) //OR 102, 102, 0

const char *ssid = "6kant";
const char *password = "clubmate";
