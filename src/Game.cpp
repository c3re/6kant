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
    
}

void Game::handleButtonPress(int buttonIndex) {
    
} 

void Game::reset() {
    game_mode = IDLE;
}
