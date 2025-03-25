#pragma once

#include <FastLED.h>
#include "configuration.hpp"

enum GameMode {
    IDLE,
    GAME_START,
    GAME_END,
    SIMON_SAYS,
    SPRINT
};

class Game {
public:
    Game();

    CRGB game_field_leds[NUM_GAME_FIELD_LEDS];
    CRGB game_player_buttons_leds[6];
    
    void reset();
    void render();
    void handleButtonPress(int buttonIndex);

private:
    int game_mode = IDLE;
};
