/*
 * AnyMotor Library
 * Universal motor driver library for H-bridge motor drivers
 * Compatible with: L298N, L293D, TB6612, MX1508, and any driver with IN1, IN2, EN pins
 * 
 * Author: Aman Prasad
 * Version: 1.0.0
 * License: MIT
 */

#ifndef ANYMOTOR_H
#define ANYMOTOR_H

#include "Arduino.h"
#include <stdint.h>

class Motor {
public:
  // ===== ENUMS =====
  
  // Motor direction states
  enum MotorDirection {
    MOTOR_FORWARD,   // Motor spinning forward
    MOTOR_BACKWARD,  // Motor spinning backward
    MOTOR_STOPPED    // Motor not moving
  };


private:
  // ===== HARDWARE PINS =====
  uint8_t _in1;  // Direction pin 1 (connected to motor driver IN1)
  uint8_t _in2;  // Direction pin 2 (connected to motor driver IN2)
  uint8_t _en;   // Enable/PWM pin (connected to motor driver EN/ENA)

  // ===== SPEED SETTINGS =====
  uint8_t _speed;       // Current motor speed (0-255)
  uint8_t _min_speed;   // Minimum allowed speed (default: 0)
  uint8_t _max_speed;   // Maximum allowed speed (default: 255)
  uint8_t _deadband;    // Minimum speed where motor actually moves (compensates for startup threshold)

  // ===== STATE TRACKING =====
  MotorDirection _currentDirection;  // Current direction (forward/backward/stopped)
  bool _inverted;                    // If true, forward/backward are swapped

  // ===== INTERNAL HELPER =====
  void _applyMotion();  // Applies current speed and direction to motor hardware

public:

  // ===== Constructor & Initialization =====
  // Creates motor object with specified pins
  Motor(uint8_t in1, uint8_t in2, uint8_t en);
  
  // Initializes motor pins as outputs and stops motor
  // MUST be called in setup() before using motor functions
  void init();

  // ===== Basic Motion Control =====
  
  // Moves motor forward at current speed
  void forward();
  
  // Moves motor backward at current speed
  void backward();
  
  // Stops motor by coasting (disconnects power, gradual stop)
  void stop();
  
  // Stops motor with active braking (shorts terminals, quick stop)
  void brake();

  // ===== Motion Control with Speed =====
  
  // Sets speed and moves forward (convenience function)
  void forward(uint8_t speed);
  
  // Sets speed and moves backward (convenience function)
  void backward(uint8_t speed);

  // ===== Timed Motion Control =====
  
  // Moves forward for specified duration then stops (BLOCKING - uses delay)
  void forward(uint8_t speed, unsigned long duration);
  
  // Moves backward for specified duration then stops (BLOCKING - uses delay)
  void backward(uint8_t speed, unsigned long duration);

  // ===== Speed Control =====
  
  // Sets motor speed (0-255) with automatic deadband and range constraints
  void setSpeed(uint8_t speed);
  
  // Sets minimum and maximum speed limits (all speeds constrained to this range)
  void setSpeedRange(uint8_t min_speed, uint8_t max_speed);
  
  // Sets deadband - minimum speed where motor actually moves (e.g., 50)
  void setDeadBand(uint8_t deadband);
  
  
  // Increases speed by specified amount (default max: 255)
  void accelerate(uint8_t increment);
  
  // Decreases speed by specified amount (default min: 0)
  void decelerate(uint8_t decrement);
  

  
  // Toggles between forward and backward
  void toggle();
  
  // Inverts motor direction (for reversed wiring)
  void setInverted(bool inverted);



  // ===== Getters (const for read-only) =====
  
  // Returns current speed (0-255)
  uint8_t getSpeed() const;
  
  // Returns minimum speed limit
  uint8_t getMinSpeed() const;
  
  // Returns maximum speed limit
  uint8_t getMaxSpeed() const;
  
  // Returns deadband value
  uint8_t getDeadBand() const;
  
  
  // Returns true if motor is moving
  bool isRunning() const;
  
  // Returns true if direction is inverted
  bool isInverted() const;
  
  // Returns current direction (MOTOR_FORWARD, MOTOR_BACKWARD, or MOTOR_STOPPED)
  MotorDirection getDirection() const;
};


/**
 * @brief Move both motors forward
 * @param left Reference to the left motor
 * @param right Reference to the right motor
 * @param speed Motor speed (0-255), default = 255 (max speed)
 */
void moveForward(Motor &left, Motor &right, uint8_t speed = 255);

/**
 * @brief Move both motors backward
 * @param left Reference to the left motor
 * @param right Reference to the right motor
 * @param speed Motor speed (0-255), default = 255 (max speed)
 */
void moveBackward(Motor &left, Motor &right, uint8_t speed = 255);

/**
 * @brief Turn the car left
 * Left motor moves backward, right motor moves forward
 * @param left Reference to the left motor
 * @param right Reference to the right motor
 * @param speed Motor speed (0-255), default = 255 (max speed)
 */
void turnLeft(Motor &left, Motor &right, uint8_t speed = 255);

/**
 * @brief Turn the car right
 * Left motor moves forward, right motor moves backward
 * @param left Reference to the left motor
 * @param right Reference to the right motor
 * @param speed Motor speed (0-255), default = 255 (max speed)
 */
void turnRight(Motor &left, Motor &right, uint8_t speed = 255);

/**
 * @brief Stop both motors
 * @param left Reference to the left motor
 * @param right Reference to the right motor
 */
void stopMotors(Motor &left, Motor &right);



#endif // ANYMOTOR_H