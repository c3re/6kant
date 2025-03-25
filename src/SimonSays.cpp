#include "SimonSays.hpp"

SimonSays::SimonSays() {
    reset();
}

void SimonSays::guess(uint8_t button_index) {
    if (isShowMode()) {
        return;
    }

    if (button_index == sequence[current_sequence_index]) {
        current_sequence_index++;
        if (current_sequence_index >= SIMON_SAYS_SEQUENCE_LENGTH) {
            game_won = true;
            game_over = true;

            return;
        }

        show_mode = true;
        last_show_time = millis();
        show_mode_sequence_index = 0;
    } else {
        game_over = true;
        game_won = false;
    }
}

bool SimonSays::isShowMode() const {
    return show_mode;
}

void SimonSays::game_loop() {
    if (show_mode) {
        last_guess_time = millis();
        if (millis() - last_show_time > SIMON_SAYS_SHOW_MODE_TIME_MILLIS) {
            if(show_mode_sequence_index >= current_sequence_index) {
                show_mode = false;
                show_mode_sequence_index = 0;
                return;
            }

            show_mode_sequence_index++;
            last_show_time = millis();
        }

        return;
    }

    last_show_time = millis();
    show_mode_sequence_index = 0;
    if (millis() - last_guess_time > SIMON_SAYS_GUESS_TIME_MILLIS) {
        game_over = true;
        game_won = false;
    }
}

void SimonSays::reset() {
    rand16seed = random();
    for (uint8_t i = 0; i < SIMON_SAYS_SEQUENCE_LENGTH; i++) {
        sequence[i] = random8(0, 6);
    }
    current_sequence_index = 0;
    game_over = false;
    game_won = false;
    last_guess_time = millis();
    last_show_time = millis();
    show_mode = true;
    show_mode_sequence_index = sequence[0];
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

uint8_t SimonSays::getCurrentShowModeSequenceIndex() const {
    return show_mode_sequence_index;
}

const uint8_t* SimonSays::getSequence() const {
    return sequence;
}

uint8_t SimonSays::getCurrentShowModeSequence() const {
    return sequence[show_mode_sequence_index];
}
