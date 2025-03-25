#include "Game.hpp"

Game::Game() {
    // Initialize player colors
    game_player_buttons_leds[0] = PLAYER1_COLOR;
    game_player_buttons_leds[1] = PLAYER2_COLOR;
    game_player_buttons_leds[2] = PLAYER3_COLOR;
    game_player_buttons_leds[3] = PLAYER4_COLOR;
    game_player_buttons_leds[4] = PLAYER5_COLOR;
    game_player_buttons_leds[5] = PLAYER6_COLOR;
}

void Game::render() {
    switch (game_mode) {
        case IDLE:
            game_mode = next_game_mode;
            fill_rainbow(game_field_leds, NUM_GAME_FIELD_LEDS, idle_hue, 10);
            idle_hue += 1;
            
            break;
        case GAME_START:
            fill_solid(game_field_leds, NUM_GAME_FIELD_LEDS, CRGB::Black);
            if (get_time_elapsed_seconds() > 3) {
                game_mode = next_game_mode;
                game_start_time = millis();
            } else if (get_time_elapsed_seconds() > 2) {
                fill_solid(game_field_leds, NUM_GAME_FIELD_LEDS, CRGB::Green);
            } else if (get_time_elapsed_seconds() > 1) {
                fill_solid(game_field_leds + 0, 20, CRGB::Yellow);
                fill_solid(game_field_leds + 30, 20, CRGB::Yellow);
            } else if (get_time_elapsed_seconds() > 0) {
                fill_solid(game_field_leds + 0, 10, CRGB::Red);
                fill_solid(game_field_leds + 20, 10, CRGB::Red);
                fill_solid(game_field_leds + 40, 10, CRGB::Red);
            }

            if (running_since_interval_ms(0, 50) || running_since_interval_ms(983, 1033) || running_since_interval_ms(1967, 2017) || running_since_interval_ms(2950, 3000)) {
                buzzer_active = true;
            } else {
                buzzer_active = false;
            }

            break;
        case GAME_END:
            if (get_time_elapsed_seconds() > 3) {
                game_mode = IDLE;
                game_start_time = millis();
                idle_hue = 0;
            }
            fill_solid(game_field_leds, NUM_GAME_FIELD_LEDS, CRGB::Red);
            break;
        case SIMON_SAYS:
            break;
        case SPRINT:
            break;
    }
}

void Game::handleButtonPress(int buttonIndex) {
    switch (game_mode) {
        case IDLE:
            next_game_mode = GAME_START;
            break;
        case SIMON_SAYS:
            //next_game_mode = SPRINT;
            break;
        case SPRINT:
            //next_game_mode = GAME_END;
            break;
    }
} 

void Game::reset() {
    game_mode = IDLE;
    next_game_mode = IDLE;
    game_start_time = millis();
    idle_hue = 0;
    buzzer_active = false;
}

int Game::get_time_elapsed_seconds() {
    return (millis() - game_start_time) / 1000;
}

bool Game::running_since_interval_ms(uint32_t a, uint32_t b) {
    return (millis() - game_start_time) >= a && (millis() - game_start_time) <= b;
}

bool Game::is_in_interval_with_pause_between(uint32_t interval, uint32_t pause) {
    uint32_t current_time = millis() - game_start_time;
    uint32_t cycle_time = interval + pause;
    uint32_t time_in_cycle = current_time % cycle_time;

    return time_in_cycle < interval;
}
//FASTLED HAS EVERY_N_MILLISECONDS()
