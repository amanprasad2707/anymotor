#include <Arduino.h>
#include "AnyMotor.h"

// Motor pins
#define IN1 2
#define IN2 3
#define EN  5

Motor motor1(IN1, IN2, EN);

void setup() {
  Serial.begin(9600);
  motor1.init();
  motor1.setDeadBand(30); // Minimum PWM to actually move the motor
  delay(1000);            // Short pause before starting
}

void loop() {

  // ===== Forward at full speed =====
  Serial.println("Forward at full speed (255)");
  motor1.forward();      
  delay(2000);

  // ===== Coasting stop =====
  Serial.println("Coasting stop: motor gradually slows down");
  motor1.stop();         
  delay(1500);

  // ===== Backward at half speed =====
  Serial.println("Backward at half speed (128)");
  motor1.backward(128);  
  delay(2000);

  // ===== Braking =====
  Serial.println("Braking: motor stops immediately");
  motor1.brake();        
  delay(1500);

  // ===== Gradual acceleration =====
  Serial.println("Gradually accelerating forward");
  motor1.stop();          // Ensure motor starts from stop
  for (uint8_t i = motor1.getMinSpeed(); i <= motor1.getMaxSpeed(); i += 50) {
    motor1.setSpeed(i);
    motor1.forward();
    Serial.print("Speed: ");
    Serial.println(i);
    delay(500);
  }

  // ===== Gradual deceleration =====
  Serial.println("Gradually decelerating forward");
  for (int i = motor1.getMaxSpeed(); i >= motor1.getMinSpeed(); i -= 50) {
    motor1.setSpeed(i);
    motor1.forward();
    Serial.print("Speed: ");
    Serial.println(i);
    delay(500);
  }

  // Pause before repeating
  Serial.println("Demo complete. Restarting loop...");
  delay(2000);
}
