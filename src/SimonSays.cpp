#include "SimonSays.hpp"

SimonSays::SimonSays() {
    reset();
}

void SimonSays::game_loop(CRGB* game_field_leds, CRGB* game_player_buttons_leds) {
    if (show_mode) {
        fill_solid(game_player_buttons_leds, 6, CRGB::Black);
        if (millis() - animation_timer > SIMON_SAYS_SHOW_MODE_TIME_MILLIS) {
            current_sequence_index++;
            if(current_sequence_index > guessed) {
                guessed = current_sequence_index;
                SwitchToGuessMode();
                
                return;
            }

            animation_timer = millis();

            return;
        }

        if(millis() - animation_timer > 750) {
            fill_solid(game_field_leds, NUM_GAME_FIELD_LEDS, CRGB::Black);

            return;
        }

        ColorizeField(game_field_leds, sequence[current_sequence_index]);

        return;
    }

    if(show_mode_correct_guess) {
        if(millis() - animation_timer > 750) {
            if(guessed >= SIMON_SAYS_SEQUENCE_LENGTH) {
                game_over = true;
                game_won = true;

                return;
            }

            SwitchToGuessMode();
        }

        ColorizeField(game_field_leds, sequence[guessed]);
    }

    if (millis() - last_press > SIMON_SAYS_GUESS_TIME_MILLIS) {
        game_over = true;
        game_won = false;
    }
}

void SimonSays::reset() {
    Serial.println("reset");
    rand16seed = random();
    for (uint8_t i = 0; i < SIMON_SAYS_SEQUENCE_LENGTH; i++) {
        sequence[i] = random8(0, 6);
    }
    Serial.print("randoms: ");
    for (uint8_t i = 0; i < SIMON_SAYS_SEQUENCE_LENGTH; i++) {
        Serial.print(sequence[i]);
        Serial.print(" ");
    }
    Serial.println();
    current_sequence_index = 0;
    guessed = 0;
    animation_timer = millis();
    game_over = false;
    game_won = false;
    last_press = millis();
    show_mode = true;
    show_mode_correct_guess = false;
    //show_mode_sequence_index = sequence[0];
}

void SimonSays::SwitchToGuessMode() {
    show_mode = false;
    show_mode_correct_guess = false;
    current_sequence_index = 0;
    animation_timer = millis();
    last_press = millis();
}

void SimonSays::SwitchToShowMode() {
    show_mode = true;
    show_mode_correct_guess = false;
    current_sequence_index = 0;
    animation_timer = millis();
}

void SimonSays::SwitchToShowModeCorrectGuess() {
    show_mode = false;
    show_mode_correct_guess = true;
    animation_timer = millis();
}

void SimonSays::ColorizeField(CRGB* game_field_leds, uint8_t player_id) {
    fill_solid(game_field_leds, NUM_GAME_FIELD_LEDS, CRGB::Black);
    switch(player_id) {
        case 0:
            fill_solid(game_field_leds + 0, 10, PLAYER1_COLOR);
            break;
        case 1:
            fill_solid(game_field_leds + 10, 10, PLAYER2_COLOR);
            break;
        case 2:
            fill_solid(game_field_leds + 20, 10, PLAYER3_COLOR);
            break;
        case 3:
            fill_solid(game_field_leds + 30, 10, PLAYER4_COLOR);
            break;
        case 4:
            fill_solid(game_field_leds + 40, 10, PLAYER5_COLOR);
            break;
        case 5:
            fill_solid(game_field_leds + 50, 10, PLAYER6_COLOR);
        break;
    }
}

void SimonSays::guess(uint8_t button_index) {
    if (isShowMode()) {
        return;
    }

    if(show_mode_correct_guess) {
        show_mode_correct_guess = false;
    }

    Serial.print("should be: ");
    Serial.println(sequence[current_sequence_index]);
    Serial.print("current_sequence_index: ");
    Serial.println(current_sequence_index);

    last_press = millis();
    if (button_index == sequence[current_sequence_index]) {
        current_sequence_index++;
        if(current_sequence_index > guessed) {
            guessed++;
        }
        if (current_sequence_index >= guessed) {
            if (guessed >= SIMON_SAYS_SEQUENCE_LENGTH) {
                game_won = true;
                game_over = true;
                return;
            }
            SwitchToShowMode();
        }
    } else {
        game_over = true;
        game_won = false;
    }
}

bool SimonSays::isShowMode() const {
    return show_mode;
}

bool SimonSays::isShowModeCorrectGuess() const {
    return show_mode_correct_guess;
}

bool SimonSays::isGameOver() const {
    return game_over;
}

bool SimonSays::isGameWon() const {
    return game_won;
}

uint8_t SimonSays::getCurrentSequenceIndex() const {
    return current_sequence_index;
}

const uint8_t* SimonSays::getSequence() const {
    return sequence;
}