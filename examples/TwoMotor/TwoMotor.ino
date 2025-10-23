#include <Arduino.h>
#include "AnyMotor.h"

// Left motor pins
#define L_IN1 2
#define L_IN2 3
#define L_EN 5

// Right motor pins
#define R_IN1 4
#define R_IN2 7
#define R_EN 6

Motor leftMotor(L_IN1, L_IN2, L_EN);
Motor rightMotor(R_IN1, R_IN2, R_EN);

void setup() {
  Serial.begin(9600);
  leftMotor.init();
  rightMotor.init();
}

void loop() {
  Serial.println("Moving forward");
  moveForward(leftMotor, rightMotor); // Max speed
  delay(2000);

  Serial.println("Turning left");
  turnLeft(leftMotor, rightMotor, 180); // Half speed
  delay(1500);

  Serial.println("Turning right");
  turnRight(leftMotor, rightMotor, 200);
  delay(1500);

  Serial.println("Moving backward");
  moveBackward(leftMotor, rightMotor, 150);
  delay(2000);

  Serial.println("Stopping");
  stopMotors(leftMotor, rightMotor);
  delay(1000);
}
