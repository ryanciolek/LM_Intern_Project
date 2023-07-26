#include "MeMegaPi.h"

// Initialize motors
MeMegaPiDCMotor motor1(PORT1A);
MeMegaPiDCMotor motor2(PORT1B);
MeMegaPiDCMotor motor3(PORT2A);
MeMegaPiDCMotor motor4(PORT2B);

// Set motor speed
uint8_t motorSpeed = 100;

// Pins where the line sensors are connected
const int lineSensor1Pin = A9;
const int lineSensor2Pin = A10;

void setup() {
  // Set the line sensor pins as inputs
  pinMode(lineSensor1Pin, INPUT);
  pinMode(lineSensor2Pin, INPUT);
}

void loop() {
  // Read the state of the line sensors
  int sensor1State = digitalRead(lineSensor1Pin);
  int sensor2State = digitalRead(lineSensor2Pin);

  // If both sensors are over the line, move forward
  if(sensor1State == HIGH && sensor2State == HIGH) {
    motor1.run(motorSpeed);
    motor2.run(motorSpeed);
    motor3.run(motorSpeed);
    motor4.run(motorSpeed);
  }
  // If the left sensor is over the line, turn left
  else if(sensor1State == HIGH && sensor2State == LOW) {
    motor1.run(-motorSpeed);
    motor2.run(-motorSpeed);
    motor3.run(motorSpeed);
    motor4.run(motorSpeed);
  }
  // If the right sensor is over the line, turn right
  else if(sensor1State == LOW && sensor2State == HIGH) {
    motor1.run(motorSpeed);
    motor2.run(motorSpeed);
    motor3.run(-motorSpeed);
    motor4.run(-motorSpeed);
  }
  // If neither sensor is over the line, stop
  else {
    motor1.stop();
    motor2.stop();
    motor3.stop();
    motor4.stop();
  }
}
