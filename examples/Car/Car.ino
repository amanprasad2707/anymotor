#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "AnyMotor.h"


/******************************************************
    NOTE — ESP32 ONLY

  This code has been tested and verified on ESP32 boards.
  It will NOT work on ESP8266, AVR (Arduino Uno/Nano), 
  STM32, ESP32-CAM, or any other architecture without 
  modification.

  Make sure your board selection in the Arduino IDE is:
      Tools → Board → ESP32 Dev Module
******************************************************/



// -------------------- MOTOR SETUP --------------------
#define L_IN1 12
#define L_IN2 13
#define L_EN  15

#define R_IN1 14
#define R_IN2 18
#define R_EN  19

Motor leftMotor(L_IN1, L_IN2, L_EN);
Motor rightMotor(R_IN1, R_IN2, R_EN);

int currentSpeed = 150;   // Default speed

// -------------------- LIGHT PIN --------------------
#define LIGHT_PIN 2

// -------------------- MOVEMENT CONSTANTS --------------------
#define UP     1
#define DOWN   2
#define LEFT   3
#define RIGHT  4
#define STOP   0

// -------------------- WIFI + WEBSERVER --------------------
const char* ssid     = "MyWiFiCar";
const char* password = "12345678";

AsyncWebServer server(80);
AsyncWebSocket wsCar("/CarInput");

// -------------------- HTML PAGE --------------------
const char* htmlHomePage PROGMEM = R"HTML(
<!DOCTYPE html>
<html>
  <head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
    <style>
      .arrows { font-size:40px; color:red; }
      td.button { background:black; border-radius:25%; box-shadow:5px 5px #888; }
      td.button:active { transform:translate(5px,5px); box-shadow:none; }
      body { user-select:none; }
      .slider { width:100%; height:15px; background:#d3d3d3; border-radius:5px; }
    </style>
  </head>

  <body align="center">
    <table style="width:300px;margin:auto;" cellspacing="10">
      <tr>
        <td></td>
        <td class="button" ontouchstart='sendCmd("MoveCar","1")' ontouchend='sendCmd("MoveCar","0")'>
          <span class="arrows">&#8679;</span>
        </td>
        <td></td>
      </tr>

      <tr>
        <td class="button" ontouchstart='sendCmd("MoveCar","3")' ontouchend='sendCmd("MoveCar","0")'>
          <span class="arrows">&#8678;</span>
        </td>
        <td></td>
        <td class="button" ontouchstart='sendCmd("MoveCar","4")' ontouchend='sendCmd("MoveCar","0")'>
          <span class="arrows">&#8680;</span>
        </td>
      </tr>

      <tr>
        <td></td>
        <td class="button" ontouchstart='sendCmd("MoveCar","2")' ontouchend='sendCmd("MoveCar","0")'>
          <span class="arrows">&#8681;</span>
        </td>
        <td></td>
      </tr>

      <tr><td colspan=3><b>Speed:</b>
        <input type="range" min="0" max="255" value="150" class="slider"
               oninput='sendCmd("Speed",value)'>
      </td></tr>

      <tr><td colspan=3><b>Light:</b>
        <input type="range" min="0" max="255" value="0" class="slider"
               oninput='sendCmd("Light",value)'>
      </td></tr>

    </table>

    <div style="margin-top:20px; font-size:14px; color:#444;">
      Developed by <b>Aman Prasad</b>
    </div>

    <script>
      var ws = new WebSocket("ws://" + location.hostname + "/CarInput");

      function sendCmd(key, value) {
        ws.send(key + "," + value);
      }
    </script>
  </body>
</html>
)HTML";


// -------------------- MOTOR FUNCTIONS --------------------
void moveForward() {
  leftMotor.forward(currentSpeed);
  rightMotor.forward(currentSpeed);
}

void moveBackward() {
  leftMotor.backward(currentSpeed);
  rightMotor.backward(currentSpeed);
}

void turnLeft() {
  leftMotor.backward(currentSpeed);
  rightMotor.forward(currentSpeed);
}

void turnRight() {
  leftMotor.forward(currentSpeed);
  rightMotor.backward(currentSpeed);
}

void stopMotors() {
  leftMotor.stop();
  rightMotor.stop();
}

void handleMovement(int cmd) {
  switch (cmd) {
    case UP:    moveForward(); break;
    case DOWN:  moveBackward(); break;
    case LEFT:  turnLeft(); break;
    case RIGHT: turnRight(); break;
    case STOP:  stopMotors(); break;
    default:    stopMotors(); break;
  }
}

// -------------------- WEBSOCKET HANDLER --------------------
void onCarEvent(AsyncWebSocket *server, AsyncWebSocketClient *client,
                AwsEventType type, void *arg, uint8_t *data, size_t len)
{
  if (type == WS_EVT_DATA) {
    String msg = String((char*)data).substring(0, len);
    int comma = msg.indexOf(',');

    String key = msg.substring(0, comma);
    int val = msg.substring(comma + 1).toInt();

    if (key == "MoveCar") { handleMovement(val); }
    if (key == "Speed")   { currentSpeed = val; }
    if (key == "Light")   { analogWrite(LIGHT_PIN, val); }
  }
}

// -------------------- SETUP --------------------
void setup() {
  Serial.begin(115200);

  leftMotor.init();
  rightMotor.init();

  pinMode(LIGHT_PIN, OUTPUT);

  WiFi.softAP(ssid, password);
  Serial.println(WiFi.softAPIP());

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *req){
    req->send_P(200, "text/html", htmlHomePage);
  });

  wsCar.onEvent(onCarEvent);
  server.addHandler(&wsCar);

  server.begin();
}

// -------------------- LOOP --------------------
void loop() {
  wsCar.cleanupClients();
}
