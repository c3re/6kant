#include "WebServerSetup.hpp"

extern AsyncWebServer server;

void setupWebServer() {
  server.on("/BTN/1", HTTP_GET, [](AsyncWebServerRequest *request){
    game.handleButtonPress(0);
    request->send(200);
  });

  server.on("/BTN/2", HTTP_GET, [](AsyncWebServerRequest *request){
    game.handleButtonPress(1);
    request->send(200);
  });

  server.on("/BTN/3", HTTP_GET, [](AsyncWebServerRequest *request){
    game.handleButtonPress(2);
    request->send(200);
  });

  server.on("/BTN/4", HTTP_GET, [](AsyncWebServerRequest *request){
    game.handleButtonPress(3);
    request->send(200);
  });

  server.on("/BTN/5", HTTP_GET, [](AsyncWebServerRequest *request){
    game.handleButtonPress(4);
    request->send(200);
  });

  server.on("/BTN/6", HTTP_GET, [](AsyncWebServerRequest *request){
    game.handleButtonPress(5);
    request->send(200);
  });

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/html", htmlCode);
  });

  server.begin();
} 