#include <Arduino.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <WebSocketsServer.h>

#include "globals.hpp"

AsyncWebServer server(80);
WebSocketsServer webSocket = WebSocketsServer(81);

const char* htmlCode = R"rawliteral(
<!DOCTYPE html>
<html lang="de">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>LED Strip Steuerung</title>
    <style>
        .led-strip {
            display: flex;
            flex-wrap: wrap;
            width: 400px;
            height: 400px;
            position: relative;
            justify-content: center;
            align-items: center;
        }
        .led {
            width: 20px;
            height: 20px;
            margin: 2px;
            border-radius: 50%;
            position: absolute;
        }
        .button {
            position: absolute;
            margin: 10px;
            padding: 10px;
            font-size: 16px;
            border-radius: 50%;
        }
        .button1 { top: -30px; left: 50%; transform: translateX(-50%); }
        .button2 { top: 50%; right: -30px; transform: translateY(-50%); }
        .button3 { bottom: -30px; left: 50%; transform: translateX(-50%); }
        .button4 { top: 50%; left: -30px; transform: translateY(-50%); }
        .button5 { top: 10%; left: 10%; }
        .button6 { bottom: 10%; right: 10%; }
    </style>
</head>
<body>
    <h1>LED Strip Steuerung</h1>
    <div class="led-strip" id="gameFieldStrip"></div>
    <button class="button button1" onclick="sendColorChange(1)">Player 1</button>
    <button class="button button2" onclick="sendColorChange(2)">Player 2</button>
    <button class="button button3" onclick="sendColorChange(3)">Player 3</button>
    <button class="button button4" onclick="sendColorChange(4)">Player 4</button>
    <button class="button button5" onclick="sendColorChange(5)">Player 5</button>
    <button class="button button6" onclick="sendColorChange(6)">Player 6</button>
    <script>
        const socket = new WebSocket('ws://' + window.location.hostname + ':81');
        const gameFieldStrip = document.getElementById("gameFieldStrip");
        const numGameFieldLeds = 60;
        for (let i = 0; i < numGameFieldLeds; i++) {
            const led = document.createElement("div");
            led.classList.add("led");
            const angle = (i / numGameFieldLeds) * 2 * Math.PI;
            const radius = 150; // Adjust the radius as needed
            led.style.left = `${200 + radius * Math.cos(angle)}px`;
            led.style.top = `${200 + radius * Math.sin(angle)}px`;
            gameFieldStrip.appendChild(led);
        }
        socket.onmessage = function(event) {
            // Create a FileReader to read the Blob as an ArrayBuffer
            const reader = new FileReader();
            
            reader.onload = function() {
                // Once the Blob is read, convert the result to a Uint8Array
                const arrayBuffer = reader.result;
                const colorData = new Uint8Array(arrayBuffer);
                
                const gameFieldLeds = gameFieldStrip.getElementsByClassName("led");
                
                for (let i = 0; i < numGameFieldLeds; i++) {
                    const r = colorData[i * 3];
                    const g = colorData[i * 3 + 1];
                    const b = colorData[i * 3 + 2];
                    gameFieldLeds[i].style.backgroundColor = `rgb(${r}, ${g}, ${b})`;
                }
            };
            
            // Read the Blob as an ArrayBuffer
            reader.readAsArrayBuffer(event.data);
        };
        function sendColorChange(btn) {
            fetch(`/BTN/${btn}`)
                .then(response => response.text())
                .then(data => {
                    console.log(data);
                    socket.send(new Uint8Array([0x01]));
                });
        }
    </script>
</body>
</html>
)rawliteral";

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

void sendColorData(uint8_t num);
void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length);

void setup() {
  delay(3500);
  WiFi.softAP(ssid, password);

  webSocket.begin();
  webSocket.onEvent(webSocketEvent);

  // Initialize game field LEDs
  FastLED.addLeds<LED_TYPE, GAME_FIELD_LED_DATA_PIN, COLOR_ORDER>(game.game_field_leds, NUM_GAME_FIELD_LEDS)
    .setCorrection(TypicalLEDStrip)
    .setDither(BRIGHTNESS < 255);

  // Initialize player buttons LEDs
  FastLED.addLeds<LED_TYPE, GAME_BUTTONS_LED_DATA_PIN, COLOR_ORDER>(game.game_player_buttons_leds, 6)
    .setCorrection(TypicalLEDStrip)
    .setDither(BRIGHTNESS < 255);

  FastLED.setBrightness(BRIGHTNESS);

  // Initialize buzzer
  pinMode(BUZZER_PIN, OUTPUT);

  // Initialize game
  game = Game();

  // Set up interrupts for player buttons
  attachInterrupt(digitalPinToInterrupt(BTN1_PIN), onButtonPress1, FALLING);
  attachInterrupt(digitalPinToInterrupt(BTN2_PIN), onButtonPress2, FALLING);
  attachInterrupt(digitalPinToInterrupt(BTN3_PIN), onButtonPress3, FALLING);
  attachInterrupt(digitalPinToInterrupt(BTN4_PIN), onButtonPress4, FALLING);
  attachInterrupt(digitalPinToInterrupt(BTN5_PIN), onButtonPress5, FALLING);
  attachInterrupt(digitalPinToInterrupt(BTN6_PIN), onButtonPress6, FALLING);
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

void loop() {
  game.render();
  digitalWrite(BUZZER_PIN, game.buzzer_active);
  
  FastLED.show();
  //webSocket.broadcastBIN((uint8_t*) "\x01", 1);
  sendColorData(0x02);

  uint32_t current_time = millis();
  while (millis() - current_time < 20) {
    webSocket.loop();
  }
}

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {
  if (type == WStype_BIN) {
    if (payload[0] == 0x01) { // "GET_COLOR"
      sendColorData(num);
    }
  }
}

void sendColorData(uint8_t num) {
  uint8_t colorData[NUM_GAME_FIELD_LEDS * 3 + 6 * 3];

  for (int i = 0; i < NUM_GAME_FIELD_LEDS; i++) {
    colorData[i * 3]     = game.game_field_leds[i].r;  // R
    colorData[i * 3 + 1] = game.game_field_leds[i].g;  // G
    colorData[i * 3 + 2] = game.game_field_leds[i].b;  // B
  }
  
  for (int i = 0; i < 6; i++) {
    colorData[NUM_GAME_FIELD_LEDS * 3 + i * 3]     = game.game_player_buttons_leds[i].r;  // R
    colorData[NUM_GAME_FIELD_LEDS * 3 + i * 3 + 1] = game.game_player_buttons_leds[i].g;  // G
    colorData[NUM_GAME_FIELD_LEDS * 3 + i * 3 + 2] = game.game_player_buttons_leds[i].b;  // B
  }

  webSocket.broadcastBIN(colorData, sizeof(colorData));
}
