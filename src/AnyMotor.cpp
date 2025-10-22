#include "AnyMotor.h"

// ===== Constructor =====
// Creates a motor object with specified pins
// Parameters: in1 = direction pin 1, in2 = direction pin 2, en = enable/speed pin (PWM)
// Initializes: speed=255, range=0-255, no deadband, stopped, not inverted
Motor::Motor(uint8_t in1, uint8_t in2, uint8_t en)
  : _in1(in1), _in2(in2), _en(en), 
    _speed(255), _min_speed(0), _max_speed(255), _deadband(0),
    _currentDirection(MOTOR_STOPPED), _inverted(false) {}

// ===== Initialization =====
void Motor::init() {
  pinMode(_in1, OUTPUT);
  pinMode(_in2, OUTPUT);
  pinMode(_en, OUTPUT);
  stop();
}

// ===== Basic Motion Control =====
void Motor::forward() {
  _currentDirection = MOTOR_FORWARD;
  _applyMotion();
}

void Motor::backward() {
  _currentDirection = MOTOR_BACKWARD;
  _applyMotion();
}

void Motor::stop() {
  _currentDirection = MOTOR_STOPPED;
  digitalWrite(_in1, LOW);
  digitalWrite(_in2, LOW);
  analogWrite(_en, 0);
}

void Motor::brake() {
  _currentDirection = MOTOR_STOPPED;
  digitalWrite(_in1, HIGH);
  digitalWrite(_in2, HIGH);
  analogWrite(_en, 255);  // Full brake force
}

// ===== Motion Control with Speed =====
void Motor::forward(uint8_t speed) {
  setSpeed(speed);
  forward();
}

void Motor::backward(uint8_t speed) {
  setSpeed(speed);
  backward();
}

// ===== Timed Motion Control =====
void Motor::forward(uint8_t speed, unsigned long duration) {
  forward(speed);
  delay(duration);
  stop();
}

void Motor::backward(uint8_t speed, unsigned long duration) {
  backward(speed);
  delay(duration);
  stop();
}

// ===== Speed Control =====
void Motor::setSpeed(uint8_t speed) {
  // Apply deadband compensation
  if (speed > 0 && speed < _deadband) {
    speed = _deadband;
  }
  
  _speed = constrain(speed, _min_speed, _max_speed);
  
  // Apply speed if motor is running
  if (_currentDirection != MOTOR_STOPPED) {
    _applyMotion();
  }
}

void Motor::setSpeedRange(uint8_t min_speed, uint8_t max_speed) {
  _min_speed = min_speed;
  _max_speed = max_speed;
  // Re-constrain current speed
  _speed = constrain(_speed, _min_speed, _max_speed);
}

void Motor::setDeadBand(uint8_t deadband) {
  _deadband = deadband;
}

// ===== Smooth Speed Control =====
void Motor::accelerate(uint8_t increment) {
    setSpeed(constrain(_speed + increment, _min_speed, _max_speed));
}

void Motor::decelerate(uint8_t decrement) {
    setSpeed(constrain(_speed - decrement, _min_speed, _max_speed));
}


void Motor::toggle() {
  if (_currentDirection == MOTOR_FORWARD) {
    backward();
  } else if (_currentDirection == MOTOR_BACKWARD) {
    forward();
  }
}

void Motor::setInverted(bool inverted) {
  _inverted = inverted;
  // Re-apply current direction with new inversion setting
  if (_currentDirection != MOTOR_STOPPED) {
    _applyMotion();
  }
}


// Forward: both motors forward
void moveForward(Motor &left, Motor &right, uint8_t speed = 255) {
  left.forward(speed);
  right.forward(speed);
}

// Backward: both motors backward
void moveBackward(Motor &left, Motor &right, uint8_t speed = 255) {
  left.backward(speed);
  right.backward(speed);
}

// Turn left: left motor backward, right motor forward
void turnLeft(Motor &left, Motor &right, uint8_t speed = 255) {
  left.backward(speed);
  right.forward(speed);
}

// Turn right: left motor forward, right motor backward
void turnRight(Motor &left, Motor &right, uint8_t speed = 255) {
  left.forward(speed);
  right.backward(speed);
}

// Stop: both motors stop
void stopMotors(Motor &left, Motor &right) {
  left.stop();
  right.stop();
}




// ===== Getters =====
uint8_t Motor::getSpeed() const {
  return _speed;
}

uint8_t Motor::getMinSpeed() const {
  return _min_speed;
}

uint8_t Motor::getMaxSpeed() const {
  return _max_speed;
}

uint8_t Motor::getDeadBand() const {
  return _deadband;
}



bool Motor::isRunning() const {
  return _currentDirection != MOTOR_STOPPED;
}

bool Motor::isInverted() const {
  return _inverted;
}

Motor::MotorDirection Motor::getDirection() const {
  return _currentDirection;
}

// ===== Private Helper Function =====
void Motor::_applyMotion() {
  
  // If inverted is true, swap forward/backward
  bool actualForward = (_currentDirection == MOTOR_FORWARD) ^ _inverted;
  
  if (actualForward) {
    digitalWrite(_in1, HIGH);
    digitalWrite(_in2, LOW);
  } else {
    digitalWrite(_in1, LOW);
    digitalWrite(_in2, HIGH);
  }
  
  analogWrite(_en, _speed);
}