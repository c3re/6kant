#include <Arduino.h>
#include "globals.hpp"
#include "ButtonHandlers.hpp"

void IRAM_ATTR onButtonPress1() {
  game.handleButtonPress(0);
}

void IRAM_ATTR onButtonPress2() {
  game.handleButtonPress(1);
}

void IRAM_ATTR onButtonPress3() {
  game.handleButtonPress(2);
}

void IRAM_ATTR onButtonPress4() {
  game.handleButtonPress(3);
}

void IRAM_ATTR onButtonPress5() {
  game.handleButtonPress(4);
}

void IRAM_ATTR onButtonPress6() {
  game.handleButtonPress(5);
} 
