#pragma once

#include <Arduino.h>
#include <FastLED.h>

#include "configuration.hpp"

class SimonSays {
    public:
        SimonSays();
        void reset();
        void game_loop(CRGB* game_field_leds, CRGB* game_player_buttons_leds);
        void guess(uint8_t button_index);
        bool isShowMode() const;
        bool isShowModeCorrectGuess() const;
        bool isGameOver() const;
        bool isGameWon() const;
        uint8_t getCurrentSequenceIndex() const;
        uint8_t getCurrentShowModeSequence() const;
        const uint8_t* getSequence() const;
        void SwitchToGuessMode();
        void SwitchToShowMode();
        void SwitchToShowModeCorrectGuess();
        void ColorizeField(CRGB* game_field_leds, uint8_t player_id);
    private:
        uint32_t last_press;
        uint32_t animation_timer;
        uint16_t current_sequence_index;
        uint16_t guessed;
        uint8_t sequence[SIMON_SAYS_SEQUENCE_LENGTH];

        bool game_over;
        bool game_won;
        bool show_mode;
        bool show_mode_correct_guess;
};
