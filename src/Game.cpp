#include "Game.hpp"

SimonSays simonSaysGame;

Game::Game() {
    // Initialize player colors
    render_player_buttons();

    simonSaysGame = SimonSays();
}

//TODO: all splitable in interactive and non interactive???
void Game::render() {
    switch (game_mode) {
        case IDLE:
            game_mode = next_game_mode;
            fill_rainbow(game_field_leds, NUM_GAME_FIELD_LEDS, idle_hue, 4);
            render_player_buttons();
            
            idle_hue += 1;
            
            break;
        case GAME_START:
            fill_solid(game_field_leds, NUM_GAME_FIELD_LEDS, CRGB::Black);
            fill_solid(game_player_buttons_leds, 6, CRGB::Black);

            if (get_time_elapsed_seconds() > 4) {
                //game_mode = next_game_mode;
                //fix
                game_mode = SIMON_SAYS;
                game_start_time = millis();
                simonSaysGame.reset();
            } else if (get_time_elapsed_seconds() > 3) {
                fill_solid(game_player_buttons_leds, 6, CRGB::Green);
                fill_solid(game_field_leds, NUM_GAME_FIELD_LEDS, CRGB::Black);
            } else if (get_time_elapsed_seconds() > 2) {
                fill_solid(game_field_leds, NUM_GAME_FIELD_LEDS, CRGB::Green);
            } else if (get_time_elapsed_seconds() > 1) {
                fill_solid(game_field_leds + 0, 10, CRGB::Yellow);
                fill_solid(game_field_leds + 20, 20, CRGB::Yellow);
                fill_solid(game_field_leds + 50, 10, CRGB::Yellow);
            } else if (get_time_elapsed_seconds() > 0) {
                fill_solid(game_field_leds + 0, 10, CRGB::Red);
                fill_solid(game_field_leds + 20, 10, CRGB::Red);
                fill_solid(game_field_leds + 40, 10, CRGB::Red);
            }

            if (running_since_interval_ms(0, 100) || running_since_interval_ms(1000, 1100) || running_since_interval_ms(2000, 2100) || running_since_interval_ms(3000, 4000)) {
                buzzer_active = true;
            } else {
                buzzer_active = false;
            }

            break;
        case GAME_END:
            if (get_time_elapsed_seconds() > 3) {
                reset();
            }
            if (last_game_was_won) {
                fill_solid(game_field_leds, NUM_GAME_FIELD_LEDS, CRGB::Green);
            } else {
                fill_solid(game_field_leds, NUM_GAME_FIELD_LEDS, CRGB::Red);
            }
            fill_solid(game_player_buttons_leds, 6, CRGB::Black);
            buzzer_active = true;

            break;
        case SIMON_SAYS:
            simonSaysGame.game_loop(game_field_leds, game_player_buttons_leds);
            if (simonSaysGame.isGameOver()) {
                if (simonSaysGame.isGameWon()) {
                    last_game_was_won = true;
                } else {
                    last_game_was_won = false;
                }
                //delay
                game_start_time = millis();
                next_game_mode = GAME_END;
                game_mode = GAME_END;
            } else if (!simonSaysGame.isShowMode() && !simonSaysGame.isShowModeCorrectGuess()) {
                render_player_buttons();
                fill_solid(game_field_leds, NUM_GAME_FIELD_LEDS, CRGB::Black);
                const uint8_t* sequence = simonSaysGame.getSequence();
                uint8_t currentIndex = simonSaysGame.getCurrentSequenceIndex();
            }
            break;
        case SPRINT:
            break;
    }
}

void Game::handleButtonPress(int buttonIndex) {
    Serial.println("handleButtonPress: " + String(buttonIndex));
    switch (game_mode) {
        case IDLE:
            reset();
            next_game_mode = GAME_START;
            break;
        case SIMON_SAYS:
            Serial.println(buttonIndex);
            simonSaysGame.guess(buttonIndex);
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
    simonSaysGame.reset();
    last_game_was_won = false;
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

void Game::render_player_buttons() {
    game_player_buttons_leds[0] = PLAYER1_COLOR;
    game_player_buttons_leds[1] = PLAYER2_COLOR;
    game_player_buttons_leds[2] = PLAYER3_COLOR;
    game_player_buttons_leds[3] = PLAYER4_COLOR;
    game_player_buttons_leds[4] = PLAYER5_COLOR;
    game_player_buttons_leds[5] = PLAYER6_COLOR;
}
