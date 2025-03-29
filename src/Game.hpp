#pragma once

#include <FastLED.h>

#include "configuration.hpp"
#include "SimonSays.hpp"

extern SimonSays simonSaysGame;

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
    bool buzzer_active;

    void reset();
    void render();
    void handleButtonPress(int buttonIndex);

private:
    //always if gamemode is changed, reset the game_start_time
    int game_mode = IDLE;
    uint32_t game_start_time = millis();
    int get_time_elapsed_seconds();
    bool running_since_interval_ms(uint32_t a, uint32_t b);
    bool is_in_interval_with_pause_between(uint32_t interval, uint32_t pause);
    int next_game_mode;
    uint8_t idle_hue;
    void render_player_buttons();
    bool last_game_was_won;
};
