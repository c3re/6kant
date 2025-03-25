#include "SimonSays.hpp"

SimonSays::SimonSays() {
    reset();
}

void SimonSays::guess(uint8_t button_index) {
    if (button_index == sequence[current_sequence_index]) {
        current_sequence_index++;
    } else {
        game_over = true;
    }
}

void SimonSays::game_loop() {
    if (millis() - last_guess_time > SIMON_SAYS_GUESS_TIME_MILLIS) {
        game_over = true;
        game_won = false;
    }
}

void SimonSays::reset() {
    for (uint8_t i = 0; i < SIMON_SAYS_SEQUENCE_LENGTH; i++) {
        sequence[i] = random8(0, 6);
    }
    current_sequence_index = 0;
    game_over = false;
    game_won = false;
    last_guess_time = millis();
}
