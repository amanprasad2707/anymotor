#include <Arduino.h>
#include "AnyMotor.h"

#define IN1 2
#define IN2 3
#define EN  5

Motor motor(IN1, IN2, EN);

void setup() {
  Serial.begin(9600);
  motor.init();
  motor.setDeadBand(20);
}

void loop() {
  motor.forward();

  // Accelerate gradually to max
  Serial.println("Accelerating...");
  for (int i = 0; i <= 255; i += 25) {
    motor.accelerate(25);
    Serial.print("Speed: ");
    Serial.println(motor.getSpeed());
    delay(300);
  }

  // Decelerate gradually to stop
  Serial.println("Decelerating...");
  while (motor.getSpeed() > 0) {
    motor.decelerate(30);
    Serial.print("Speed: ");
    Serial.println(motor.getSpeed());
    delay(300);
  }

  motor.stop();
  delay(2000);
}
