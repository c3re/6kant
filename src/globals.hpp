#pragma once

#include <FastLED.h>
#include <ESPAsyncWebServer.h>
#include <WebSocketsServer.h>

#include "Game.hpp"
#include "SimonSays.hpp"

extern Game game;
extern AsyncWebServer server;
extern WebSocketsServer webSocket;

