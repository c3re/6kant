#pragma once

#include <Arduino.h>
#include <FastLED.h>

#include "configuration.hpp"

class SimonSays {
    public:
        SimonSays();
        void reset();
        void game_loop();
        void guess(uint8_t button_index);
        bool game_over;
        bool game_won;
    private:
        uint8_t sequence[SIMON_SAYS_SEQUENCE_LENGTH];
        uint32_t last_guess_time;
        uint16_t current_sequence_index;
};
