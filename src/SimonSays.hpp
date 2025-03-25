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
        bool isShowMode() const;
        bool isGameOver() const;
        bool isGameWon() const;
        uint8_t getCurrentSequenceIndex() const;
        uint8_t getCurrentShowModeSequenceIndex() const;
        uint8_t getCurrentShowModeSequence() const;
        const uint8_t* getSequence() const;
    private:
        uint8_t sequence[SIMON_SAYS_SEQUENCE_LENGTH];
        uint32_t last_guess_time;
        uint32_t last_show_time;
        uint16_t current_sequence_index;
        uint16_t show_mode_sequence_index;
        bool game_over;
        bool game_won;
        bool show_mode;
};
